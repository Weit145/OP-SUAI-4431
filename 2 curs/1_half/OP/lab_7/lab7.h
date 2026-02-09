#include <iostream>
#include <vector>

using namespace std;

template <typename T>
class Example{
private:
    vector<T> vec;
    const int size;
public:
    Example(vector<T> & t,const int size): vec(t), size(size) {}

    T zad1();

    void zad2();

    void st();

    void show();
};