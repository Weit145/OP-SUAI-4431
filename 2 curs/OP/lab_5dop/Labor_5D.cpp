#include <iostream>
#include "screen.h"
#include "line.h"
#include "square.h"
#include <vector>
#include <memory>

int main()
{
    auto screen = std::make_unique<Screen>(50, 35);
    std::vector<std::shared_ptr<Shape>> shapes;
    
    shapes.emplace_back(std::make_shared<Square>(Point(15, 3), Point(32, 12))); // Шляпа
    Point p1 = shapes[shapes.size() - 1]->getLeftBottom();
    p1.setX(p1.getX() - 2);
    p1.setY(p1.getY() + 1);
    Point p2 = shapes[shapes.size() - 1]->getRightBottom();
    p2.setX(p2.getX() + 2);
    p2.setY(p2.getY() + 1);
    shapes.emplace_back(std::make_shared<Line>(p1, p2)); // Линия под шляпой
    
    p1 = shapes[shapes.size() - 1]->getLeftBottom();
    p1.setX(p1.getX() + 1);
    p1.setY(p1.getY() + 1);
    p2 = shapes[shapes.size() - 1]->getRightBottom();
    p2.setX(p2.getX() - 1); 
    p2.setY(p2.getY() + 10);
    shapes.emplace_back(std::make_shared<Square>(p1, p2)); // Голова

    // Вычисляем позиции для глаз
    Point eyeLeft = shapes[shapes.size() - 1]->getLeftTop();
    eyeLeft.setX(eyeLeft.getX() + 2);
    eyeLeft.setY(eyeLeft.getY() + 3);
    
    Point eyeRight = shapes[shapes.size() - 1]->getRightTop();
    eyeRight.setX(eyeRight.getX() - 2);
    eyeRight.setY(eyeRight.getY() + 3);

    // Левый глаз (ромб из 4 линий)
    Point eyeCenterLeft = eyeLeft;
    eyeCenterLeft.setX(eyeCenterLeft.getX() + 1);
    int rhombSize = 2;
    
    // Вершины ромба
    Point rhombTopLeft(eyeCenterLeft.getX(), eyeCenterLeft.getY() - rhombSize);
    Point rhombBottomLeft(eyeCenterLeft.getX(), eyeCenterLeft.getY() + rhombSize);
    Point rhombLeftLeft(eyeCenterLeft.getX() - rhombSize, eyeCenterLeft.getY());
    Point rhombRightLeft(eyeCenterLeft.getX() + rhombSize, eyeCenterLeft.getY());
    
    // 4 линии ромба
    shapes.emplace_back(std::make_shared<Line>(rhombTopLeft, rhombLeftLeft));    // Верх-Лево
    shapes.emplace_back(std::make_shared<Line>(rhombTopLeft, rhombRightLeft));   // Верх-Право
    shapes.emplace_back(std::make_shared<Line>(rhombBottomLeft, rhombLeftLeft)); // Низ-Лево
    shapes.emplace_back(std::make_shared<Line>(rhombBottomLeft, rhombRightLeft));// Низ-Право
    
    // Крест внутри ромба (опционально)
    shapes.emplace_back(std::make_shared<Line>(rhombTopLeft, rhombBottomLeft));  // Вертикаль
    shapes.emplace_back(std::make_shared<Line>(rhombLeftLeft, rhombRightLeft));  // Горизонталь

    // Правый глаз (ромб из 4 линий)
    Point eyeCenterRight = eyeRight;
    eyeCenterRight.setX(eyeCenterRight.getX() - 1);
    
    Point rhombTopRight(eyeCenterRight.getX(), eyeCenterRight.getY() - rhombSize);
    Point rhombBottomRight(eyeCenterRight.getX(), eyeCenterRight.getY() + rhombSize);
    Point rhombLeftRight(eyeCenterRight.getX() - rhombSize, eyeCenterRight.getY());
    Point rhombRightRight(eyeCenterRight.getX() + rhombSize, eyeCenterRight.getY());
    
    // 4 линии ромба
    shapes.emplace_back(std::make_shared<Line>(rhombTopRight, rhombLeftRight));    // Верх-Лево
    shapes.emplace_back(std::make_shared<Line>(rhombTopRight, rhombRightRight));   // Верх-Право
    shapes.emplace_back(std::make_shared<Line>(rhombBottomRight, rhombLeftRight)); // Низ-Лево
    shapes.emplace_back(std::make_shared<Line>(rhombBottomRight, rhombRightRight));// Низ-Право
    
    // Крест внутри ромба (опционально)
    shapes.emplace_back(std::make_shared<Line>(rhombTopRight, rhombBottomRight));  // Вертикаль
    shapes.emplace_back(std::make_shared<Line>(rhombLeftRight, rhombRightRight));  // Горизонталь

    // Нос
    Point nose = Point(eyeCenterLeft.getX() +
        (eyeCenterRight.getX() - eyeCenterLeft.getX()) / 2,
        eyeCenterLeft.getY() + 3);
    shapes.emplace_back(std::make_shared<Line>(nose, nose)); // Нос
    
    std::shared_ptr<Shape> head = shapes[2]; // Голова это третий элемент
    p1 = head->getLeftBottom(); 
    p1.setX(p1.getX() + 2); 
    p1.setY(p1.getY() - 1);
    p2 = head->getRightBottom(); 
    p2.setX(p2.getX() - 2); 
    p2.setY(p2.getY() - 1);
    shapes.emplace_back(std::make_shared<Line>(p1, p2)); // Рот
    
    auto costume = std::make_shared<Line>(Point(p1.getX(), p1.getY() + 10),
        Point(p2.getX(), p2.getY() + 10));
    shapes.emplace_back(costume); // Линия костюма
    
    p1 = Point(costume->getLeftBottom().getX(), costume->getLeftBottom().getY() + 1);
    auto leftDot = std::make_shared<Line>(p1, p1);
    shapes.emplace_back(leftDot); // Левая точка
    
    p1 = Point(costume->getRightBottom().getX(), costume->getRightBottom().getY() + 1);
    auto rightDot = std::make_shared<Line>(p1, p1);
    shapes.emplace_back(rightDot); // Правая точка
    
    for (auto shape : shapes)
        shape->draw(screen.get());
    
    screen->draw();
    std::cin.get();
    return 0;
}