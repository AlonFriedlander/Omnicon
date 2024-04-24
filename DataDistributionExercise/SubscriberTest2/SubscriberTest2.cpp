#include "Subscriber\Subscriber.h"

int main()
{
    Subscriber subscriber("ExampleSubscriber2",12346);
    //subscriber.subscribe(Subscriber::ShapeType::CIRCLE, "127.0.0.1"); // Example subscription

    //std::this_thread::sleep_for(std::chrono::seconds(3)); // Sleep for 10 seconds


    subscriber.subscribe(Subscriber::ShapeType::SQUARE, "127.0.0.1"); // Example subscription

    //Subscriber subscriber2("ExampleSubscriber3");
    //subscriber2.subscribe(ShapeType::CIRCLE, "127.0.0.1", 22222); // Example subscription
    //subscriber2.subscribe(ShapeType::SQUARE, "127.0.0.1", 22222); // Example subscription

    std::this_thread::sleep_for(std::chrono::seconds(500)); // Sleep for 10 seconds
    return 0; // Return success
}