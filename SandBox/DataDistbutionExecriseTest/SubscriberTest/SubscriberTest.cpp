#include "Subscriber\Subscriber.h"

int main(int argc, char *argv[])
{
    // Command line arguments:
    // subscriberName: Specify a name for this test instance.
    // portNumber: Enter a port number for this instance.
    // shapeType: Choose the shape(s) you want to receive. Available options: SQUARE, CIRCLE, or both SQUARE:CIRCLE.
    // attributes: Specify the attributes you want to receive. Available options: size, coordinates, colors.

    // Convert command-line arguments to vector of strings, excluding the program name
    std::vector<std::string> args(argv + 1, argv + argc);

    // Pass the arguments as an array to the constructor of Subscriber
    Subscriber subscriber(args);

    std::this_thread::sleep_for(std::chrono::seconds(50000));
    return 0;
}