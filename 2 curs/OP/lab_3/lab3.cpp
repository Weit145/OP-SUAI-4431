#include<iostream>
#include <cmath>

#include "lab3.h"

using namespace std;

Point::Point(double a,double b){
    x=a;
    y=b;
}
Point::~Point(){}

double Point::distance(const Point& other)const{
    return sqrt(pow(x-other.x,2)+pow(y-other.y,2));
}

double Point::getx()const{return x;}
double Point::gety()const{return y;}


Triangle::Triangle(const Point& a,const Point& b,const Point& c):a(a),b(b),c(c){};
Triangle::~Triangle(){}

double  Triangle::Area()const{
    return abs((a.getx()*(b.gety()-c.gety())+b.getx()*(c.gety()-a.gety())+c.getx()*(a.gety()-b.gety()))/2.0);
}

bool Triangle::Valid()const{
    return Area()>0;
}

Triangle::operator bool()const{
    return Valid();
}

bool operator<( Triangle& t1,  Triangle& t2) {
    return t1.Area() < t2.Area();
}

bool operator==( Triangle& t1,  Triangle& t2) {
    return abs(t1.Area() - t2.Area()) < 0;
}

int main(){
    Point p1(0, 0), p2(0, 3), p3(4, 0);
    Point p4(0, 0), p5(0, 5), p6(5, 0);
    Point p7(0, 0), p8(1, 1), p9(2, 2);
    
    Triangle t1(p1, p2, p3);
    Triangle t2(p4, p5, p6);
    Triangle t3(p7, p8, p9);
    
    cout << "Triangle 1 area: " << t1.Area() << endl;
    cout << "Triangle 2 area: " << t2.Area() << endl;
    cout << "Triangle 3 area: " << t3.Area() << endl;
    
    cout << "Triangle 1 is valid: " <<bool(t1) << endl;
    cout << "Triangle 3 is valid: " << bool(t3) << endl;
    
    cout << "t1 < t2: " << (t1 < t2) << endl;
    cout << "t1 == t2: " << (t1 == t2) << endl;

}