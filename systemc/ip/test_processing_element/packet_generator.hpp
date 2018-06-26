/*
 * This file implements a simple packet generator with
 * fixed traffic (counter, no random data).
 */

#ifndef __PACKET_GENERATOR_HPP__
#define __PACKET_GENERATOR_HPP__

#include <cstdint>
#include <memory>
#include "router.hpp"
#include "packet.hpp"

enum class GenerationModes {counter}; // TODO: Add other modes

class PacketGenerator {
public:
    PacketGenerator(uint16_t address, std::shared_ptr<Router> router);
    void generate_packet(uint16_t packet_length, uint16_t destination,
                          GenerationModes mode);

    void send_packet();

private:
    void counter_based_generation();

    uint16_t m_address;
    uint16_t m_checksum;
    uint16_t m_packet_id;
    std::shared_ptr<Router> m_router;

    std::shared_ptr<Packet> m_generated_packet;
};


#endif //__PACKET_GENERATOR_HPP__
