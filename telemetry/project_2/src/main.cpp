#include <stdio.h>
#include <iostream>
#include "../include/parser.h"
#include "../include/reader.h"
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


int main(int argc, char* argv[]){
    // Check if an argument is provided
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <path_to_file>" << endl;
        return 1;
    }

    const char* path = argv[1];
    cout << "Welcome to Project 2" << endl;
    cout << "Press any button to stop receiving info" << endl;

    
    // Start reader thread
    atomic<bool> stopr_flag(false);
    atomic<bool> writing(false);
    thread readerThread(reader, ref(data_stored), path, ref(stopr_flag), ref(writing), ref(gLock));

    //Start a writer thread

    // Start a thread to check input to stop reading
    // To stop the thread you would have to press a key and then press enter.
    thread inputThread([&stopr_flag]() {
        string input;
        cout << "Input Thread started" << endl;
        while (true) {
            cin >> input;
            if (!input.empty()) {
                stopr_flag.store(true); // Signal the reader thread to stop.
                cout << "STOP READER" << endl;
                break;
            }
        }});
    
    cout << "Finished Main!!" << endl;
    readerThread.join();
    inputThread.join();
    return 0;
}