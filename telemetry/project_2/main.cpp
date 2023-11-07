#include <stdio.h>
#include <iostream>
#include "include/reader.h"
#include "include/parser.h"
#include <string>
#include <deque>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <functional>

using namespace std;

#include "include/reader.h"
extern "C"{
    #include "include/fake_receiver.h"
}

deque<vector<TMessage>> data_stored;
mutex gLock;


int main(void){
    const char* path = "candump.log";
    cout << "Welcome to Project 2" << endl;
    cout << "Press any button to stop receiving info" << endl;

    
    // Start reader thread
    atomic<bool> stopr_flag(false);
    atomic<bool> writing(false);
    thread readerThread(reader, ref(data_stored), path, ref(stopr_flag), ref(writing), ref(gLock));

    //Start a writer thread

    // Start a thread to check input to stop reading
    thread inputThread([&stopr_flag]() {
        string input;
        while (true) {
            cin >> input;
            if (!input.empty()) {
                stopr_flag.store(true); // Signal the reader thread to stop.
                break;
            }
        }});
    
    cout << "Finished!!" << endl;
    readerThread.join();
    inputThread.join();
    return 0;
}