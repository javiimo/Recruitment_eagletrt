#include "../include/fake_receiver.h"
#include "../include/parser.h"
#include "../include/reader.h"
#include "../include/writer.h"
#include <iostream>
#include <string>
#include <cstring>
#include <deque>
#include <vector>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <chrono>



// Define writer variables
std::mutex mtx;
std::condition_variable cv;
bool dataAvailablew = false;
bool dataAvailablem = false;





void reader(std::deque<std::vector<TMessage>>& data_stored, const char* filepath,std::atomic<bool>& stop_flag, std::atomic<bool>& writing, std::mutex& dLock){
    // Declare message, vector of message session, open the file for CAN and set vector as non-written
    char message[MAX_CAN_MESSAGE_SIZE];
    std::vector<TMessage> message_session;
    int open_worked=open_can(filepath);
    bool running=false;
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
                running=true;
                std::cout << "RUNNING!" << std::endl;
                if (!message_session.empty()){
                    dLock.lock();
                    data_stored.push_back(message_session);
                    dLock.unlock();
                    message_session.clear();
                    std::cout << "Emptied vector and added, calling writer" << std::endl;
                    }
            }

            //Check if it is a Stop message
            if(std::string(message) == "0A0#66FF"){
                running=false;
                std::cout << "IDLE!" << std::endl;
                if (!message_session.empty()){
                    dLock.lock();
                    data_stored.push_back(message_session);
                    dLock.unlock();
                    message_session.clear();
                    std::cout << "Emptied vector and added" << std::endl;
                    // Call the writer and main threads to write the log and compute statistics of the last Runing Session.
                    std::unique_lock<std::mutex> lock(mtx);
                    dataAvailablew = true;
                    dataAvailablem = true;
                    lock.unlock();
                    cv.notify_all(); // Notify writer and main
                    std::cout << "Called Writer" << std::endl;
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

            // Reset the message array to receive the next message 
            memset(message, 0, sizeof(message)); 
        } else {
            std::cout << "RECEIVED AN EMPTY MESSAGE" << std::endl;
        }
            
    }
    // Push last messages received.
    dLock.lock();
    data_stored.push_back(message_session);
    dLock.unlock();
    std::cout << "Added the last vector of Messages to the deque" << std::endl;
    close_can();
    std::cout << "CLOSED CAN" << std::endl;

    // Call writer one last time if the last session was running
    if(running==true){
        std::unique_lock<std::mutex> lock(mtx);
        dataAvailablew = true;
        dataAvailablem = true;
        lock.unlock();
        cv.notify_all(); // Notify writer and main
        writing.store(false);
        std::cout << "Called Writer LAST TIME" << std::endl;
    }

    return;
}