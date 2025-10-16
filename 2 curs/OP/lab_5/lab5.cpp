#include<iostream>

#include"lab5.h"

using namespace std;

B1::B1(int x){
    e=x;
    cout<<"B1 Konstructor "<<e<<endl;
}
B1::~B1(){
    cout<<"B1 Destructor"<<endl;
}

B2::B2(int x){
    q=x;
    cout<<"B2 Konstructor "<<q<<endl;
}

B2::~B2(){
    cout<<"B2 Destructor"<<endl;
}

D1::D1(int b1,int b2, int d1): B1(b1), B2(b2){
    w=d1;
    cout<<"D1 Konstructor "<<w<<endl;
}

D1::~D1(){
    cout<<"D1 Destructor"<<endl;
}

D2::D2(int b1,int b2,int d2):B1(b1), B2(b2){
    r=d2;
    cout<<"D2 Konstructor "<<r<<endl;
}

D2::~D2(){
    cout<<"D2 Destructor"<<endl;
}

D3::D3(int b1,int b2,int d1,int d2,int d3):B1(b1), B2(b2), D1(b1, b2, d1), D2(b1, b2, d2){
    t=d3;
    cout<<"D3 Konstructor "<<t<<endl;
}

D3::~D3(){
    cout<<"D3 Destructor"<<endl;
}

void B1::B1show(){
    cout<<"B1: "<<e<<endl;
}

void B2::B2show(){
    cout<<"B2: "<<q<<endl;
}

void D1::D1show(){
    cout<<"D1: "<<w<<endl;
    B1show();
}

void D2::D2show(){
    cout<<"D2: "<<r<<endl;
    B2show();
}

void D3::D3show(){
    cout<<"D3: "<<t<<endl;
    D1show();
    D2show();
}

int main(){
    D3 temp(1,2,3,4,5);
    temp.D3show();
}