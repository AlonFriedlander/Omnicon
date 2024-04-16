#include "string.h"

int main(){
    String s1("hello");
    String s2("world");
    String s3(s1);

    s1 = s2 = s3;
    // s1.operator=(s2);
    std::cout<<"length of s1: "<<s1.length()<<std::endl;
    std::cout<<"content of s1: "<<s1.cstr()<<std::endl;
    std::cout<<"length of s2: "<<s2.length()<<std::endl;
    std::cout<<"content of s2: "<<s2.cstr()<<std::endl;
    std::cout<<"content of s3: "<<s3.cstr()<<std::endl;

    return 0;
}