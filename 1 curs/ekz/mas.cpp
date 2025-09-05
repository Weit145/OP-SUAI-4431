#include<iostream>

using namespace std;

void add(char* &mas,int &n){
    int k,i=0;
    char a;
    for(i;cin.get(a) and a!='\n';i++){
        if(i>=n-1){
            k=n*2;
            char* mass= new char[k];
            for (int l = 0; l < n; l++){
               mass[l]=mas[l];
            }
            delete [] mas;
            mas=mass;
            n=k;
        }
        mas[i]=a;
    }
    mas[i]='\0';
}


void out(char* mas){
    cout<<mas<<endl;
}

int main(){
    int n=16;
    char* mas=new char[n];
    add(mas,n);
    out(mas);
    delete [] mas;
}
