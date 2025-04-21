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

int zad1(double* arr,const int n){
    int f=0;
    double max=abs(arr[0]);
    for(int k=0;k<n;k++){
        if (max<=abs(arr[k])){
            f=k+1;
        }
    }
    return f;
}

int zad2(double* arr,const int n,double &sum){
    int flag=0;
    for(int i=0;i<n;i++){
        
        if(flag>0){
            sum+=abs(arr[i]);
            flag++;
        }
        else if (arr[i]>=0){
            flag++;
        }
    }
    return flag;
}

int info(char a,double* arr,int &x,const int n){
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
    double sum=0;
    double mass[n];
    input(mass,n);
    output(mass,n);
    cout<<"Max: "<<zad1(mass,n)<<endl;
    switch (zad2(mass,n,sum)){
    case 0:
        cout<<"There is no positive number"<<endl;
        break;
    case 1:
        cout<<"No number after positive number"<<endl;
        break;
    default:
        cout<<"Sum: "<<sum<<endl;
        break;
    }
    zad3(mass,n);
    output(mass,n);
}