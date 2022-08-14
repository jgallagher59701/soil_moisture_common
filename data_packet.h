// Header for the data_packet functions

#ifndef h_data_packet_h
#define h_data_packet_h

#include <Arduino.h>

/// Size of the data packet in bytes
#define DATA_PACKET_SIZE sizeof(packet_t)   // 19 bytes

struct packet_t {
    uint32_t message;
    uint32_t time;
    uint16_t battery;
    uint16_t last_tx_duration;
    int16_t temp;
    uint16_t humidity;
    uint8_t status;
    uint8_t data;
    uint8_t node;
};

void build_data_packet(packet_t *data, const uint8_t node, const uint32_t message, const uint32_t time,
                       const uint16_t battery, const uint16_t last_tx_duration,
                       const int16_t temp, const uint16_t humidity, const uint8_t status);

void parse_data_packet(const packet_t *data, uint8_t *node, uint32_t *message, uint32_t *time, uint16_t *battery,
                       uint16_t *last_tx_duration, int16_t *temp, uint16_t *humidity, uint8_t *status);

char *data_packet_to_string(const packet_t *data, bool pretty = false);

#if 0   /// Not yet... jhrg 2/5/22
#define DATA_PACKET_2_ SIZE sizeof(packet_2_t)  // 11 bytes

struct packet_2_t {
    uint16_t message;       // 65536 message maximum
    uint32_t time;
    int16_t temp;
    uint8_t humidity;
    uint8_t battery;        // really only needs 4 bits; y = 0.1x + 2.7; 0 <= x <= 10
    uint8_t status;         // only using 4 bits
    // uint8_t node; Not needed since the code uses 'reliable datagrams' and that includes the node number
};

void build_data_packet2(packet_2_t *data, const uint16_t message, const uint32_t time,
                        const uint8_t battery, const int16_t temp, const uint8_t humidity, 
                        const uint8_t status);

void parse_data_packet2(const packet_2_t *data, uint16_t *message, uint32_t *time,
                        uint8_t *battery, int16_t *temp, uint8_t *humidity, 
                        uint8_t *status);

char *data_packet_to_string2(const packet_2_t *data, bool pretty = false);
#endif

#endif