#include <iostream>
#include "Publisher\Publisher.h" // Include the header file for your Publisher class

int main() {
    const int PUB_PORT = 11111;
    Publisher publisher(PUB_PORT); // Create a Publisher object with the specified port
    publisher.startPublishing(); // Start publishing shapes to subscribers

    std::this_thread::sleep_for(std::chrono::seconds(1000)); // Sleep for 10 seconds
    
    //publisher.stopPublishing(); // Stop the publishing process
    
    return 0; // Return success
}
