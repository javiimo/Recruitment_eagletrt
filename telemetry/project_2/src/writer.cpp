#include "../include/writer.h"
#include "../include/parser.h"
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <string>
#include <cstring>
#include <deque>
#include <vector>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <filesystem>


void write_last_vector_to_file(const std::deque<std::vector<TMessage>>& data_stored) {
    // Access the last vector in the deque
    const std::vector<TMessage>& last_vector = data_stored.back();

    // Check and create the 'Generated' directory if it doesn't exist
    std::filesystem::path directory_path = "Generated";
    if (!std::filesystem::exists(directory_path)) {
        std::filesystem::create_directory(directory_path);
    }

    // Generate the file path including the 'Generated' folder
    std::time_t first_timestamp = last_vector.front().timestamp;
    std::stringstream file_path_stream;
    file_path_stream << directory_path.string() << "/Session" << first_timestamp << ".log";
    std::string file_path = file_path_stream.str();

    // Open a file with the generated path for writing
    std::ofstream out_file(file_path);
    if (!out_file.is_open()) {
        std::cout << "Could not open the file in " << file_path << std::endl;
        return;
    }

    // Iterate through the last vector and write each message
    for (const TMessage& tmessage : last_vector) {
        out_file << "(" << tmessage.timestamp << ") " << tmessage.message.toString() << std::endl;
    }

    // Close the file
    out_file.close();

    return;
}


void writer(std::deque<std::vector<TMessage>>& data_stored, std::atomic<bool>& writing){
    std::cout << "Writer started" << std::endl;
    while(writing.load()){
        std::cout << "Writer waits" << std::endl;
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, []{ return dataAvailable; });
        write_last_vector_to_file(data_stored);
        dataAvailable = false;
    }
    std::cout << "Writer finished" << std::endl;
    return;
}