#include "shape.h"

// Implementation of Line class
void Line::Draw(){
    std::cout << "Drawing Line" << std::endl;
}

void Line::Move() {
    std::cout << "Moving Line" << std::endl;
}

// Implementation of Circle class
void Circle::Draw(){
    std::cout << "Drawing Circle" << std::endl;
}

void Circle::Move() {
    std::cout << "Moving Circle" << std::endl;
}

// Implementation of Rectangle class
void Rectangle::Draw(){
    std::cout << "Drawing Rectangle" << std::endl;
}

void Rectangle::Move() {
    std::cout << "Moving Rectangle" << std::endl;
}

// Implementation of Square class
void Square::Draw(){
    std::cout << "Drawing Square" << std::endl;
}

void Square::Move() {
    std::cout << "Moving Square" << std::endl;
}

void DrawShapes(const std::vector<Shape*>& shapes) {
    for (const auto& shape : shapes) {
        shape->Draw();
        shape->Move();
    }
}