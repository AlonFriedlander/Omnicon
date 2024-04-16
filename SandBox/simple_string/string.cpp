#include "String.h"

String::String(const char *str){
    m_str = new char[strlen(str) + 1];
    strcpy(m_str, str);
}
String::String(const String &other) : m_str(nullptr){
    m_str = new char[strlen(other.m_str) + 1];
    strcpy(m_str, other.m_str);
}

String &String::operator=(const String &other){
    if(this != &other){
        delete[] m_str;
        m_str = new char[strlen(other.m_str) + 1];
        strcpy(m_str,other.m_str);
    }
    return *this;
}

String::~String(){
    delete[] m_str;
}

size_t String::length() const{
    return strlen(m_str);
}

const char* String::cstr() const{
    return m_str;
}

