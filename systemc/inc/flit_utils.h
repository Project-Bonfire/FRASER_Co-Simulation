#include <stdio.h>
#include <stdint.h>

#define HEADER_FLIT 1
#define BODY_FLIT   2
#define TAIL_FLIT   4

//#define DEBUG

#ifdef DEBUG
# define DBG_PRINT(x) printf x
#else
# define DBG_PRINT(x) do {} while (0)
#endif

// Clear a bit at position 'p' in the value '*val'
void clear_bit(uint32_t *val, uint32_t p)
{
    DBG_PRINT(("DEBUG: clear_bit val: 0x%x, pos %d\n", *val, p));
    uint32_t mask = 1<<p; //Set the bit to be cleared
    *val = *val & ~mask;
}

// Clear 'range' bits starting at position 'start' in the value '*val'
void clear_bits(uint32_t *val, uint32_t start, uint32_t range)
{
    int i;
    for(i=0 ; i<range; i++)
    {
        clear_bit(val, start+i);
    }
}

// Return bit-range from 'start' in the value 'val'
uint32_t get_bit_range(uint32_t val, uint32_t start, uint32_t range)
{
    uint32_t i, result=0;

    //for each interation, extract bit at (start+i)
    //and insert at left-shifted position of 'i' in result.
    for(i=0 ; i<range; i++)
    {
        result |= ((val >> (start+i)) & 1) << i;
    }

    return result;
}


//Compute even parity of value '*f' and set bit[0]
void set_parity_bit(unsigned int *f)
{
    //clear the parity bit at location [0]
    //so that it is not taken in later computation
    clear_bit(f, 0);

    //www.graphics.stanford.edu/~seander/bithacks.html#ParityParallel
    //Compute parity in parallel
    unsigned int v = *f;  // word value to compute the parity of
    v ^= v >> 16;
    v ^= v >> 8;
    v ^= v >> 4;
    v &= 0xf;

    //Set bit[0]
    *f |= (0x6996 >> v) & 1;
}

///////////////////////////////////////////////////
//////                                     ////////
////// Methods to set each field of a flit ////////
//////                                     ////////
///////////////////////////////////////////////////

void set_flit_type(unsigned int *f, unsigned int v)
{
    int loc = 29;           //field location to update
    clear_bits(f, loc, 3);  //clear field location
    *f |= v << loc;         //insert field value

    set_parity_bit(f);      //update parity bit
}

void set_headerflit_len(unsigned int *f, unsigned int v)
{
    int loc = 17;           //field location to update
    clear_bits(f, loc, 12); //clear field location
    *f |= v << loc;         //insert field value

    set_parity_bit(f);      //update parity bit
}

void set_headerflit_dest_adrs(unsigned int *f, unsigned int v)
{
    int loc = 13;           //field location to update
    clear_bits(f, loc, 4);  //clear field location
    *f |= v << loc;         //insert field value

    set_parity_bit(f);      //update parity bit
}
void set_headerflit_src_adrs(unsigned int *f, unsigned int v)
{
    int loc = 9;            //field location to update
    clear_bits(f, loc, 4);  //clear field location
    *f |= v << loc;         //insert field value

    set_parity_bit(f);      //update parity bit
}

void set_headerflit_id(unsigned int *f, unsigned int v)
{
    int loc = 1;            //field location to update
    clear_bits(f, loc, 8);  //clear field location
    *f |= v << loc;         //insert field value

    set_parity_bit(f);      //update parity bit
}

/////////////////////////////////////////////////////
////                                             ////
//// Methods to make a header, body or tail flit ////
////                                             ////
/////////////////////////////////////////////////////

/***************************
** HEADER flit encoding
****************************
Bits        Field
31,30,29    001 (header flit)
28-17       Length of the packet, upto 4096 flits.
16-13       Destination router address
12-9        Source router address.
8-1         Packet ID
0           Parity
****************************/
//Make a header-flit in '*h_flit' with the given
//length, destination address, source address and packet ID
void make_header_flit(unsigned int *h_flit, unsigned int len, unsigned int dest_adrs, unsigned int src_adrs, unsigned int id)
{
    *h_flit = 0;

    set_flit_type(h_flit, HEADER_FLIT);

    set_headerflit_len(h_flit, len);
    set_headerflit_dest_adrs(h_flit, dest_adrs);
    set_headerflit_src_adrs(h_flit, src_adrs);
    set_headerflit_id(h_flit, id);
} //end void make_header_flit()

/***************************
** BODY flit encoding
****************************
Bits        Field
31,30,29    010 (body flit)
28-1        Payload
0           Parity
****************************/
void set_bodyflit_payload(unsigned int *f, unsigned int v)
{
    int loc = 1;            //field location to update
    clear_bits(f, loc, 28); //clear field location
    *f |= v << loc;         //insert field value

    set_parity_bit(f);      //update parity bit
}
//Make a body-flit in '*b_flit' with the given payload
void make_body_flit(unsigned int *b_flit, unsigned int payld)
{
    *b_flit = 0;

    set_flit_type(b_flit, BODY_FLIT);

    set_bodyflit_payload(b_flit, payld);
} //end void make_body_flit()

/***************************
** TAIL flit encoding
****************************
Tail flit:
Bits        Field
31,30,29    100 (tail flit)
28-1        Payload
0           Parity
****************************/
void set_tailflit_payload(unsigned int *f, unsigned int v)
{
    int loc = 1;            //field location to update
    clear_bits(f, loc, 28); //clear field location
    *f |= v << loc;         //insert field value

    set_parity_bit(f);      //update parity bit
}
//Make a tail-flit in '*t_flit' with the given payload
void make_tail_flit(unsigned int *t_flit, unsigned int payld)
{
    *t_flit = 0;

    set_flit_type(t_flit, TAIL_FLIT);

    set_tailflit_payload(t_flit, payld);
} //end void make_tail_flit()

////////////////////////////////////////////////////////////////
////                                                        ////
//// Methods to get fields from a header, body or tail flit ////
////                                                        ////
////////////////////////////////////////////////////////////////

/***************************
** HEADER flit encoding
****************************
Bits        Field
31,30,29    001 (header flit)
28-17       Length of the packet, upto 4096 flits.
16-13       Destination router address
12-9        Source router address.
8-1         Packet ID
0           Parity
****************************/
//From header 'flit' return error=1 otherwise get
//length, destination address, source address, packet ID and parity
unsigned int parse_header_flit(unsigned int flit, unsigned int *len, unsigned int *dest_adrs, unsigned int *src_adrs, unsigned int *id, unsigned int *p)
{

    //check flit-type field(bits 31,30,29) to indicate HEADER_FLIT,
    //return 1 on error
    unsigned int flit_type = get_bit_range(flit, 29, 3);
    if(flit_type != HEADER_FLIT)
    {
        printf("Error! Got unexpected flit-type(%d) for header-flit type(%d)\n", flit_type, HEADER_FLIT);
        return 1;
    }

    *len        = get_bit_range(flit, 17, 28-17+1);
    *dest_adrs  = get_bit_range(flit, 13, 16-13+1);
    *src_adrs   = get_bit_range(flit, 9, 12-9+1);
    *id         = get_bit_range(flit, 1, 8-1+1);
    *p          = get_bit_range(flit, 0, 1);

    return 0; //No error
} //end parse_header_flit()

/***************************
** BODY flit encoding
****************************
Bits        Field
31,30,29    010 (body flit)
28-1        Payload
0           Parity
****************************/
//From body 'flit' return error=1 otherwise get payload and parity
unsigned int parse_body_flit(unsigned int flit, unsigned int *payload, unsigned int *p)
{
    //check flit-type field(bits 31,30,29) to indicate BODY_FLIT,
    //return 1 on error
    unsigned int flit_type = get_bit_range(flit, 29, 3);
    if(flit_type != BODY_FLIT)
    {
        printf("Error! Got unexpected flit-type(%d) for body-flit type(%d)\n", flit_type, BODY_FLIT);
        return 1;
    }

    *payload = get_bit_range(flit, 1, 28-1+1);
    *p       = get_bit_range(flit, 0, 1);

    return 0; //No error
} //end parse_body_flit()

/***************************
** TAIL flit encoding
****************************
Tail flit:
Bits        Field
31,30,29    100 (tail flit)
28-1        Payload
0           Parity
****************************/
//From tail 'flit' return error=1 otherwise get payload and parity
unsigned int parse_tail_flit(unsigned int flit, unsigned int *payload, unsigned int *p)
{
    //check flit-type field(bits 31,30,29) to indicate TAIL_FLIT,
    //return 1 on error
    unsigned int flit_type = get_bit_range(flit, 29, 3);
    if(flit_type != TAIL_FLIT)
    {
        printf("Error! Got unexpected flit-type(%d) for tail-flit type(%d)\n", flit_type, TAIL_FLIT);
        return 1;
    }

    *payload = get_bit_range(flit, 1, 28-1+1);
    *p       = get_bit_range(flit, 0, 1);

    return 0; //No error
} //end parse_tail_flit()

