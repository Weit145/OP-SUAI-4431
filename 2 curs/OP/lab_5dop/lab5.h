#ifndef LAB5_H
#define LAB5_H

#include <iostream>
#include <vector>
#include <string>

// -------------------------------------------------------------
// Абстрактный базовый класс "Фигура"
// -------------------------------------------------------------
class Shape {
protected:
    static std::vector<Shape*> allShapes;  // общий список всех фигур
public:
    Shape();                 // добавляет себя в список
    virtual ~Shape();        // виртуальный деструктор

    virtual void draw() const = 0;  // чисто виртуальная функция
    virtual std::string name() const = 0;

    // Виртуальные функции для получения крайних координат
    virtual int minX() const = 0;
    virtual int maxX() const = 0;
    virtual int minY() const = 0;
    virtual int maxY() const = 0;

    // Статический метод — вывести все фигуры
    static void drawAll();
};

// -------------------------------------------------------------
// Класс "Поворачиваемая фигура"
// -------------------------------------------------------------
class Rotatable {
public:
    virtual void rotate90() = 0; // повернуть на 90 градусов
    virtual ~Rotatable() {}
};

// -------------------------------------------------------------
// Класс "Отражаемая фигура"
// -------------------------------------------------------------
class Reflectable {
public:
    virtual void reflectX() = 0; // отражение по горизонтали
    virtual void reflectY() = 0; // отражение по вертикали
    virtual ~Reflectable() {}
};

// -------------------------------------------------------------
// Пример конкретной фигуры — Квадрат с крестом
// -------------------------------------------------------------
class SquareWithCross : public Shape, public Rotatable, public Reflectable {
    int x, y; // координаты левого верхнего угла
    int size;
public:
    SquareWithCross(int x, int y, int size);

    void draw() const override;
    std::string name() const override { return "Квадрат с крестом"; }

    int minX() const override { return x; }
    int maxX() const override { return x + size; }
    int minY() const override { return y; }
    int maxY() const override { return y + size; }

    void rotate90() override;
    void reflectX() override;
    void reflectY() override;
};

#endif // LAB5_H
