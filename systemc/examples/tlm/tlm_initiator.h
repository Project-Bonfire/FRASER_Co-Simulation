#define SC_INCLUDE_DYNAMIC_PROCESSES

#include "systemc"
using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"

struct Initiator: sc_module
{
    tlm_utils::simple_initiator_socket<Initiator> socket;

    SC_CTOR(Initiator) : socket("socket")
    {
        SC_THREAD(thread_process);
    }
    void thread_process()
    {
        cout << endl << "In thread_process" << endl;
        tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
        sc_time delay = sc_time(10, SC_NS);

        int data;
        int i = 4;
        tlm::tlm_command cmd = static_cast<tlm::tlm_command>(rand() % 2);
        //static_cast(rand() % 2);
        if (cmd == tlm::TLM_WRITE_COMMAND) data = 0xFF000000 | i;

        trans->set_command( cmd );
        trans->set_address( i );
        data=0x100;
        trans->set_data_ptr( reinterpret_cast<unsigned char*>(&data) );
        trans->set_data_length( 4 );
        trans->set_streaming_width( 4 );
        trans->set_byte_enable_ptr( 0 );
        trans->set_dmi_allowed( false );
        trans->set_response_status( tlm::TLM_INCOMPLETE_RESPONSE );

        cout << endl << "Starting from Initiator" << endl;
        socket->b_transport( *trans, delay ); //blocking call
        cout << endl << "Done from Initiator" << endl;

        if ( trans->is_response_error() )
        {
            char txt[100];
            sprintf(txt, "Error from b_transport, response status = %s",
                    trans->get_response_string().c_str());
            SC_REPORT_ERROR("TLM-2", txt);

        }

    }

};

