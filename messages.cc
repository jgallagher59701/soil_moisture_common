//
// Created by James Gallagher on 3/13/21.
//
// For each message type besides 'data_packet' define a build_, parse_
// and _to_string function.

#include <Arduino.h>
#include <message_type.h>

/**
 * @brief Get MessageType field of any of the messages
 *
 * This assumes \c message is really a buffer with one of the messages
 * in it and that the type field is first. If this is used on a data
 * packet, the results are undefined.
 *
 * @param message A pointer to the message
 * @return the Message Type.
 */
MessageType get_message_type(void *message) {
    MessageType type;
    memcpy(&type, message, sizeof(MessageType));
    return type;
}

/**
 * @brief Get the name for a MessageType
 */
char *get_message_type_string(MessageType type) {
    switch (type) {
        case join_request:
            return "join request";
        case join_response:
            return "join response";
        case time_request:
            return "time request";
        case time_response:
            return "time response";
        case error:
            return "error";
        case data_packet:
            return "data packet";
        case text:
            return "text";

        default:
            return "unknown";
    }
}

/**
 * @brief Build a join_request message
 * @param jr Pointer to join_request_t structure
 * @param dev_eui The EUI for this leaf node
 */
void build_join_request(join_request_t *jr, uint64_t dev_eui) {
    jr->type = join_request;
    jr->dev_eui = dev_eui;
}

/**
 * @brief extract information from a join_request message
 * @param dev_eui If not null, returns the deveice EUI of the requesting leaf node
 * @return true is this is a join_request message, false otehrwise.
 */
bool parse_join_request(const join_request_t *data, uint64_t *dev_eui) {
    if (data->type != join_request)
        return false;

    if (dev_eui)
        *dev_eui = data->dev_eui;

    return true;
}

/**
 * @brief Get a string representation for a join request message

 * @param jr A pointer to the join request message
 * @param pretty True == print a verbose version, false == just the field values
 * @return The string representation, a pointer to static memory. Overwritten
 * on subsequent calls.
 */
char *join_request_to_string(const join_request_t *jr, bool pretty /*false*/) {
    uint64_t dev_eui;

    parse_join_request(jr, &dev_eui);

    static char decoded_string[64];
    if (pretty) {
        // string length 62 characters + 2 bytes (6 chars) + 2 Longs (20) + 3 Shorts (15)
        // = 62 + 41 = 103
        snprintf((char *)decoded_string, sizeof(decoded_string), "type: %s, device EUI: 0x%16x",
                 get_message_type_string(get_message_type((void*)jr)), dev_eui);
    } else {
        snprintf((char *)decoded_string, sizeof(decoded_string), "%s, 0x%16x",
                 get_message_type_string(get_message_type((void*)jr)), dev_eui);
    }

    return decoded_string;
}