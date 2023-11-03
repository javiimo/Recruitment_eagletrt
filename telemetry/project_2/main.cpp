#include <stdio.h>
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <atomic>
using namespace std;

#include "include/reader.h"
extern "C"{
    #include "include/fake_receiver.h"
}

int main(void){
    const char* path = "candump.log";
    cout << "Welcome to Project 2" << endl;

    // Start reader thread
    vector<string> raw_messages;
    atomic<bool> stop_flag(false);
    atomic<bool> writing(false);
    thread readerThread(reader, raw_messages, path, stop_flag, writing);

    // Start a thread to check input to stop reading
    thread inputThread([&stop_flag]() {
        string input;
        while (true) {
            cin >> input;
            if (!input.empty()) {
                stop_flag.store(true); // Signal the reader thread to stop.
                break;
            }
        }})
    
    return 0;
}