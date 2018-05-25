/* A dummy router file for testing the packet generators
 * It forwards each received flit to the packet sink.
 */

#include <memory>
#include "router.hpp"
#include "packet_sink.hpp"

/*
 * Constructor.
 *
 * Parameters:
 * std::shared_ptr<PacketSink> packet_sink - packet sink which is connected to the router
 */
Router::Router(std::shared_ptr<PacketSink> packet_sink) {
    m_packet_sink = packet_sink;
}

/*
 * A dummy flit receiver.
 * Just forwars received flits to packet sink.
 * Meant to be called from the packet gen.
 *
 * Parameters:
 * uint32_t flit - flit to recieve
 */

void Router::send_flit_to_router(uint32_t flit){
    m_packet_sink -> send_flit_to_local(flit);
}
