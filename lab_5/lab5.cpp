#include <iostream>
#include <math.h>

using namespace std;

const int n=5;

void input(double* arr){
    double l;
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
double zad1(double* arr){
    double max=abs(arr[0]);
    for(int k=0;k<n;k++){
        if (max<abs(arr[k])){
            max=abs(arr[k]);
        }
        else{
            continue;
        }
    }
    return max;
}
double zad2(double* arr){
    double sum=0;
    bool flag=0;
    for(int i=0;i<n;i++){
        if((arr[i]>0 or flag==1) and i+1<n){
            flag=1;
            sum+=abs(arr[i+1]);
        }
    }
    return sum;
}
void zad3(double* arr){
    int x,y,l=0;
    double k;
    bool flag=0,fal=0,hell=0,good=0;
    cout<<"Input x and y:"<<endl;
    cin>>x>>y;
    while (hell==0){
        if (!cin.good() or good==1){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            cout<<"Problem  repeat"<<endl;
            cin>>x>>y;
            good=0;
        }
        else{
            for(int d=0;d<n;d++){
                if(int(arr[d])==x){
                    fal=1;
                }
                else if(int(arr[d])==y and fal==1){
                    hell=1;
                }
            }
            fal=0;
            good=1;
        }
    }
    for(int i=0;i<n;i++){
        if(int(arr[i])==x or flag==1){
            int(arr[i])==y ? flag=0 : flag=1;
            k=arr[l];
            arr[l]=arr[i];
            arr[i]=k;
            l++;
        }
    }
}

int main(){
    double mass[n];
    input(mass);
    cout<<"Max: "<<zad1(mass)<<endl;
    cout<<"Sum: "<<zad2(mass)<<endl;
    zad3(mass);
    for(int i=0;i<n;i++){
        cout<<mass[i]<<"  ";
    }
    cout<<endl;
}