#define SC_INCLUDE_DYNAMIC_PROCESSES

#include "systemc"
using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"

#include "packet_generator.h"
#include "packet_sink.h"

//Processing Element model
//has a
//- packet generator
//- packet sink
//- TLM initiator port
//- TLM target port
struct sc_proc_elem: sc_module
                  , PacketGenerator
                  , PacketSink
{
    // TLM-2 socket, defaults to 32-bits wide, base protocol
    tlm_utils::simple_initiator_socket<sc_proc_elem> init_socket; //to send packets
    tlm_utils::simple_target_socket<sc_proc_elem> tgt_socket;     //to receive packets

    PacketGenerator my_pkt_gen;
    PacketSink my_pkt_sink;

    int in_flit_counter;

    SC_CTOR(sc_proc_elem) : init_socket("init_socket")
                         , tgt_socket("tgt_socket")
    {
        //invokes pkt-generator and sends packets on the init_socket
        SC_THREAD(thread_process);
        //incoming packets on the tgt_socket are passed to the sink
        tgt_socket.register_b_transport(this, &sc_proc_elem::b_transport);
        //initialize packet_generator
        my_pkt_gen.init(1000 //uint16_t address
                  , 4 //NOC_NODE_COUNT //uint8_t nocSize
                  , GenerationModes::counter //GenerationModes generationMode
                  , 0.05 //double pir
                  , 3 //uint16_t minPacketLength
                  , 10 //uint16_t maxPacketLength
                  , 42 //uint64_t randomSeed
                  , 7000 //uint64_t generationEndTime
                  );
        my_pkt_sink.init(1000 //uint16_t address
                        );

        in_flit_counter = 0;

    }

    void thread_process()
    {
        cout << endl << "In pkt-gen thread_process" << endl;
	
        // TLM-2 generic payload transaction, reused across calls to b_transport
        tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
        sc_time delay = sc_time(10, SC_NS);

        uint32_t flit = 0;

        int cnt = 5;
        for(int i=0; i<cnt; i++)
        {
            // get next flit
            flit = my_pkt_gen.getFlit(i //uint64_t time
                                      );
            cout << "Flit from pkt-gen: 0x" << hex << flit << endl;

            // set the transaction
            trans->set_data_ptr( reinterpret_cast<unsigned char*>(&flit) );
            trans->set_data_length( 4 );
            trans->set_streaming_width( 4 );
            trans->set_byte_enable_ptr( 0 );
            trans->set_dmi_allowed( false );
            trans->set_response_status( tlm::TLM_INCOMPLETE_RESPONSE );

            cout << "ProcElem flit #" << dec << i << " --> 0x" << hex << flit << endl;
            init_socket->b_transport( *trans, delay ); //blocking call

            if ( trans->is_response_error() )
            {
                char txt[100];
                sprintf(txt, "Error from b_transport, response status = %s",
                        trans->get_response_string().c_str());
                SC_REPORT_ERROR("TLM-2", txt);
            }
        } //for(int i=0; i<cnt; i++)
    } //void thread_process()

    virtual void b_transport( tlm::tlm_generic_payload& trans, sc_time& delay )
    {
        unsigned int in_flit;
        // parse the transaction
        unsigned char*   ptr = trans.get_data_ptr();
        unsigned int     len = trans.get_data_length();
        unsigned char*   byt = trans.get_byte_enable_ptr();
        unsigned int     wid = trans.get_streaming_width();

        memcpy(&in_flit, ptr, len);
        cout << "ProcElem flit #" << dec << in_flit_counter++ << " <-- 0x" << hex << in_flit << endl;

        my_pkt_sink.putFlit(in_flit //uint32_t flit
                            , in_flit_counter //uint64_t time
                            );

        trans.set_response_status( tlm::TLM_OK_RESPONSE );
    }

};
