#include <iostream>
#include "Publisher\Publisher.h" 

int main() {
    const int PUB_PORT = 22222;
    Publisher publisher;
    publisher.startPublishing(); 

    std::this_thread::sleep_for(std::chrono::seconds(1000));

    publisher.stopPublishing(); 

    return 0; 
}