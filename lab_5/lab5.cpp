#include <iostream>
#include <math.h>

using namespace std;

void input(double* arr,const int n){
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
double zad1(double* arr,const int n){
    double max=abs(arr[0]);
    for(int k=1;k<n;k++){
        if (max<abs(arr[k])){
            max=abs(arr[k]);
        }
    }
    return max;
}
double zad2(double* arr,const int n){
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
double info(char a,double* arr,int &x,const int n){
    cout<<"Input "<<a<<":"<<endl; 
    cin>>x;
    while(!cin.good()){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cout<<"Problem with "<<a<<" repeat"<<endl;
        cin>>x;
    }
    for(int d=0;d<n;d++){
        if(int(arr[d])==x){
            return d+1;
        }
    }
    return 0;
}
void zad3(double* arr,const int n){
    int x,y,l=0;
    double k;
    bool flag=0;
    int f=info('a',arr,x,n);
    int u=info('b',arr,y,n);
    while (f==0 or u==0 or f>=u){
        cout<<"Problem  repeat"<<endl;
        f=info('a',arr,x,n);
        u=info('b',arr,y,n);
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
void output(double* arr,const int n){
    for(int i=0;i<n;i++){
        cout<<arr[i]<<"  ";
    }
    cout<<endl;
}
int main(){
    const int n=5;
    double mass[n];
    input(mass,n);
    output(mass,n);
    cout<<"Max: "<<zad1(mass,n)<<endl;
    cout<<"Sum: "<<zad2(mass,n)<<endl;
    zad3(mass,n);
    output(mass,n);
}