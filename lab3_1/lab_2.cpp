#include <iostream>
#include <math.h>

using namespace std;

unsigned short input(char a){
    unsigned short l;
    cout<<"Input "<<a<<":"<<endl;
    cin>>hex>>l;
    while(!cin.good()){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cout<<"Problem with "<<a<<" repeat"<<endl;
        cin>>hex>>l;
    }
    return l;
}


int main(){
    unsigned short x=input('x');
    int y=x&0x1;
    cout<<"B: "<<dec<<y<<endl;
    y=(x>>2)&0x3f;
    cout<<"E: "<<dec<<y<<endl;
    y=(x>>8)&0x1f;
    cout<<"U: "<<dec<<y<<endl;
    y=(x>>13)&0x7;
    cout<<"C: "<<dec<<y<<endl;
}