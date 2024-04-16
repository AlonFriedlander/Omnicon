#include "date.h"

int main(){
    
    Date *defaultDate = new Date;

    Date *schoolStart = new Date(8,17,2020);

    cout << defaultDate->getShortDate()<< endl;
    cout << schoolStart->getShortDate()<< endl;

    defaultDate->setDay(20);
    cout << defaultDate->getShortDate()<< endl;

    Date otherDate(8,17,2020);

    cout<< (otherDate == *schoolStart)<<endl;
}
