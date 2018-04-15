#include "flit_utils.h"
void main()
{
    printf("Test flit utils\n");

    unsigned int h_flit;
    make_header_flit(&h_flit, 1, 1, 1, 1);
    printf("\nOrig header-flit: 0x%X\n", h_flit);

    set_headerflit_id(&h_flit, 5);
    printf("header-flit with ID 5: 0x%X\n", h_flit);

    unsigned int b_flit;
    make_body_flit(&b_flit, 1);
    printf("\nOrig body-flit: 0x%X\n", b_flit);

    int payld = 0x6600;
    set_bodyflit_payload(&b_flit, payld);
    printf("body-flit with payload 0x%X: 0x%X\n", payld, b_flit);

    unsigned int t_flit;
    make_tail_flit(&t_flit, 3);
    printf("\nOrig tail-flit: 0x%X\n", t_flit);

    payld = 0x8000000;
    set_tailflit_payload(&t_flit, payld);
    printf("tail-flit with payload 0x%X: 0x%X\n", payld, t_flit);

}
