/* A dummy router file for testing the packet generators
 * It forwards each received flit to the packet sink.
 */

#ifndef __ROUTER_HPP__
#define __ROUTER_HPP__

#include <memory>
#include "packet_sink.hpp"

class Router {
public:
    Router(std::shared_ptr<PacketSink> packet_sink);
    void send_flit_to_router(uint32_t flit);

private:
    std::shared_ptr<PacketSink> m_packet_sink;
};

#endif //__ROUTER_HPP__
