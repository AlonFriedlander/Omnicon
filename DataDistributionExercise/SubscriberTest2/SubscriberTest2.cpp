#include "Subscriber\Subscriber.h"

int main()
{
    Subscriber subscriber("ExampleSubscriber2");
    subscriber.subscribe(ShapeType::SQUARE, "127.0.0.1", 11111); // Example subscription

    std::this_thread::sleep_for(std::chrono::seconds(50)); // Sleep for 10 seconds
    return 0; // Return success
}