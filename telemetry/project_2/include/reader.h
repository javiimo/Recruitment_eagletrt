#pragma once

#include "../include/parser.h"
#include <vector>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <deque>

// Condition variables used for the writer and main for synchronization with reader thread.
extern std::mutex mtx;
extern std::condition_variable cv;
extern bool dataAvailablew;
extern bool dataAvailablem;


/**
 * @brief manages the messages received by the CAN simulator, it parses each message and store them in a session vector. The session vector is added to a deque for making it accessible to the rest of threads. It also fires signals to the main and writer threads to work.
 * 
 * @param data deque to store the vector sessions
 * @param filepath string containing path to the file (for open_can function)
 * @param stop_flag fired by the InputThread, to allow the user stop the receiving data process at any moment
 * @param writing atomic boolean to manage when the writing process should stop.
 * @param gLock mutex for preventing data races when accessing the deque
 */
void reader(std::deque<std::vector<TMessage>>& data, const char* filepath,std::atomic<bool>& stop_flag, std::atomic<bool>& writing, std::mutex& gLock);