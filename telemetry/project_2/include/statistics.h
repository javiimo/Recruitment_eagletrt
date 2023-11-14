#include "../include/parser.h"
#include <vector>
#include <set>
#include <map>
#include <cstdint>
#include <string>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <iomanip>


// Struct for storing the info regarding the statistics regarding an ID
struct IDInfo {
    int count;
    std::time_t first_timestamp;
    std::time_t last_timestamp;
};

/**
 * @brief Uses a set to get the different types of ID that are there in a vector
 * 
 * @param messages vector containing the messages
 * @return the set of IDs
 */
std::set<std::uint16_t> find_unique_ids(const std::vector<TMessage>& messages);

/**
 * @brief Maps the unique IDs to their statistics needed, the IDInfo contains the count (in IDInfo.count) and the mean_time (in IDInfo.last_timestamp)
 * 
 * @param messages vector containing the messages
 * @return map between the unique IDs and their IDInfo which already containe the computed statistics
 */
std::map<std::uint16_t, IDInfo> count_messages_and_time_diff(const std::vector<TMessage>& messages);

/**
 * @brief Uses the two functions from above to write a csv file (should be called after each running session)
 * 
 * @param messages vector containing the messages
 */
void write_to_csv(const std::vector<TMessage>& messages);