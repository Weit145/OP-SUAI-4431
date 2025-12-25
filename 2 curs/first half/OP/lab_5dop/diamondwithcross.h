#pragma once
#ifndef DIAMONDWITHCROSS_H
#define DIAMONDWITHCROSS_H

#include "shape.h"
#include "reflectable.h"
#include "rotatable.h"
#include "screen.h"
#include "point.h"
#include <vector>
#include <cmath>
#include <algorithm>

class DiamondWithCross : public Shape, public reflectable, public Rotatable
{
public:
    DiamondWithCross(const Point& center, std::uint32_t size)
        : _center(center), _size(size), _flippedHorizontally(false),
          _flippedVertically(false), _rotationAngle(0)
    {
    }
    
    // Shape interface
    virtual void draw(Screen* screen) const override
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
        
        screen->putLine(hStart, hEnd);
        screen->putLine(vStart, vEnd);
    }
    
    virtual void move(Point p) override
    {
        _center.setX(_center.getX() + p.getX());
        _center.setY(_center.getY() + p.getY());
    }
    
    virtual Point getLeftTop() const override
    {
        return Point(_center.getX() - _size, _center.getY() - _size);
    }
    
    virtual Point getRightTop() const override
    {
        return Point(_center.getX() + _size, _center.getY() - _size);
    }
    
    virtual Point getLeftBottom() const override
    {
        return Point(_center.getX() - _size, _center.getY() + _size);
    }
    
    virtual Point getRightBottom() const override
    {
        return Point(_center.getX() + _size, _center.getY() + _size);
    }
    
    // reflectable interface
    virtual void flipHorisontally() override
    {
        _flippedHorizontally = !_flippedHorizontally;
    }
    
    virtual void flipVertically() override
    {
        _flippedVertically = !_flippedVertically;
    }
    
    // Rotatable interface
    virtual void rotateLeft() override
    {
        _rotationAngle = (_rotationAngle - 90) % 360;
        if (_rotationAngle < 0) _rotationAngle += 360;
    }
    
    virtual void rotateRight() override
    {
        _rotationAngle = (_rotationAngle + 90) % 360;
    }
    
    Point getCenter() const { return _center; }
    
private:
    Point _center;
    std::uint32_t _size;
    bool _flippedHorizontally;
    bool _flippedVertically;
    int _rotationAngle;
    
    Point calculateRotatedPoint(const Point& p) const
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
};

#endif // DIAMONDWITHCROSS_H