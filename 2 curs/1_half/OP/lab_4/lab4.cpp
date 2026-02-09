#include<iostream>

#include "lab4.h"

using namespace std;

bool Bankomat::input_sum(int sum){
    if(sum%10){return 1;}
    all_sum+=sum;
    return 0;
}

bool Bankomat::out_sum(int sum){
    if(all_sum<=0 or all_sum<sum or sum%10){return 1;}
    all_sum-=sum;
    return 0;
}

void Bankomat::show_sum(){
    cout<<all_sum<<endl;
}

int main(){
    bool i=1;
    Bankomat bank;
    while (i)
    {      
        int sum=0;
        int x=0;
        cout<<"1 - Take sum  2 - Give sum  3 - Show sum 4- Exit"<<endl;
        cin>>x;
        switch (x)
        {
        case 1:
            cout<<"Enter sum"<<endl;
            cin>>sum;
            if(bank.out_sum(sum)){
                cout<<"Error"<<endl;
            }
            break;
        case 2:
            cout<<"Enter sum"<<endl;
            cin>>sum;
            if(bank.input_sum(sum)){
                cout<<"Error"<<endl;
            }
            break;
        case 3:
            bank.show_sum();
            break;
        case 4:
            i=0;
        default:
            break;
        }
    }
    
}