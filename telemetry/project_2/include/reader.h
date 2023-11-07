#pragma once
#include "../include/parser.h"
#include <vector>
#include <atomic>
#include <mutex>
#include <deque>
extern bool running;

void reader(std::deque<std::vector<TMessage>>& data, const char* filepath,std::atomic<bool>& stop_flag, std::atomic<bool>& writing, std::mutex& gLock);