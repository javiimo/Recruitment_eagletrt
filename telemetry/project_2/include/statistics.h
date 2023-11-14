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



struct IDInfo {
    int count;
    std::time_t first_timestamp;
    std::time_t last_timestamp;
};


std::set<std::uint16_t> find_unique_ids(const std::vector<TMessage>& messages);

std::map<std::uint16_t, IDInfo> count_messages_and_time_diff(const std::vector<TMessage>& messages);

void write_to_csv(const std::vector<TMessage>& messages);