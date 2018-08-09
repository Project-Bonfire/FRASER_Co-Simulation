#define SC_INCLUDE_DYNAMIC_PROCESSES

#include "systemc"
using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"

#include "packet_sink.hpp"

struct sc_pkt_sink: sc_module, PacketSink
{
    // TLM-2 socket, defaults to 32-bits wide, base protocol
    tlm_utils::simple_target_socket<sc_pkt_sink> socket;

    int pkt; 

    SC_CTOR(sc_pkt_sink) : socket("socket"), PacketSink(1)
    {
        socket.register_b_transport(this, &sc_pkt_sink::b_transport);
    };

    virtual void b_transport( tlm::tlm_generic_payload& trans, sc_time& delay )
    {
        // parse the transaction
        unsigned char*   ptr = trans.get_data_ptr();
        unsigned int     len = trans.get_data_length();
        unsigned char*   byt = trans.get_byte_enable_ptr();
        unsigned int     wid = trans.get_streaming_width();

        memcpy(&pkt, ptr, len);
        cout << hex << "Sink received pkt: 0x" << pkt << endl;

        trans.set_response_status( tlm::TLM_OK_RESPONSE );
    }
};

