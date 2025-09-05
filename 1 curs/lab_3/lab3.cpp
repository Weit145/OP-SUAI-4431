#include <iostream>
#include <math.h>

using namespace std;

int input(char a,int d){
    int l;
    cout<<"Input "<<a<<":"<<endl;
    cin>>l;
    while(!cin.good() or l>d or l<0){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cout<<"Problem with "<<a<<" repeat"<<endl;
        cin>>l;
    }
    return l;
}
int lab3_1(){
    int c,e,u,b;
    short int x=0;
    c=input('c',7);
    u=input('u',31);
    e=input('e',63);
    b=input('b',1);
    cout<<"Size x: "<<sizeof(x)<<endl;
    x=b|e<<2|u<<8|c<<13;
    cout<<hex<<x<<endl;
    return x;
}

int main(){
    short int x=lab3_1();
    //lab3_2
    int y=x&0x1;
    cout<<"B: "<<dec<<y<<endl;
    y=(x>>2)&0x3f;
    cout<<"E: "<<dec<<y<<endl;
    y=(x>>8)&0x1f;
    cout<<"U: "<<dec<<y<<endl;
    y=(x>>13)&0x7;
    cout<<"C: "<<dec<<y<<endl;
}


