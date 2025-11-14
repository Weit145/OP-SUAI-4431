#include <iostream>
#include <vector>
#include <string>

class Shape {
protected:
    static std::vector<Shape*> allShapes;
public:
    Shape();
    virtual ~Shape();

    virtual void draw() const = 0;

    virtual int minX() const = 0;   
    virtual int maxX() const = 0;
    virtual int minY() const = 0;
    virtual int maxY() const = 0;

    static void drawAll();
};

class Line: public Shape{
    int x,y;
public:
    Line(int x,int y);

    void draw() const override;

    int minX() const override{return x;}
    int minY() const override{return y;}
};

class Square : public Shape{
    int x, y;
public:
    Square(int x, int y);

    void draw() const override;

    int minX() const override{return x;}
    int minY() const override{return y;}
};

class SquareWithCross : public Shape {
    int x, y;
    int size;
public:
    SquareWithCross(int x, int y, int size);

    void draw() const override;

    int minX() const override { return x; }
    int maxX() const override { return x + size; }
    int minY() const override { return y; }
    int maxY() const override { return y + size; }


};

