#include "../include/fake_receiver.h"
#include "../include/parser.h"
#include "../include/reader.h"
#include <iostream>
#include <string>
#include <cstring>
#include <deque>
#include <vector>
#include <mutex>
#include <atomic>
#include <chrono>



void reader(std::deque<std::vector<TMessage>>& data_stored, const char* filepath,std::atomic<bool>& stop_flag, std::atomic<bool>& writing, std::mutex& gLock){
    // Declare message, vector of message session, open the file for CAN and set vector as non-written
    char message[MAX_CAN_MESSAGE_SIZE];
    std::vector<TMessage> message_session;
    int open_worked=open_can(filepath);
    if(open_worked==0){
        std::cout << "Can oppened correctly" << std::endl;
    }else{
        std::cout << "Error opening the can" << std::endl;
        return;
    }
    //Until sent the stop flag, we keep reading.
    while(!stop_flag.load()){
        
        int bytes_received = can_receive(message);
        if (bytes_received > 0) {
            // Check if it is a Start message
            if(std::string(message) == "0A0#6601" || std::string(message) == "0A0#FF01"){
                std::cout << "RUNNING!" << std::endl;
                if (!message_session.empty()){
                    gLock.lock();
                    data_stored.push_back(message_session);
                    gLock.unlock();
                    message_session.clear();
                    std::cout << "Emptied vector and added" << std::endl;
                    //!Call the writer thread!
                    }
            }

            //Check if it is a Stop message
            if(std::string(message) == "0A0#66FF"){
                std::cout << "IDLE!" << std::endl;
                if (!message_session.empty()){
                    gLock.lock();
                    data_stored.push_back(message_session);
                    gLock.unlock();
                    message_session.clear();
                    std::cout << "Emptied vector and added" << std::endl;
                    }
            }

            // Get the timestamp in Unix time
            auto time_point = std::chrono::system_clock::now();
            std::time_t mtimestamp = std::chrono::system_clock::to_time_t(time_point);
            //Parse the message (Create a Message object)
            Message parsed_message(message, bytes_received);
            //Create a TMessage object and add it to the vector
            TMessage tMessage(parsed_message, mtimestamp);
            message_session.push_back(tMessage);
            std::cout << "Pushed message" << std::endl;

            // Reset the message array to receive the next message 
            memset(message, 0, sizeof(message)); 
        } else {
            std::cout << "RECEIVED AN EMPTY MESSAGE" << std::endl;
        }
            
    }
    //! Push last messages received.
    gLock.lock();
    data_stored.push_back(message_session);
    gLock.unlock();
    std::cout << "Added the last vector of Messages to the deque" << std::endl;
    close_can();
    std::cout << "CLOSED CAN" << std::endl;
    return;
}