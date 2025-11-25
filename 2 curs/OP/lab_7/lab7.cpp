#include "lab7.h"
#include <cstdlib> 
#include <iostream>


using namespace std;


template <typename T>
void rd(vector<T> & t, const int size){
    for (int i=0;i<size;i++){
        double num = (rand() % 100) / 10.0;
        t[i]=T(num);
    }
}

int main(){
    srand(time(NULL));
    const int size = 10;
	vector<int> v(size);
    rd(v,size);
    Example V = Example(v,size);
    cout<<V.zad1()<<endl;
    V.show();
    V.zad2();
    V.show();
    V.st();
    V.show();


    vector<double> k(size);
    rd(k,size);
    Example K = Example(k,size);
    cout<<K.zad1()<<endl;
    K.show();
    K.zad2();
    K.show();
    K.st();
    K.show();


    vector<float> g(size);
    rd(g,size);
    Example G = Example(g,size);
    cout<<G.zad1()<<endl;
    G.show();
    G.zad2();
    G.show();
    G.st();
    G.show();
}