#include <iostream>
#include <vector>

using namespace std;

template <typename T>
class Example{
private:
    T vec;
public:
    Example(const T& t): vec(t) {}
    int zad();
};