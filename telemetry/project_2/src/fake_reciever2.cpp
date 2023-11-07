#include "../include/fake_receiver.h"
#include <ctime>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <thread>
#include <chrono>

static int current_line_count = 0;
static int next_start_stop = 50;  
static int start_or_stop = 0;
static int line_count = 0;
static int opened = 0;
static std::ifstream can;

int open_can(const char* filepath) {
    if (opened == 1)
        return -1;

    can.open(filepath, std::ios::in);

    if (!can.is_open())
        return -1;

    std::srand(std::time(nullptr));
    opened = 1;

    char c;
    while (can.get(c)) {
        if (c == '\n') {
            line_count++;
        }
    }
    can.clear();
    can.seekg(0, std::ios::beg);

    return 0;
}

int can_receive(char message[MAX_CAN_MESSAGE_SIZE]) {
    if (opened == 0)
        return -1;

    char c;
    int bytes_received = 0;
    std::memset(message, 0, std::strlen(message));

    if ((current_line_count + 1) % next_start_stop == 0) {
        std::cout << "\n\n" << next_start_stop << ", " << start_or_stop << "\n\n";
        if (start_or_stop == 0) {      // start
            if (std::rand() % 2 == 0) {
                std::strcpy(message, "0A0#6601");
            } else {
                std::strcpy(message, "0A0#FF01");
            }
            start_or_stop = 1;
            next_start_stop = 400;
        } else {                      // stop
            std::strcpy(message, "0A0#66FF");
            start_or_stop = 0;
            next_start_stop = 100;
        }
        bytes_received = std::strlen(message);
        current_line_count++;
        return bytes_received;
    }

    while (can.get(c)) {
        if (c == '\n')
            break;
        message[bytes_received] = c;
        bytes_received++;
    }

    if (bytes_received == 0)
        return -1;

    current_line_count++;
    if (current_line_count == line_count)
        can.clear();
    can.seekg(0, std::ios::beg);

    std::this_thread::sleep_for(std::chrono::milliseconds((std::rand() % 50 + 1) * 1000));

    return bytes_received;
}

void close_can() {
    if (opened == 0)
        return;

    if (!can.is_open())
        return;

    can.close();
}
