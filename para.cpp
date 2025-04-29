#include <iostream>
#include <math.h>
using namespace std;

void print(int first, ...){
    int* ptr = &first;
    while (*ptr != 0) {
        cout << *ptr << " ";
        ++ptr;
    }
    cout << endl;
}

int main(int argc, char* argv[]){
    // int* a;
    // int x=10;
    // a=&x;
    // *a=20;
    // cout<<sizeof(a)<<endl;
    // cout<<a<<endl;
    // cout<<*a<<endl;
    print(1, 2, 3, 4, 0);
    print(3, 8, 0);
    cout << "Программа: " << argv[0] <<endl;
    cout << "Аргументов: " << argc-1 <<endl;
    
    for(int i = 1; i < argc; ++i) {
        cout << i << ": " << argv[i] <<endl;
    }
}
