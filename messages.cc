//
// Created by James Gallagher on 3/13/21.
//
// For each message type besides 'data_packet' define a build_, parse_
// and _to_string function.

#include <Arduino.h>
#include <messages.h>

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
            return (char*)"join request";
        case join_response:
            return (char*)"join response";
        case time_request:
            return (char*)"time request";
        case time_response:
            return (char*)"time response";
#if 0
        case error:
            return (char*)"error";
#endif
        case data_packet:
            return (char*)"data packet";
        case text:
            return (char*)"text";

        default:
            return (char*)"unknown";
    }
}

/** @name Join Request */
///@{
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
        snprintf((char *)decoded_string, sizeof(decoded_string), "type: %s, device EUI: 0x%16llx",
                 get_message_type_string(get_message_type((void*)jr)), dev_eui);
    } else {
        snprintf((char *)decoded_string, sizeof(decoded_string), "%s, 0x%16llx",
                 get_message_type_string(get_message_type((void*)jr)), dev_eui);
    }

    return decoded_string;
}
///@}

/** @name Join response */
///@{

/**
 * @brief Build a Join Response message
 *
 * The main node responds to a Join Request by assigning a node number (1-254)
 * to the requesting node and returning that node number and the current time.
 * The requester updates its node number and time.
 *
 * @param jr The join Response message
 * @param node The node number
 * @param time The time
 */
void build_join_response(join_response_t *jr, uint8_t node, uint32_t time) {
    jr->type = join_response;
    jr->node = node;
    jr->time = time;
}


bool parse_join_response(const join_response_t *data, uint8_t *node, uint32_t *time) {
    if (data->type != join_request)
        return false;

    if (node)
        *node = data->node;
    if (time)
        *time = data->time;

    return true;
}

char *join_response_to_string(const join_response_t *jr, bool pretty /*false*/) {
    uint8_t node;
    uint32_t time;

    parse_join_response(jr, &node, &time);

    static char decoded_string[64];
    if (pretty) {
        snprintf((char *)decoded_string, sizeof(decoded_string), "node: %u, time: %ul", node, time);
    } else {
        snprintf((char *)decoded_string, sizeof(decoded_string), "%u, %ul", node, time);
    }

    return decoded_string;
}
///@}

/** @name Time Request */
///@{
/**
 * @brief Build a time_request message
 * @param tr Pointer to time_request_t structure
 * @param node The node making the request
 */
void build_time_request(time_request_t *tr, uint8_t node) {
    tr->type = time_request;
    tr->node = node;
}

/**
 * @brief extract information from a time_request message
 * @param node If not null, returns the node number of the requesting leaf node
 * @return true is this is a time_request message, false otherwise.
 */
bool parse_time_request(const time_request_t *data, uint8_t *node) {
    if (data->type != time_request)
        return false;

    if (node)
        *node = data->node;

    return true;
}

/**
 * @brief Get a string representation for a time request message

 * @param tr A pointer to the time request message
 * @param pretty True == print a verbose version, false == just the field values
 * @return The string representation, a pointer to static memory. Overwritten
 * on subsequent calls.
 */
char *time_request_to_string(const time_request_t *tr, bool pretty /*false*/) {
    uint8_t node;

    parse_time_request(tr, &node);

    static char decoded_string[64];
    if (pretty) {
        snprintf((char *)decoded_string, sizeof(decoded_string), "type: %s, Node: %d",
                 get_message_type_string(get_message_type((void*)tr)), node);
    } else {
        snprintf((char *)decoded_string, sizeof(decoded_string), "%s, %d",
                 get_message_type_string(get_message_type((void*)tr)), node);
    }

    return decoded_string;
}
///@}

/** @name Time response */
///@{

/**
 * @brief Build a Time Response message
 *
 * The main node responds to a Time Request by the time from its RTC.
 *
 * @param tr The Time Response message
 * @param node The node number
 * @param time The time
 */
void build_time_response(time_response_t *jr, uint8_t node, uint32_t time) {
    jr->type = time_response;
    jr->node = node;
    jr->time = time;
}

bool parse_time_response(const time_response_t *data, uint8_t *node, uint32_t *time) {
    if (data->type != time_response)
        return false;

    if (node)
        *node = data->node;
    if (time)
        *time = data->time;

    return true;
}

char *time_response_to_string(const time_response_t *tr, bool pretty /*false*/) {
    uint8_t node;
    uint32_t time;

    parse_time_response(tr, &node, &time);

    static char decoded_string[64];
    if (pretty) {
        snprintf((char *)decoded_string, sizeof(decoded_string), "node: %u, time: %ul", node, time);
    } else {
        snprintf((char *)decoded_string, sizeof(decoded_string), "%u, %ul", node, time);
    }

    return decoded_string;
}
///@}
