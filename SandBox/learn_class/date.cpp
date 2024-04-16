#include "date.h"

Date::Date(){
    month = 1;
    day = 1;
    year = 2020;
}

string Date::getShortDate() const{
    return to_string(month) + "/" + to_string(day) + "/" + to_string(year);
}

bool Date::operator==(const Date &rhs){
    return this->year == rhs.year;
}

ostream &operator << (ostream & outStream, const Date &dateToPrint){
    return (outStream<< dateToPrint.getShortDate());
}

