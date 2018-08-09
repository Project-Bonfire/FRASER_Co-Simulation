#define SC_INCLUDE_DYNAMIC_PROCESSES

#include "systemc"
using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"

#include "packet_generator.hpp"

struct sc_pkt_gen: sc_module
                  , PacketGenerator
{
    // TLM-2 socket, defaults to 32-bits wide, base protocol
    tlm_utils::simple_initiator_socket<sc_pkt_gen> socket;

    SC_CTOR(sc_pkt_gen) : socket("socket")
                         , PacketGenerator(2)
    {
        SC_THREAD(thread_process);
    }

    void thread_process()
    {
        cout << endl << "In pkt-gen thread_process" << endl;
	
        // TLM-2 generic payload transaction, reused across calls to b_transport
        tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
        sc_time delay = sc_time(10, SC_NS);

        int data;

        cout << "Generating packets..." << endl;
        generate_packet(10, 1, GenerationModes::counter);

        // Send packets 1-by-1
        int cnt = get_packet_count();
        for(int i=0; i<cnt; i++)
        {
            // get next packet
            data = get_packet_at(i);

            // set the transaction
            trans->set_data_ptr( reinterpret_cast<unsigned char*>(&data) );
            trans->set_data_length( 4 );
            trans->set_streaming_width( 4 );
            trans->set_byte_enable_ptr( 0 );
            trans->set_dmi_allowed( false );
            trans->set_response_status( tlm::TLM_INCOMPLETE_RESPONSE );

            cout << "Gen sending pkt-" << dec << i << ": 0x" << hex << data << endl;
            socket->b_transport( *trans, delay ); //blocking call

            if ( trans->is_response_error() )
            {
                char txt[100];
                sprintf(txt, "Error from b_transport, response status = %s",
                        trans->get_response_string().c_str());
                SC_REPORT_ERROR("TLM-2", txt);
            }
        } //for(int i=0; i<cnt; i++)
    } //void thread_process()
};

