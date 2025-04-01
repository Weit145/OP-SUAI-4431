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
    if (n<1 and c!=0){
        f=a*n*n+(b/c);
    }
    else if(n>1.5 and c==0 ){
        f=(n-a)/(pow((n-c),2));
    }
    else{
        if (c!=0){
            f=n*n/(c*c);
        }
        else{
            flag=0;
        }
    }
}
int main(){
    double a,b,c,n,k,d,f;
    bool flag;
    a=input('a');
    b=input('b');
    c=input('c');
    n=input('n');
    k=input('k');
    d=input('d');
    if(n>=k or d>=abs(k-n)){
        cout<<"Error"<<endl;
    }
    else{
        for(n;n<=k;n+=d){
            flag=1;
            func(n,c,a,b,f,flag);
            if (flag==1){
                cout<<"x: "<<n<<"\t"<<"y: "<<f<<endl;
            }
            else{
                cout<<"Error"<<endl;
            }
        }
    }
}