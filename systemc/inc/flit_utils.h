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

/***************************
** Flit encodings
****************************
Header flit:
Bits        Field
31,30,29    001 (header flit)
28-17       Length of the packet, upto 4096 flits.
16-13       Destination router address
12-9        Source router address.
8-1         Packet ID
0           Parity

Body flit:
Bits        Field
31,30,29    010 (body flit)
28-1        Payload
0           Parity

Tail flit:
Bits        Field
31,30,29    100 (tail flit)
28-1        Payload
0           Parity
****************************/

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

