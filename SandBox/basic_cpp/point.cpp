#include "point.h"
#include <iostream>
#include <cmath>

void point::AdjustBy(const point &other){
    x += other.x;
    y += other.y;
}

void point::AdjustBy(int dx, int dy){
    x+=dx;
    y+=dy;
}



double point::length() const{
    return std::sqrt(x*x + y*y);
}

void point::print (char open, char close) const{
    std::cout << open<<x<<","<<y<<close<<std::endl;
}

void point::print (ParenthesesType parenthesesType) const{
    std::cout << GetOpeningParentheses(parenthesesType) << x << "," << y << GetClosingParentheses(parenthesesType);
}


char point::GetOpeningParentheses(ParenthesesType parenthesesType) const {
    switch(parenthesesType) {
        case ROUND:
            return '(';
        case SQUARE:
            return '[';
        case CURLY:
            return '{';
        case ANGULAR:
            return '<';
        default:
            return '('; // Default to round parentheses
    }
}

char point::GetClosingParentheses(ParenthesesType parenthesesType) const {
    switch(parenthesesType) {
        case ROUND:
            return ')';
        case SQUARE:
            return ']';
        case CURLY:
            return '}';
        case ANGULAR:
            return '>';
        default:
            return ')'; // Default to round parentheses
    }
}