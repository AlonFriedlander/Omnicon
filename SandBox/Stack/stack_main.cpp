#include <iostream>
#include "stack.h"

int main() {
    Stack stack;
    stack.Push(10);
    stack.Push(20);
    stack.Push(30);

    std::cout << "Top element: " << stack.Top() << std::endl;
    std::cout << "Stack count: " << stack.Count() << std::endl;

    stack.Pop();
    std::cout << "Top element after pop: " << stack.Top() << std::endl;
    std::cout << "Stack count after pop: " << stack.Count() << std::endl;

    return 0;
}