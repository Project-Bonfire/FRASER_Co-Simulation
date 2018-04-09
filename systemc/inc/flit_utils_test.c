#include "flit_utils.h"
void main()
{
    printf("Test flit utils\n");

    unsigned int h_flit;
    make_header_flit(&h_flit, 1, 1, 1, 1);
    printf("Orig header-flit: 0x%X\n", h_flit);

    set_headerflit_id(&h_flit, 5);
    printf("header-flit with ID 5: 0x%X\n", h_flit);

}
