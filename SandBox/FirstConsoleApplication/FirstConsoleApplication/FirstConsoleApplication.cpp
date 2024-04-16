
#include <iostream>
#include <string>
#include "student.h"

using namespace std;
void print_hello();



int main()
{
    std::cout << "Hello World!\n";
    print_hello();

    student s;
    s.address = "new york";
    s.age = 50;
    s.first_name = "alon";
    s.last_name = "frie";
    s.printinfo();

    cout << "please give me your name" << endl;

    string str_name;
    getline(cin, str_name);


    cout << "Name: " << str_name << endl;

    return 0;
}


