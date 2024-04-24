#include "Subscriber\Subscriber.h"

int main()
{
    Subscriber subscriber("ExampleSubscriber", 12345);

    subscriber.subscribe(Subscriber::ShapeType::SQUARE, "127.0.0.1");

    std::this_thread::sleep_for(std::chrono::seconds(3)); 

    subscriber.subscribe(Subscriber::ShapeType::CIRCLE, "127.0.0.1");

    std::this_thread::sleep_for(std::chrono::seconds(3)); 

    subscriber.subscribe(Subscriber::ShapeType::SQUARE, "127.0.0.1"); 

    std::this_thread::sleep_for(std::chrono::seconds(500)); 
    return 0; 
}