#define SC_INCLUDE_DYNAMIC_PROCESSES

#include "systemc"
using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"

struct Target: sc_module
{
    tlm_utils::simple_target_socket<Target> socket;
    int mem[1000];
    SC_CTOR(Target) : socket("socket")
    {
        socket.register_b_transport(this, &Target::b_transport);
    };

    virtual void b_transport( tlm::tlm_generic_payload& trans, sc_time& delay )
    {
        cout << endl << "In Target::b_transport" << endl;
        tlm::tlm_command cmd = trans.get_command();
        sc_dt::uint64    adr = trans.get_address() / 4;
        unsigned char*   ptr = trans.get_data_ptr();
        unsigned int     len = trans.get_data_length();
        unsigned char*   byt = trans.get_byte_enable_ptr();
        unsigned int     wid = trans.get_streaming_width();

        cout << "*ptr 0x" << char(*ptr) << endl;

        cout << reinterpret_cast<unsigned char>(*ptr) ;

        if ( cmd == tlm::TLM_READ_COMMAND )
            memcpy(ptr, &mem[adr], len);
        else if ( cmd == tlm::TLM_WRITE_COMMAND )
            memcpy(&mem[adr], ptr, len);

        trans.set_response_status( tlm::TLM_OK_RESPONSE );
    }
};

