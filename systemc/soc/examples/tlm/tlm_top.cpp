#define SC_INCLUDE_DYNAMIC_PROCESSES

#include "systemc"
using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"

#include "tlm_initiator.h"
#include "tlm_target.h"

SC_MODULE(Top)
{
    Initiator *initiator;
    Target    *target;

    SC_CTOR(Top)
    {
        initiator = new Initiator("initiator");
        target    = new Target   ("target");
        initiator->socket.bind( target->socket );
    }

};

int sc_main(int argc, char* argv[])
{
    cout << endl << "Hello world from SystemC !" << endl;

    Top top("top");
    sc_start();
    return 0;
}

