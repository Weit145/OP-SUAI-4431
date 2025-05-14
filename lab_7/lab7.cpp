#include <iostream>
#include <cmath>

using namespace std;

int input(char* &mas,int &size){
    char a;
    int n;
    cout << "Input string"<< endl;
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
    cout<<arr;
    // for(int i=0;i<n;i++){
    //     cout<<arr[i];
    // }
    cout<<endl;
}

int zad(char* &mas,int size,int n,int j){
    if (n<=j){
        return 0;
    }
    bool flag=0;
    int sum=0;
    for(int i=0;i<size;i++){
        if (mas[i]!=' ' and !flag){
            sum++;
            flag=1;
        }
        else if (mas[i]==' '){
            flag=0;

        }
    }
    if(sum<2){
        return 1;
    
    }
    int add=n-j;
    int space=sum-1;
    int base=add/space;
    int exta=add%space;
    char* new_mas = new char[n+1];

    int l=0;
    int cout=0;
    bool flag1=0;
    for(int i = 0; i < n; i++) {
        if (mas[i] != ' ') {
            new_mas[l++] = mas[i];
            flag1 = 1;
        } 
        else if (flag1 and cout < space) {
            new_mas[l++] = ' ';
            int insert = base + (cout < exta ? 1 : 0);
            for (int j = 0; j < insert; j++){
                new_mas[l++] = ' ';
            }
            cout++;
            flag1 = 0;
        }
    }
    new_mas[l] = '\0';
    delete [] mas;
    mas = new_mas;
    return 2;
}

int main(){
    int size=25;
    char* mas = new char[size];
    int j=input(mas,size);
    output(mas,size);
    int n=number();
    switch (zad(mas, size, n, j)){
    case 0:
        cout << "String is already long enough" << endl;
        break;
    case 1:
        cout << "Not enough words to add spaces between" << endl;
        break;
    default:
        output(mas,size);
        break;
    }
    delete [] mas;
}