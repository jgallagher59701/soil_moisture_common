//
// Created by James Gallagher on 3/13/21.
//
// For each message type besides 'data_packet' define a build_, parse_
// and _to_string function.

// #include <Arduino.h>
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
        case data_message:
            return (char*)"data message";
        case text:
            return (char*)"text";

        default:
            return (char*)"unknown";
    }
}

/** 
 * @name Join Request 
 * @note Instead of using this, write a python program to handle initial configuration.
 */
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
        snprintf((char *)decoded_string, sizeof(decoded_string), "node: %u, time: %lu", node, time);
    } else {
        snprintf((char *)decoded_string, sizeof(decoded_string), "%u, %lu", node, time);
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
 * @return true if this is a time_request message, false otherwise.
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
 * @param node The node number of the responder
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
        snprintf((char *)decoded_string, sizeof(decoded_string), "node: %u, time: %lu", node, time);
    } else {
        snprintf((char *)decoded_string, sizeof(decoded_string), "%u, %lu", node, time);
    }

    return decoded_string;
}
///@}

/** @name Text message */
///@{

/**
 * @brief Build a Text message
 *
 * Either the main or leaf node can send a Text message.
 *
 * @param t The Text message
 * @param node The node number
 * @param length The number of bytes in buf 
 * @param buf The message characters
 */

void build_text_message(text_t *t, const uint8_t node, const uint8_t length, const uint8_t *buf /* TEXT_BUF_LEN */) {
    t->type = text;
    t->node = node;
    t->length = length;
    memcpy(t->buf, buf, (length < TEXT_BUF_LEN) ? length: TEXT_BUF_LEN);
}

bool parse_text_message(const text_t *data, uint8_t *node, uint8_t *length, uint8_t *buf) {
    if (data->type != text)
        return false;

    if (node)
        *node = data->node;
    if (length)
        *length = data->length;
    if (length && buf)
        memcpy(buf, data->buf, (*length < TEXT_BUF_LEN) ? *length: TEXT_BUF_LEN);

    return true;
}

char *text_message_to_string(const text_t *t, bool pretty /*false*/) {
    uint8_t node;
    uint8_t length;
    uint8_t msg[TEXT_BUF_LEN+1];

    parse_text_message(t, &node, &length, msg);

    static char decoded_string[TEXT_BUF_LEN + 20];
    if (pretty) {
        snprintf((char *)decoded_string, sizeof(decoded_string), "node: %u, message: %s", node, t->buf);
    } else {
        snprintf((char *)decoded_string, sizeof(decoded_string), "%u, %s", node, t->buf);
    }

    return decoded_string;
}

/**
 * @brief Encode information for transmission
 * Information is stored in the data buffer. The data packets uses 12 bytes.
 * @param data Pointer to a buffer
 * @param node Unsigned Byte the holds the node number
 * @param message Unsigned Long that holds the message number
 * @param time Unsigned Long that holds the epoch time (seconds since 1/1/1970)
 * @param battery Unsigned Short with battery voltage * 100
 * @param last_tx_duration Unsigned Short ms duration of the previous transmission
 * @param temp Short with temperature in degrees C * 100
 * @param humidity Unsigned Short with percent relative humidity * 100
 * @param status Unsigned Byte with the leaf node status.
 * @return The number of bytes in the packet.
 */
void build_data_message(data_message_t *data, const uint8_t node, const uint32_t message,
                       const uint32_t time, const uint16_t battery, const uint16_t last_tx_duration,
                       const int16_t temp, const uint16_t humidity, const uint8_t status) {
    data->type = data_message;
    data->node = node;
    data->message = message;
    data->time = time;
    data->battery = battery;
    data->last_tx_duration = last_tx_duration;
    data->temp = temp;
    data->humidity = humidity;
    data->status = status;
}

/**
 * @brief Unpack a data packet
 * @param data Pointer to a DATA_PACKET_SIZE byte data packet
 * @param node Value-result parameter for the node number. if NULL, no value is extracted
 * @param message if not NULL, V-R parameter for the message number
 * @param time If not NULL, V-R parameter for epoch time.
 * @param battery If not NULL, V-R parameter for battery voltage *100
 * @param last_tx_duration if not NULL, V-R parameter for the last tx duration in ms
 * @param temp If not NULL, V-R parameter for temperature om C * 100
 * @param humidity If not NULL, V-R parameter for percent rel. humidity * 100
 * @param status If not NULL, V-R parameter for status info
 */
bool parse_data_message(const data_message_t *data, uint8_t *node, uint32_t *message, uint32_t *time,
                       uint16_t *battery, uint16_t *last_tx_duration, int16_t *temp, uint16_t *humidity, uint8_t *status) {
    if (data->type != data_message)
        return false;

    if (node)
        *node = data->node;

    if (message)
        *message = data->message;

    if (time)
        *time = data->time;

    if (battery)
        *battery = data->battery;

    if (last_tx_duration)
        *last_tx_duration = data->last_tx_duration;

    if (temp)
        *temp = data->temp;

    if (humidity)
        *humidity = data->humidity;

    if (status)
        *status = data->status;

    return true;
}

/**
 * @brief print the data packet to a string
 *
 * Print the data in packet to a string. The returned pointer is to
 * static storage and will be changed by subsequent calls to this function.
 * If the optional parameter \c pretty is true, add info for a human.
 *
 * @param data The data packet
 * @param pretty Optional, if true, print names, units, etc. Default: false
 * @return Pointer to the string in static storage. Always null terminated.
 */
char *data_message_to_string(const data_message_t *data, bool pretty /* false */) {
    uint8_t node;
    uint32_t time;
    uint32_t message;
    uint16_t battery;
    uint16_t last_tx_duration;
    int16_t temp;
    uint16_t humidity;
    uint8_t status;

    parse_data_message(data, &node, &message, &time, &battery, &last_tx_duration, &temp, &humidity, &status);

    static char decoded_string[256];
    if (pretty) {
        // string length 62 characters + 2 bytes (6 chars) + 2 Longs (20) + 3 Shorts (15)
        // = 62 + 41 = 103
        snprintf((char *)decoded_string, sizeof(decoded_string),
                 "node: %u, message: %lu, time: %lu, Vbat %u v, Tx dur %u ms, T: %d C, RH: %u %%, status: 0x%02x",
                 node, message, time, battery, last_tx_duration, temp, humidity, (unsigned int)status);
    } else {
        snprintf((char *)decoded_string, sizeof(decoded_string),
                 "%u, %lu, %lu, %u, %u, %d, %u, 0x%02x",
                 node, message, time, battery, last_tx_duration, temp, humidity, (unsigned int)status);
    }

    return decoded_string;
}
