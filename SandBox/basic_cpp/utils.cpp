#include "point.h"

point Add(const point &p1, const point &p2){
    point res;
    res.x = p1.x + p2.x;
    res.y = p1.y + p2.y;

    return res;
}