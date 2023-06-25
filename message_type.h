
/**
 * Leaf nodes will need to send several different types of
 * messages to the main node(s).
 */

#ifndef h_message_type_h
#define h_message_type_h

/** 
 * Message types the leaf node may send to the main node.
 */
enum MessageType {
    join_request = 1,
    join_response = 2,
    time_request = 3,
    time_response = 4,
    error = 5,

    // the main node only provides the ACK for these messages
    data_packet = 10,
    text = 11
};

/// Size of the join request in bytes
#define JOIN_REQUEST_SIZE sizeof(join_request_t)

/**
 * A leaf node send a request to join the main node. It includes
 * its EUI - a kind of UUID. The response from the main nide is a
 * join_response_t.
 */
struct join_request_t {
    MessageType type; // join_request
    uint64_t dev_eui; // read from the RS EUI chip
};

/// Size of the join response in bytes
#define JOIN_RESPONSE_SIZE sizeof(join_response_t)

/**
 * The main node responds with the byte node number this leaf node
 * should use in all subsequent messages (it maintains a table of 
 * EUIs to byte node numbers) and the time. The leaf node records the
 * byte node number and sets its time (so it will be synchronized
 * with the main node).
 */
struct join_response_t {
    uint8_t node;
    uint32_t time;
};

/// Size of the time request in bytes
#define TIME_REQUEST_SIZE sizeof(time_request_t)

/**
 * The leaf node asks the main node for the time.
 */
struct time_request_t {
    MessageType type;
};

/// Size of the time request in bytes
#define TIME_RESPONSE_SIZE sizeof(time_response_t)

/**
 * The time from the main node.
 */
struct time_response_t {
    uint32_t time;
}

// TODO add text message

#endif
