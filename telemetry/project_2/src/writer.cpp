#include <vector>
#include <string>
#include <atomic>

bool running=false;

void reader(std::vector<std::string>& raw_messages, const char* filepath,std::atomic<bool>& stop_flag){
    open_can(filepath);
    while(!stop_flag.load()){


    }
    close_can();
    return;
}