#include <string>
#include <iostream>
using namespace std;

class Date
{
    private:
        int month;
        int day;
        int year;

    public:
        Date();
        Date(int theMonth, int theDay, int theYear): month(theMonth), day(theDay), year(theYear){}

        int getMonth() const{return month;}

        string getShortDate() const;

        void setDay(int day){this->day = day;}

        bool operator==(const Date &rhs);
};

ostream &operator << (ostream & outStream, const Date &dateToPrint);