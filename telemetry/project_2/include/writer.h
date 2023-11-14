#pragma once

#include "../include/parser.h"
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <vector>


/**
 * @brief writes the log file from the last vector of the deque
 * 
 * @param data_stored where the data retrieved for writing actions
 */
void write_last_vector_to_file(const std::deque<std::vector<TMessage>>& data_stored);

/**
 * @brief handles the loop for the thread execution desired and manages the calls to the write_last_vector_to_file function.
 * 
 * @param data_stored where the data retrieved for writing actions
 * @param writing atomic bool controlled by reader to manage the behaviour of this program
 */
void writer(std::deque<std::vector<TMessage>>& data_stored, std::atomic<bool>& writing);
