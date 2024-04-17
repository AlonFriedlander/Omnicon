// Shape.cpp

#include "Shape.h"

// Overload the << operator for Shape
//std::ostream& operator<<(std::ostream& os, const Shape& shape) {
//    shape.printProperties(os); // Assuming printProperties() prints the shape's properties to the output stream
//    return os;
//}

Square::Square(int size, const std::vector<int>& coordinates) : size(size), coordinates(coordinates) {}

void Square::printProperties(std::ostream& os) const {
    os << "Square - Color: blue, Size: " << size << ", Coordinates: ";
    for (int coord : coordinates) {
        os << coord << " ";
    }
    os << std::endl;
}

Circle::Circle(int size, const std::vector<int>& coordinates) : size(size), coordinates(coordinates) {}

void Circle::printProperties(std::ostream& os) const {
    os << "Circle - Color: green, Size: " << size << ", Coordinates: ";
    for (int coord : coordinates) {
        os << coord << " ";
    }
    os << std::endl;
}
