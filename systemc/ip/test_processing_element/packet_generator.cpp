/*
 * This file implements a simple packet generator with
 * fixed traffic (counter, no random data).
 */

#include <iostream>
#include <string>
#include <sstream>
#include <bitset>
#include <cstdint>
#include <memory>
#include <boost/crc.hpp>
#include "packet_generator.hpp"
#include "flit_utils.hpp"
#include "router.hpp"

/*
 * Constructor
 *
 * Parameters:
 * 	uint16_t address               - Address of the current packet generator
 * 	std::shared_ptr<Router> router - Pointer to the router (input port) instance
 * 					                 the packet generator is connected to
 */
PacketGenerator::PacketGenerator(uint16_t address, std::shared_ptr<Router> router) {
	m_address = address;
	m_packet_id = 0;
	m_router = router;
	m_checksum = 0;
}

/*
 * Generates a packet by increasing the value stored in each flit by 1, starting from 0.
 *
 * Parameters:
 * 	std::shared_ptr<std::vector<uint32_t>> packet - A vector used for storing the packet flit-by-flit
 * 	uint16_t packet_length                        - Length of the packet in flits
 * 	uint16_t destination                          - Destination address for the packet
 */
void PacketGenerator::counter_based_generation() {
	boost::crc_ccitt_type result;

	/* Build headers */
	m_generated_packet->raw_data.push_back(make_header_flit(m_generated_packet->dst_addr, m_generated_packet->src_addr));
	m_generated_packet->raw_data.push_back(make_first_body_flit(m_generated_packet->packet_length, m_packet_id));

	/* Make data flits */
	for (int i=2; i < m_generated_packet->packet_length - 1; i++){
		m_generated_packet->raw_data.push_back(make_body_flit(i));
	}
	/* Calculate result of the packet (ignoring the tail flit) */
    result.process_bytes(&m_generated_packet->raw_data.at(0), m_generated_packet->raw_data.size()); // TODO: include tail flit

	m_checksum = result.checksum();

	/* Store checksum in the tail */
	m_generated_packet->raw_data.push_back(make_tail_flit(m_checksum));
}


/*
 * A simple packet generator. Currently it only implements counter-based data
 * generation, no random data support.
 *
 * Parameters:
 * 	uint16_t packet_length - Length of the data to be generated
 * 							 (in packets, including headers and tail)
 * 	uint16_t destination   - Destination address of the packet
 * 	GenerationModes mode   - Mode to use for data generation
 *
 * Returns:
 * 	Generated packet
 */
void PacketGenerator::generate_packet(uint16_t packet_length, uint16_t destination,
									  GenerationModes mode) {

	/* Make a new packet struct */
	m_generated_packet = std::make_shared<Packet>();

	m_generated_packet -> packet_length = packet_length;
	m_generated_packet -> dst_addr = destination;
	m_generated_packet -> src_addr = m_address;

	/* Packet generation */
	// TODO: Add more modes (like random)
	switch (mode) {
		case GenerationModes::counter:
			counter_based_generation();
			break;

		default:
			std::cerr << "Unknown data generation mode!" << std::endl;
			return;
	}
}

/*
 * Sends a packet
 *
 * Parameters:
 * 	std::shared_ptr<std::vector<uint32_t>> packet - Packet to be sent. Destination is
 * 													automatically read from the packet
 */

void PacketGenerator::send_packet(){
	std::stringstream log_stream;

	/* Send the packet */
	for (size_t i=0; i < m_generated_packet->raw_data.size(); i++) {
		m_router->send_flit_to_router(m_generated_packet->raw_data.at(i));
	}


	/* Logging */
	log_stream << "Sent_" << m_generated_packet->src_addr << " - "
		<< "ID: " << m_packet_id
		<< ", Dst: " << m_generated_packet->dst_addr
		<< ", Length: " << m_generated_packet->packet_length
		<< ", CRC: 0x" << std::hex << m_checksum
		<< std::dec << ", time: " << "N/A"  // TODO: Add time
		<< std::endl;

	// TODO: Better logging than just printing on the screen??
    std::string log_line  = log_stream.str();
	std::cout << log_line;

	/* Packet ID will increase after every sent packet */
	m_packet_id++;
}
