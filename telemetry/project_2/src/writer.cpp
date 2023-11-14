#include "../include/writer.h"
#include "../include/reader.h"
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

    // Generate the file path in the 'Generated' folder
    std::time_t first_timestamp = last_vector.front().timestamp;
    std::stringstream file_path_stream;
    file_path_stream << "Generated/Session" << first_timestamp << ".log";
    std::string file_path = file_path_stream.str();

    // Open a file for writing
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
    while(true){
        {//Release the lock inmediately after waking up
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [&]{ return dataAvailablew || !writing.load(); });
        }
        if (!dataAvailablew) break; // Exit the loop it there is anything to write
        write_last_vector_to_file(data_stored);
        dataAvailablew = false;
        if (!writing.load()) break; // Exit loop if writing is done
    }
    return;
}