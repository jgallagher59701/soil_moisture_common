
/**
 * Leaf nodes will need to send several different types of
 * messages to the main node(s).
 */

#ifndef h_message_type_h
#define h_message_type_h

#include <cstdint>
#include <cstdio>
#include <cstring>

#ifndef RH_RF95_MAX_MESSAGE_LEN
#define RH_RF95_MAX_MESSAGE_LEN 251
#endif

/** 
 * Message types the leaf node may send to the main node.
 */
enum MessageType {
    join_request = 1,
    join_response = 2,
    time_request = 3,
    time_response = 4,
    // error = 5,

    // the main node only provides the ACK for these messages
    data_message = 10,
    text = 11,

    // The original data packet
    data_packet = 12
};

/// Size of the join request in bytes
#define JOIN_REQUEST_SIZE sizeof(join_request_t)

/**
 * A leaf node send a request to join the main node. It includes
 * its EUI - a kind of UUID. The response from the main nide is a
 * join_response_t.
 *
 * @note there is no node number in this request because this is
 * the request a new node makes to get that 8-bit number bound to
 * its 64-bit EUI.
 */
struct join_request_t {
    MessageType type; // join_request
    uint64_t dev_eui; // read from the RS EUI chip
};

/// Size of the join response in bytes
#define JOIN_RESPONSE_SIZE sizeof(join_response_t)

/**
 * The main node responds with the byte node number this leaf node
 * should use in all subsequent messages and the time. The main node
 * maintains a table mapping EUIs to leaf_ node numbers.  The leaf
 * node records the node number and sets its time (so it will be
 * synchronized with the main node).
 *
 * @todo Add EUI to the response
 */
struct join_response_t {
    MessageType type;
    uint8_t node;       // From
    uint8_t leaf_node;  // TO
    uint32_t time;
};

/// Size of the time request in bytes
#define TIME_REQUEST_SIZE sizeof(time_request_t)

/**
 * The leaf node asks the main node for the time.
 */
struct time_request_t {
    MessageType type;
    uint8_t node; // From
};

/// Size of the time request in bytes
#define TIME_RESPONSE_SIZE sizeof(time_response_t)

/**
 * The time from the main node.
 */
struct time_response_t {
    MessageType type;
    uint8_t node;       // From
    uint32_t time;      // Unix time
};

#define TEXT_BUF_LEN (RH_RF95_MAX_MESSAGE_LEN - sizeof(MessageType) - sizeof(uint8_t) - sizeof(uint8_t))

struct text_t {
    MessageType type;
    uint32_t node;      // From
    uint8_t length;     // Number of chars in buf
    uint8_t buf[TEXT_BUF_LEN];
};

MessageType get_message_type(void *message);
char *get_message_type_string(MessageType type);

void build_join_request(join_request_t *jr, uint64_t dev_eui);
bool parse_join_request(const join_request_t *data, uint64_t *dev_eui);
char *join_request_to_string(const join_request_t *jr, bool pretty = false);

char *join_response_to_string(const join_response_t *jr, bool pretty = false);
bool parse_join_response(const join_response_t *data, uint8_t *node, uint32_t *time);
void build_join_response(join_response_t *jr, uint8_t node, uint32_t time);

char *time_request_to_string(const time_request_t *tr, bool pretty /*false*/);
bool parse_time_request(const time_request_t *data, uint8_t *node);
void build_time_request(time_request_t *tr, uint8_t node);

char *time_response_to_string(const time_response_t *tr, bool pretty /*false*/);
bool parse_time_response(const time_response_t *data, uint8_t *node, uint32_t *time);
void build_time_response(time_response_t *jr, uint8_t node, uint32_t time);

char *text_message_to_string(const text_t *t, bool pretty /*false*/);
bool parse_text_message(const text_t *data, uint8_t *node, uint8_t *length, uint8_t **buf);
void build_text_message(text_t *t, const uint8_t node, const uint8_t length, const uint8_t *buf /* TEXT_BUF_LEN */);

struct data_message_t {
    MessageType type;
    uint8_t node;               // Node number
    uint32_t message;           // message number
    uint32_t time;              // Unix time
    uint16_t battery;           // battery voltage * 100
    uint16_t last_tx_duration;  // duration in ms
    int16_t temp;               // Temperature in C * 100
    uint16_t humidity;          // % relative humidity * 100
    uint8_t status;             // Sensor status code
};

#define DATA_MESSAGE_SIZE sizeof(data_message_t)

void build_data_message(data_message_t *data, const uint8_t node, const uint32_t message, const uint32_t time,
                       const uint16_t battery, const uint16_t last_tx_duration,
                       const int16_t temp, const uint16_t humidity, const uint8_t status);

bool parse_data_message(const data_message_t *data, uint8_t *node, uint32_t *message, uint32_t *time, uint16_t *battery,
                       uint16_t *last_tx_duration, int16_t *temp, uint16_t *humidity, uint8_t *status);

char *data_message_to_string(const data_message_t *data, bool pretty = false);

#endif
