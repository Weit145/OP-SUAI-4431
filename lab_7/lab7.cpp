#include <iostream>
#include <cmath>

using namespace std;

int input(char* &mas,int &size){
    char a;
    int n;
    for(n=0;cin.get(a) and a!='\n';n++){
        if(n>=size-1){
            size*=2;
            char* mas1 = new char[size];
            for(int i=0;i<n;i++){
                mas1[i]=mas[i];
            }
            delete []mas;
            mas=mas1;
        }
        mas[n]=a;
    }
    mas[n]='\0';
    return n;
}

int number(){
    int n;
    cout << "Input size string"<< endl;
    cin >> n;
    while (!cin.good() or n<0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Problem with parametr repyt" << endl;
        cin >> n;
    }
    return n;
}

void output(char* arr,const int n){
    for(int i=0;i<n;i++){
        cout<<arr[i];
    }
    cout<<endl;
}

bool zad(char* &mas,int &size,int n,int j){
    int raz=n-j;
    if (size<n){
        int i;
        char* mas2=new char[n];
        for(i=0;i<size;i++){
            mas2[i]=mas[i];
        }
        mas2[i]='\0';
        size=n;
        delete []mas;
        mas=mas2;
    }
    if (raz<=0){
        return 0;
    }
    else{
        for(int i=0;i<size-2;i++){
            
        }
    }

}

int main(){
    int n,size=25;
    char* mas = new char[size];
    int j=input(mas,size);
    output(mas,size);
    n=number();
    delete [] mas;
}