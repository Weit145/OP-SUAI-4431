#include <iostream>
#include <math.h>
using namespace std;


int main(){
    int* a;
    int x=10;
    a=&x;
    *a=20;
    cout<<sizeof(a)<<endl;
    cout<<a<<endl;
    cout<<*a<<endl;
}
