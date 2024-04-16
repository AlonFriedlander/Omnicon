#include "point.h"

int main(){
    point p1 = {1,2};
    point p2 = {3,4};
    point p3 = {5,6};

    p1.print('[',']');

    p1.AdjustBy(p2);
    p1.print('(',')');

    Add(p1,p2).print('(',')');


    point p4 = {1,2};
    point p5 = {3,4};
    point p6 = {5,6};

    p4.AdjustBy(5,7);
    p4.print(point::ANGULAR);

    return 0;
}