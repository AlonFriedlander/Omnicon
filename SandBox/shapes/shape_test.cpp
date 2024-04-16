#include "shape.h"

int main() {
    // Create some shapes
    Line line;
    Circle circle;
    Rectangle rectangle;
    Square square;

    // Create a vector of shapes
    std::vector<Shape*> shapes = {&line, &circle, &rectangle, &square};

    // Draw the shapes
    DrawShapes(shapes);

    return 0;
}