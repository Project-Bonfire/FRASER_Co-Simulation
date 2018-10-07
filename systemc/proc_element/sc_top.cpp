#include "systemc"
using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include "proc_elem.h"

SC_MODULE(Top)
{
    sc_proc_elem *my_proc_elem;

    SC_CTOR(Top)
    {
        // instance of processing-element
        my_proc_elem = new sc_proc_elem   ("my_proc_elem");
        // loopback connection, target-port to init-port
        my_proc_elem->init_socket.bind( my_proc_elem->tgt_socket );
    }
};

int sc_main(int argc, char* argv[])
{
    cout << endl << "Hello world from SystemC !" << endl;

    Top top("top");
    sc_start();

    return 0;
}

