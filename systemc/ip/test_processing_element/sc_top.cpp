#ifndef NO_SYSTEMC
#define SC_INCLUDE_DYNAMIC_PROCESSES

#include "systemc"
using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"

#include "sc_pkt_gen.h"
#include "sc_pkt_sink.h"

SC_MODULE(Top)
{
    sc_pkt_gen  *my_pkt_gen;
    sc_pkt_sink *my_pkt_sink;

    SC_CTOR(Top)
    {
        my_pkt_gen  = new sc_pkt_gen("my_pkt_gen");
        my_pkt_sink = new sc_pkt_sink   ("my_pkt_sink");
        my_pkt_gen->socket.bind( my_pkt_sink->socket );
    }
};

int sc_main(int argc, char* argv[])
{
    cout << endl << "Hello world from SystemC !" << endl;

    Top top("top");
    sc_start();

    return 0;
}
#endif // NO_SYSTEMC
