#pragma once

#include "../include/parser.h"
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <vector>



void write_last_vector_to_file(const std::deque<std::vector<TMessage>>& data_stored);
void writer(std::deque<std::vector<TMessage>>& data_stored, std::atomic<bool>& writing);
