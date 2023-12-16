#include <iostream>
#include <thread>
#include <string>

using namespace std::literals::chrono_literals;    

void doingWork(std::string prefix){
    for(int i = 0; i < 100; i++){
        std::cout << prefix << i;
        std::this_thread::sleep_for(100ms);
    }
}

int main(int, char**){
    // while(1){
    //     std::string action;
    //     std::cout << std::endl <<  "1. Start new work..." << std::endl << "2. Exit" << std::endl;
    //     std::getline(std::cin,action);
    //     if(action == "1"){
    //         doingWork("F");
    //     }
    //     if(action == "2"){
    //         return;
    //     }

    // }

    // int x=12;
    // std::this_thread::sleep_for(100ms);
    // x++; 
    // std::this_thread::sleep_for(100ms);
    // x++;
    // std::this_thread::sleep_for(100ms);
    // x++;
    


    std::thread worker1(doingWork,"F");
    std::thread worker2(doingWork,"G");

    worker1.join();
    worker2.join();
}
