#include <iostream>
#include <math.h>

using namespace std;

const int n=5;

int input(int a){
    int l;
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
    int mass[n];
    for(int i=0;i<n;i++){
        mass[i]=input(i+1);
    }
    //zad1
    int min=abs(mass[0]);
    for(int k=0;k<n;k++){
        if (min>abs(mass[k])){
            min=abs(mass[k]);
        }
        else{
            continue;
        }
    }
    cout<<"Min: "<<min<<endl;
    //zad2
    bool flag=0;
    int sum=0;
    for(int h=0;h<n;h++){
        if(flag==1){
            sum
        }
        else if (mass[h]<0){
            flag=1;
        }
        else{
            continue;
        }
    }
}