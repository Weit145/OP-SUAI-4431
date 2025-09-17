class Point{
private:
    double x;
    double y;
public:
    Point(double a,double b);
    ~Point();
    double distance(const Point& other)const;

    double getx()const;
    double gety()const;
};

class Triangle{
private:
    Point a,b,c;
public:
    Triangle(const Point& a,const Point& b,const Point& c);
    ~Triangle();
    double Area()const;
    bool Valid()const;
    operator bool()const;
    friend bool operator<(const Triangle& a,const Triangle& b);
    friend bool operator==(const Triangle& a,const Triangle& b);
};

