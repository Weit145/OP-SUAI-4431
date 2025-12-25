#include "diamondwithcross.h"
#include <algorithm>
#include <cmath>

DiamondWithCross::DiamondWithCross(const Point& center, std::uint32_t size)
    : _center(center), _size(size), _flippedHorizontally(false),
      _flippedVertically(false), _rotationAngle(0)
{
}

void DiamondWithCross::draw(Screen* screen) const
{
    // Вершины ромба
    std::vector<Point> diamondPoints = {
        Point(_center.getX(), _center.getY() - _size),           // верх
        Point(_center.getX() + _size, _center.getY()),           // право
        Point(_center.getX(), _center.getY() + _size),           // низ
        Point(_center.getX() - _size, _center.getY())            // лево
    };
    
    // Применяем трансформации
    for (auto& point : diamondPoints) {
        // Поворот
        point = calculateRotatedPoint(point);
        
        // Отражение по горизонтали
        if (_flippedHorizontally) {
            std::int32_t dx = static_cast<std::int32_t>(point.getX()) - static_cast<std::int32_t>(_center.getX());
            point.setX(_center.getX() - dx);
        }
        
        // Отражение по вертикали
        if (_flippedVertically) {
            std::int32_t dy = static_cast<std::int32_t>(point.getY()) - static_cast<std::int32_t>(_center.getY());
            point.setY(_center.getY() - dy);
        }
    }
    
    // Рисуем ромб (соединяем точки)
    for (size_t i = 0; i < diamondPoints.size(); ++i) {
        size_t next = (i + 1) % diamondPoints.size();
        screen->putLine(diamondPoints[i], diamondPoints[next]);
    }
    
    // Рисуем горизонтальную линию креста
    Point hStart = Point(_center.getX() - _size/2, _center.getY());
    Point hEnd = Point(_center.getX() + _size/2, _center.getY());
    
    // Рисуем вертикальную линию креста
    Point vStart = Point(_center.getX(), _center.getY() - _size/2);
    Point vEnd = Point(_center.getX(), _center.getY() + _size/2);
    
    // Применяем трансформации к линиям креста
    hStart = calculateRotatedPoint(hStart);
    hEnd = calculateRotatedPoint(hEnd);
    vStart = calculateRotatedPoint(vStart);
    vEnd = calculateRotatedPoint(vEnd);
    
    if (_flippedHorizontally) {
        std::swap(hStart, hEnd);
        std::int32_t dx1 = static_cast<std::int32_t>(vStart.getX()) - static_cast<std::int32_t>(_center.getX());
        vStart.setX(_center.getX() - dx1);
        std::int32_t dx2 = static_cast<std::int32_t>(vEnd.getX()) - static_cast<std::int32_t>(_center.getX());
        vEnd.setX(_center.getX() - dx2);
    }
    
    if (_flippedVertically) {
        std::swap(vStart, vEnd);
        std::int32_t dy1 = static_cast<std::int32_t>(hStart.getY()) - static_cast<std::int32_t>(_center.getY());
        hStart.setY(_center.getY() - dy1);
        std::int32_t dy2 = static_cast<std::int32_t>(hEnd.getY()) - static_cast<std::int32_t>(_center.getY());
        hEnd.setY(_center.getY() - dy2);
    }
    
    screen->putLine(hStart, hEnd);
    screen->putLine(vStart, vEnd);
}

void DiamondWithCross::move(Point p)
{
    _center.setX(_center.getX() + p.getX());
    _center.setY(_center.getY() + p.getY());
}

Point DiamondWithCross::getLeftTop() const
{
    return Point(_center.getX() - _size, _center.getY() - _size);
}

Point DiamondWithCross::getRightTop() const
{
    return Point(_center.getX() + _size, _center.getY() - _size);
}

Point DiamondWithCross::getLeftBottom() const
{
    return Point(_center.getX() - _size, _center.getY() + _size);
}

Point DiamondWithCross::getRightBottom() const
{
    return Point(_center.getX() + _size, _center.getY() + _size);
}

void DiamondWithCross::flipHorisontally()
{
    _flippedHorizontally = !_flippedHorizontally;
}

void DiamondWithCross::flipVertically()
{
    _flippedVertically = !_flippedVertically;
}

void DiamondWithCross::rotateLeft()
{
    _rotationAngle = (_rotationAngle - 90) % 360;
    if (_rotationAngle < 0) _rotationAngle += 360;
}

void DiamondWithCross::rotateRight()
{
    _rotationAngle = (_rotationAngle + 90) % 360;
}

Point DiamondWithCross::calculateRotatedPoint(const Point& p) const
{
    if (_rotationAngle == 0) return p;
    
    double angle = _rotationAngle * M_PI / 180.0;
    double cosA = cos(angle);
    double sinA = sin(angle);
    
    // Преобразуем в относительные координаты
    double dx = static_cast<double>(p.getX()) - static_cast<double>(_center.getX());
    double dy = static_cast<double>(p.getY()) - static_cast<double>(_center.getY());
    
    // Поворачиваем
    double newX = dx * cosA - dy * sinA;
    double newY = dx * sinA + dy * cosA;
    
    // Преобразуем обратно в абсолютные координаты
    return Point(static_cast<std::uint32_t>(_center.getX() + newX + 0.5),
                 static_cast<std::uint32_t>(_center.getY() + newY + 0.5));
}