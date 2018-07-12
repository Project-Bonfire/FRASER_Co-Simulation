#ifndef __PACKET_HPP__
#define __PACKET_HPP__

#include <vector>

struct Packet {
    uint16_t address;
    uint16_t src_addr;
    uint16_t dst_addr;
    uint16_t packet_length;
    uint16_t packet_id;
    uint16_t crc;

    std::vector<uint32_t> raw_data;
};

#endif //__PACKET_HPP__
