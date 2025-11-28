#include "lab7.h"
#include <cstdlib> 
#include <iostream>
#include <algorithm>
#include <iomanip>

using namespace std;

template <typename T>
T Example<T>::zad1(){
    double sum=0;
    for(T i : vec){
        double k = double(i);
        sum+=k;
    }
    return T(sum);   
}

template <typename T>
void Example<T>::zad2(){
    for(int i=0;i<size;i++){
        double k = double(vec[i]);
        double val = 2*k+5;
        if(val>20){
            vec[i]=T(10);
            continue;
        }
        vec[i]=T(val);
    }
}

template <typename T>
void Example<T>::st(){
    sort(vec.begin(), vec.end());
}

template <typename T>
void Example<T>::show() {
    for (const auto& val : vec) {
        cout << setw(8) << val ;
    }
    cout<<endl;
}





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