#include "Subscriber\Subscriber.h"

int main()
{
    Subscriber subscriber("ExampleSubscriber");
    subscriber.subscribe(ShapeType::CIRCLE, "127.0.0.1", 11111); // Example subscription
    subscriber.subscribe(ShapeType::SQUARE, "127.0.0.1", 11111); // Example subscription
    subscriber.receiveData(); // Start receiving data



    Subscriber subscriber2("ExampleSubscriber3");
    subscriber2.subscribe(ShapeType::CIRCLE, "127.0.0.1", 22222); // Example subscription
    subscriber2.subscribe(ShapeType::SQUARE, "127.0.0.1", 22222); // Example subscription
    subscriber2.receiveData(); // Start receiving data

    std::this_thread::sleep_for(std::chrono::seconds(50)); // Sleep for 10 seconds
    return 0; // Return success
}