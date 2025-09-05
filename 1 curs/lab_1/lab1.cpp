#include <iostream>
#include <math.h>

using namespace std;

const double PI=3.14159;

double problem(int l){
    double x;
    cout<<"Input Grad "<<l<<endl;
    cin>>x;
    while(!cin.good()){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cout<<"Problem with parametr repyt"<<endl;
        cin>>x;
    }
    return (PI*x)/180;
}
int main(){
    double rad1,rad2,fuc1,fuc2;
    rad1=problem(1);
    rad2=problem(2);
    fuc1=(sin(rad1)+cos(2*rad2-rad1))/(cos(rad1)-sin(2*rad2-rad1));
    fuc2=(1+sin(2*rad2))/(cos(2*rad2));
    if((cos(rad1)-sin(2*rad2-rad1))!=0 or (cos(2*rad2))!=0){
        cout<<"Resultat 1 func: "<<fuc1<<endl;
        cout<<"Resultat 2 func: "<<fuc2<<endl;
    }
    else {
        cout<<"Problem with func"<<endl;
    }
}