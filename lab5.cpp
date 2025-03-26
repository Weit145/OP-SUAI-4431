#include <iostream>
#include <math.h>

using namespace std;

const int n=5;

void input(int* arr){
    int l;
    for (int i=0;i<n;i++){
        cout<<"Input "<<i+1<<":"<<endl;
        cin>>l;
        while(!cin.good()){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            cout<<"Problem with "<<i+1<<" repeat"<<endl;
            cin>>l;
        }
        arr[i]=l;
    }
}
int zad1(int* arr){
    int min=abs(arr[0]);
    for(int k=0;k<n;k++){
        if (min>abs(arr[k])){
            min=abs(arr[k]);
        }
        else{
            continue;
        }
    }
    return min;
}
int zad2(int* arr){
    int sum=0;
    bool flag=0;
    for(int i=0;i<n;i++){
        if((arr[i]<0 or flag==1) and i+1<n){
            flag=1;
            sum+=abs(arr[i+1]);
        }
    }
    return sum;
}
void zad3(int* arr){
    int x,y;
    cout<<"Input x and y:"<<endl;
    cin>>x>>y;
    while(!cin.good() or x<1 or y>n or y<x){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cout<<"Problem  repeat"<<endl;
        cin>>x>>y;
    }
    int k=x;
    for(int i=0;i<n;i++){
        if (i>=x-1 and i<=y-1){
            arr[i]=0;
        }
        else if(i>=y){
            arr[k-1]=arr[i];
            arr[i]=0;
            k++;
        }
    }

}

int main(){
    int mass[n];
    input(mass);
    cout<<"Min: "<<zad1(mass)<<endl;
    cout<<"Sum: "<<zad2(mass)<<endl;
    zad3(mass);
    for(int i=0;i<n;i++){
        cout<<mass[i]<<"  ";
    }
}