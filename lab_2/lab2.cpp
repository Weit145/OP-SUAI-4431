#include <iostream>
#include <math.h>

using namespace std;


double input(char a){
    double l;
    cout<<"Input "<<a<<":"<<endl;
    cin>>l;
    while(!cin.good()){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cout<<"Problem with "<<a<<" repeat"<<endl;
        cin>>l;
    }
    return l;
}

int main(){
    double x,y;
    x=input('x');
    y=input('y');
    if (((y<=0 and y>=-1) and (x<=1 and x>=-1)) or (pow(x,2)+pow(y,2)<=1 and y<=1)){
        cout<<"True"<<endl;
    }
    else{
        cout<<"False"<<endl;
    }
}