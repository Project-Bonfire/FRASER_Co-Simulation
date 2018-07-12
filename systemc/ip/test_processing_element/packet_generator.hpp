/*
 * This file implements a simple packet generator with
 * fixed traffic (counter, no random data).
 */

#ifndef __PACKET_GENERATOR_HPP__
#define __PACKET_GENERATOR_HPP__

#include <cstdint>
#include <memory>

#ifdef NO_SYSTEMC
    #include "router.hpp"
#endif

#include "packet.hpp"

enum class GenerationModes {counter}; // TODO: Add other modes

class PacketGenerator {
public:
#ifdef NO_SYSTEMC
    PacketGenerator(uint16_t address, std::shared_ptr<Router> router);
#else
    PacketGenerator(uint16_t address);
#endif

    void generate_packet(uint16_t packet_length, uint16_t destination,
                          GenerationModes mode);

    uint32_t get_packet_count() {
            return m_generated_packet->raw_data.size();
    }

    uint32_t get_packet_at(uint32_t n) {
            return m_generated_packet->raw_data.at(n);
    }

    void send_packet();

private:
    void counter_based_generation();

    uint16_t m_address;
    uint16_t m_checksum;
    uint16_t m_packet_id;
#ifdef NO_SYSTEMC
    std::shared_ptr<Router> m_router;
#endif
    std::shared_ptr<Packet> m_generated_packet;
};


#endif //__PACKET_GENERATOR_HPP__
