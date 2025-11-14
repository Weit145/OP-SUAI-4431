#include "lab5.h"
#include <iostream>
#include <algorithm>

std::vector<Shape*> Shape::allShapes;


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



SquareWithCross::SquareWithCross(int x, int y, int size)
    : x(x), y(y), size(size) {}

void SquareWithCross::draw() const {
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

int main() {
    SquareWithCross s1(0, 0, 5);
    s1.draw();

    Shape::drawAll();
    return 0;
}
