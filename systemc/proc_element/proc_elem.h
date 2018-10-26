#define SC_INCLUDE_DYNAMIC_PROCESSES

#include "systemc"
using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"

#include "traffic_generator/packet_generator.h"
#include "traffic_generator/packet_sink.h"

//Processing Element model
//has a
//- packet generator
//- packet sink
//- TLM initiator port
//- TLM target port
struct sc_proc_elem: sc_module, PacketGenerator, PacketSink {
	// TLM-2 socket, defaults to 32-bits wide, base protocol
	tlm_utils::simple_initiator_socket<sc_proc_elem> init_socket; //to send packets
	tlm_utils::simple_target_socket<sc_proc_elem> tgt_memory_socket; //to receive packets
	tlm_utils::simple_target_socket<sc_proc_elem> tgt_credit_cnt_socket; //to update credit cnt
	tlm_utils::simple_target_socket<sc_proc_elem> tgt_interrupt_socket; //to stop PE

	PacketGenerator my_pkt_gen;
	PacketSink my_pkt_sink;

	int in_flit_counter;
	int credit_cnt_l;
	bool run;

	SC_CTOR(sc_proc_elem) :
			init_socket("init_socket"), tgt_memory_socket("tgt_socket_0"), tgt_credit_cnt_socket(
					"tgt_socket_1"), tgt_interrupt_socket("tgt_socket_2") {
		//invokes pkt-generator and sends packets on the init_socket
		//incoming packets on the tgt_socket are passed to the sink
		tgt_memory_socket.register_b_transport(this,
				&sc_proc_elem::store_in_memory);
		tgt_credit_cnt_socket.register_b_transport(this,
				&sc_proc_elem::update_credit_cnt);
		tgt_interrupt_socket.register_b_transport(this, &sc_proc_elem::stop);

		//initialize packet_generator
		my_pkt_gen.init(0000 //uint16_t address
				, 4 //NOC_NODE_COUNT //uint8_t nocSize
				, GenerationModes::random //GenerationModes generationMode
				, 0.05 //double pir
				, 3 //uint16_t minPacketLength
				, 10 //uint16_t maxPacketLength
				, 42 //uint64_t randomSeed
				, 3 //num of packets to generate
				);
		my_pkt_sink.init(0000 //uint16_t address
				);

		in_flit_counter = 0;
		credit_cnt_l = 3;
		run = true;

		SC_THREAD(thread_process);
	}

	void thread_process() {
		cout << endl << "In pkt-gen thread_process" << endl;

		// TLM-2 generic payload transaction, reused across calls to b_transport
		tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
		sc_time delay = sc_time(100, SC_MS);

		uint32_t flit = 0;

		while (run) {

			if (credit_cnt_l > 0) {
				// get next flit
				flit = my_pkt_gen.getFlit();

				if (flit != 0) {
					cout << "Flit from pkt-gen: " << flit << endl;

					// set the transaction
					trans->set_data_ptr(
							reinterpret_cast<unsigned char*>(&flit));
					trans->set_data_length(4);
					trans->set_streaming_width(4);
					trans->set_byte_enable_ptr(0);
					trans->set_dmi_allowed(false);
					trans->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

					cout << "TX: ProcElem flit --> " << flit << endl;
					init_socket->b_transport(*trans, delay); //blocking call

					credit_cnt_l--;
				}

			}

			wait(delay);
		}
	}

	virtual void store_in_memory(tlm::tlm_generic_payload& trans,
			sc_time& time) {
		unsigned int in_flit;
		// parse the transaction
		unsigned char* ptr = trans.get_data_ptr();
		unsigned int len = trans.get_data_length();
		unsigned char* byt = trans.get_byte_enable_ptr();
		unsigned int wid = trans.get_streaming_width();

		memcpy(&in_flit, ptr, len);
		cout << "RX: ProcElem flit #" << in_flit_counter++ << " <-- " << in_flit
				<< endl;

		my_pkt_sink.putFlit(in_flit);

		trans.set_response_status(tlm::TLM_OK_RESPONSE);
	}

	virtual void update_credit_cnt(tlm::tlm_generic_payload& trans,
			sc_time& time) {

		if (credit_cnt_l < 3) {
			cout << "Increase Credit Counter of PE0" << endl;
			credit_cnt_l++;
		}

		trans.set_response_status(tlm::TLM_OK_RESPONSE);
	}

	virtual void stop(tlm::tlm_generic_payload& trans, sc_time& time) {
		run = false;

		trans.set_response_status(tlm::TLM_OK_RESPONSE);
	}
};
