#pragma once

#include "../include/parser.h"
#include <vector>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <deque>

// Condition variables used for the writer.
extern std::mutex mtx;
extern std::condition_variable cv;
extern bool dataAvailablew;
extern bool dataAvailablem;



void reader(std::deque<std::vector<TMessage>>& data, const char* filepath,std::atomic<bool>& stop_flag, std::atomic<bool>& writing, std::mutex& gLock);