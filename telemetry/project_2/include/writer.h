#pragma once

#include "../include/parser.h"
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <vector>

// Condition variables used for the writer.
extern std::mutex mtx;
extern std::condition_variable cv;
extern bool dataAvailable;

void write_last_vector_to_file(const std::deque<std::vector<TMessage>>& data_stored);
void writer(std::deque<std::vector<TMessage>>& data_stored, std::atomic<bool>& writing);
