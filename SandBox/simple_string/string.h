#ifndef STRING_H
#define STRING_H

#include <iostream>
#include <cstring>

class String{
    public:
        String(const char *str = "");
        String(const String &other);

        String &operator=(const String &other);

        ~String();

        size_t length() const;
        const char *cstr() const;
    
    private:
        char *m_str;
};

#endif