#include <iostream>

#include "lab1.h"

using namespace std;

void lab1::SetA(int num_a){
    a=num_a;
}
void lab1::SetB(int num_b){
    b=num_b;
}
int lab1::GetA(){
    return a;
}
int lab1::GetB(){
    return b;
}

int lab1::NOD(){
    int k=a;
    int l=b;
    while (l != 0) {
        int temp = l;
        l = k % l;
        k = temp;
    }
    return k;
}


int main(){
    lab1 Num;
    Num.SetA(45);
    Num.SetB(25);
    cout<<"A:"<<Num.GetA()<<endl;
    cout<<"B:"<<Num.GetB()<<endl;
    cout<<"Наибольший общий делитель:"<<Num.NOD()<<endl;
}