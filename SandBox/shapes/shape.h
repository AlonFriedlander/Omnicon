#ifndef SHAPE_H
#define SHAPE_H

#include<iostream>
#include<vector>

class Shape{
    public:
        virtual void Draw();
        virtual void Move();
};

// Derived classes for specific shapes
class Line : public Shape {
public:
    void Draw() override;
    void Move() override;
};

class Circle : public Shape{
    public:
        virtual void Draw() override;
        virtual void Move() override;
};

class Rectangle : public Shape{
    public:
        virtual void Draw() override;
        virtual void Move() override;
};

class Square : public Shape{
    public:
        virtual void Draw() override;
        virtual void Move() override;
};

void DrawShapes(const std::vector<Shape*> &shapes);

#endif