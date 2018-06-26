/*
 * A very simple packet generator. Just for basic testing
 * of the packet generation and sink
 */

#include <memory>
#include "packet_generator.hpp"
#include "packet_sink.hpp"
#include "router.hpp"

int main(){

	/* Build a "network" */
	std::shared_ptr<PacketSink> packet_sink = std::make_shared<PacketSink>(1);
	std::shared_ptr<Router> router = std::make_shared<Router>(packet_sink);
	std::shared_ptr<PacketGenerator> packet_generator = std::make_shared<PacketGenerator>(2, router);

	/* Generate and send a packet */
	packet_generator -> generate_packet(10, 1, GenerationModes::counter);
	packet_generator -> send_packet();

	return 0;
}
