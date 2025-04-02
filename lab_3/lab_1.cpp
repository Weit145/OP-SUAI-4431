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

int main(){
    int c,e,u,b;
    short int x=0;
    c=input('c',7);
    u=input('u',31);
    e=input('e',63);
    b=input('b',1);
    cout<<"Size x: "<<sizeof(x)<<endl;
    x=b|e<<2|u<<8|c<<13;
    cout<<hex<<x<<endl;
}