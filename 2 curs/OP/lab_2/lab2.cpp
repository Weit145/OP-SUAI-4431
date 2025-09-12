#include <iostream>

#include "lab2.h"

using namespace std;

double Worker::getSalary(double rate, double days){
    return rate*days;
}

void Worker::print(){
        cout<<"Name: "<<name<<"\n"<<"Surname: "<<surname<<"\n"<<"Rate: "<<rate<<"\n"<<"Days: "<<days<<"\n"<<"Salary: "<<getSalary(rate,days)<<"\n"<<endl;
    }

int main(){
    Worker obj1;
    obj1.print();
    Worker obj2("Bob","Wilson",100,20);
    obj2.print();
    Worker obj3(obj2);
    obj3.print();
}