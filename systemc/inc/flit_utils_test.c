#include "flit_utils.h"
#include <assert.h>
void main()
{
    printf("Test flit utils\n");

    ///////////////////////////
    //   Header flit tests   //
    ///////////////////////////
    unsigned int h_flit;
    //make a new header-flit
    make_header_flit(&h_flit, 1, 1, 1, 1);
    printf("\nOrig header-flit: 0x%X\n", h_flit);
    //update ID to 5
    set_headerflit_id(&h_flit, 5);
    printf("header-flit with ID 5: 0x%X\n", h_flit);
    //verify all fields as expected
    unsigned int len, dest, src, id, p;
    parse_header_flit(h_flit, &len, &dest, &src, &id, &p);
    assert(len  == 1);
    assert(dest == 1);
    assert(src  == 1);
    assert(id   == 5); //updated ID
    printf("Parsed ID : %d\n", id);

    ///////////////////////////
    //   Body flit tests     //
    ///////////////////////////
    unsigned int b_flit;
    //make a new body-flit
    make_body_flit(&b_flit, 1);
    printf("\nOrig body-flit: 0x%X\n", b_flit);
    //update payload
    int payld = 0x6600;
    set_bodyflit_payload(&b_flit, payld);
    printf("body-flit with payload 0x%X: 0x%X\n", payld, b_flit);
    //verify all fields as expected
    unsigned int payload;
    parse_body_flit(b_flit, &payload, &p);
    assert(payload  == payld); //updated payload
    printf("Parsed payload : 0x%X\n", payload);

    ///////////////////////////
    //   Tail flit tests     //
    ///////////////////////////
    unsigned int t_flit;
    //make a new tail-flit
    make_tail_flit(&t_flit, 3);
    printf("\nOrig tail-flit: 0x%X\n", t_flit);
    //update payload
    payld = 0x8000000;
    set_tailflit_payload(&t_flit, payld);
    printf("tail-flit with payload 0x%X: 0x%X\n", payld, t_flit);
    //verify all fields as expected
    parse_tail_flit(t_flit, &payload, &p);
    assert(payload  == payld); //updated payload
    printf("Parsed payload : 0x%X\n", payload);

}
