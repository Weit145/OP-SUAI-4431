#include <iostream>

#include "lab2.h"

using namespace std;

double Worker::getSalary(double rate, double days){
    return rate*days;
}

int main(){
    Worker obj1;
    Worker obj2("Bob","Wilson",100,20);
    Worker obj3(obj2);
}