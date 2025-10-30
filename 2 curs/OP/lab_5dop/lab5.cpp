#include "lab5.h"
#include <iostream>
#include <algorithm>

std::vector<Shape*> Shape::allShapes;

// -------------------------------------------------------------
// Реализация методов Shape
// -------------------------------------------------------------
Shape::Shape() {
    allShapes.push_back(this);
}

Shape::~Shape() {}

void Shape::drawAll() {
    std::cout << "=== Все фигуры на экране ===\n";
    for (auto* s : allShapes)
        s->draw();
    std::cout << "============================\n";
}

// -------------------------------------------------------------
// Реализация SquareWithCross
// -------------------------------------------------------------
SquareWithCross::SquareWithCross(int x, int y, int size)
    : x(x), y(y), size(size) {}

void SquareWithCross::draw() const {
    std::cout << name() << " (" << x << "," << y << "), размер " << size << std::endl;

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (i == size / 2 || j == size / 2 || i == 0 || j == 0)
                std::cout << "#";
            else
                std::cout << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void SquareWithCross::rotate90() {
    std::cout << name() << ": повернут на 90 градусов.\n";
}

void SquareWithCross::reflectX() {
    std::cout << name() << ": отражён по горизонтали.\n";
}

void SquareWithCross::reflectY() {
    std::cout << name() << ": отражён по вертикали.\n";
}

// -------------------------------------------------------------
// Пример использования (можно разместить в main.cpp)
// -------------------------------------------------------------
#ifdef LAB5_DEMO
int main() {
    SquareWithCross s1(0, 0, 5);
    s1.draw();
    s1.rotate90();
    s1.reflectX();

    Shape::drawAll();
    return 0;
}
#endif
