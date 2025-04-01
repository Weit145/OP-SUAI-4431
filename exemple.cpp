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
void func(double n,double c, double a, double b,double &f,bool &flag){
    if (n<0.6 and b+c!=0){
        f=a*pow(n,3)-b*pow(n,2)+c;
    }
    else if(n>0.6 and b+c==0 and n!=c){
        f=(n-a)/(n-c);
    }
    else if(c!=0 and a!=0){
        f=n/c;
    }
    else{
        flag=0;
    }
}

int main(){
    double a,b,c,n,k,x,f;
    bool flag=1;
    a=input('a');
    b=input('b');
    c=input('c');
    n=input('n');
    k=input('k');
    x=input('x');
    if(n>=k or x>=abs(k-n)){
        cout<<"Error"<<endl;
    }
    else{
        for(n;n<=k;n=n+x){
            func(n,c,a,b,f,flag);
            if (flag==1){
                cout<<"x: "<<n<<"\t"<<"y: "<<f<<endl;
            }
            else{
                cout<<"Error"<<endl;
                break;
            }
        }
    }
}