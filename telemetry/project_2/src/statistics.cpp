#include "../include/parser.h"
#include "../include/statistics.h"
#include <vector>
#include <set>
#include <map>
#include <cstdint>
#include <string>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <filesystem>


// Use a set to get the different types of ID that are there
std::set<std::uint16_t> find_unique_ids(const std::vector<TMessage>& messages) {
    std::set<std::uint16_t> unique_ids;
    for (const auto& message : messages) {
        unique_ids.insert(message.message.id);
    }
    return unique_ids;
}


// To compute the mean time we need the total time between the first and last, so we map the IDs to the info about them we need
std::map<std::uint16_t, IDInfo> count_messages_and_time_diff(const std::vector<TMessage>& messages) {
    std::map<std::uint16_t, IDInfo> id_info_map;

    for (const auto& message : messages) {
        auto& info = id_info_map[message.message.id];

        if (info.count == 0) {
            // First occurrence of this ID
            info.first_timestamp = message.timestamp;
        } else {
            // Update last timestamp for this ID
            info.last_timestamp = message.timestamp;
        }

        info.count++;
    }

    for (auto& pair : id_info_map) {
        auto& info = pair.second;
        // Calculate the difference if there are multiple messages with this ID
        if (info.count > 1) {
            info.last_timestamp -= info.first_timestamp;
            info.last_timestamp= (1000 * info.last_timestamp) / info.count;
        } else {
            // Only one message, so print the total time of the session in miliseconds
            info.last_timestamp = (messages.back().timestamp - messages.front().timestamp) * 1000;
        }
    }

    return id_info_map;
}


void write_to_csv(const std::vector<TMessage>& messages) {

    // Generate the file path in the 'Generated' folder
    std::time_t first_timestamp = messages.front().timestamp;
    std::stringstream file_path_stream;
    file_path_stream << "Generated/Statistics" << first_timestamp << ".csv";
    std::string file_path = file_path_stream.str();

    // Use the previously defined functions
    auto id_infos = count_messages_and_time_diff(messages);

    // Open a file stream
    std::ofstream file(file_path);

    // Check if the file is open
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << file_path << std::endl;
        return;
    }

    // Write the header
    file << "ID,number_of_messages,mean_time\n";

    // Write data
    for (const auto& pair : id_infos) {
        std::stringstream ss;
        ss << std::uppercase << std::hex << std::setfill('0') << std::setw(3) << pair.first;
        file << ss.str() << "," 
             << pair.second.count << "," 
             << pair.second.last_timestamp << "\n";
    }

    // Close the file
    file.close();
}


