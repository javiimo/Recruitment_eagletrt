#pragma once
#include <vector>
#include <string>
#include <atomic>
extern bool running;

void reader(std::vector<std::string>& raw_messages, const char* filepath,std::atomic<bool>& stop_flag);