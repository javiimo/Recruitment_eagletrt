#pragma once

#include <cstdint>
#include <string>
#include <chrono>
#include <sstream>
#include <cstdlib>
#include <algorithm>



/**
 * @brief Message is a struct to contain the two components of each message char array: ID and Payload 
 * 
 * @param message the pointer to the char array
 * @param length the bytes in the char array
 * @return the toString() gives the string directly of the message received, quite useful for writing the log. With the format desired (Converts the uint16)
 */
struct Message {
    std::uint16_t id;          // 12-bit ID, using uint16_t 
    std::string payload;       // Payload as a string of hexadecimal characters

    // Constructor to parse a message from a char array
    Message(const char* message, int length) {
        // Find the '#' character in the char array
        const char* hash_pos = std::find(message, message + length, '#');

        if (hash_pos != message + length) {
            // Calculate the position of the '#' character
            size_t pos = std::distance(message, hash_pos);

            // Parse the ID as a hexadecimal string and convert to uint16_t
            std::string idStr(message, message + pos);
            id = static_cast<std::uint16_t>(std::strtol(idStr.c_str(), nullptr, 16));

            // Extract the payload
            payload = std::string(hash_pos + 1, message + length);
        } else {
            // Default invalid message format
            id = 0; 
            payload = "Wrong Format";
        }
        
    }

    // Function to convert the message to a string
    std::string toString() const {
    std::stringstream ss;
    ss << std::uppercase << std::hex << std::setfill('0') << std::setw(3) << id;
    return ss.str() + '#' + payload;
}

};

/**
 * @brief just an extension of the message to associate a timestamp as well to each message
 * 
 * @param _message Message object
 * @param _timestamp A timestamp of dtype time_t
 */
struct TMessage {
    Message message;          
    std::time_t timestamp;     

    TMessage(const Message& _message, const std::time_t& _timestamp)
        : message(_message), timestamp(_timestamp) {}
};
