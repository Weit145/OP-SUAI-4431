#include <iostream>
#include <math.h>

using namespace std;

unsigned int input_size(){
    unsigned int n;
    cout<<"Input Size "<<endl;
    cin>>n;
    while(!cin.good()){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cout<<"Problem with parametr repyt"<<endl;
        cin>>n;
    }
    return n;
}
void input(double** arr, int n){
    double l;
    for(int i=0;i<n;i++){
        for(int k=0;k<n;k++){
            cout<<"Input "<<i+1<<" "<<k+1<<":"<<endl;
            cin>>l;
            while(!cin.good()){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(),'\n');
                cout<<"Problem with "<<i+1<<" "<<k+1<<" repeat"<<endl;
                cin>>l;
            }
            arr[i][k]=l;
        }
    }
}
void output(double** arr, int n){
    for(int i=0;i<n;i++){
        for(int k=0;k<n;k++){
            cout<<arr[i][k]<<" ";
        }
        cout<<endl;
    }
}
// void zad1(double arr[n][n]){
//     double sum=0;
//     for(int i=0;i<n;i++){
//         for(int k=0;k<n;k++){
//             arr[i][k]==0 ? 0 : sum++;
//         }
//         cout<<endl;
//     }
// }
int main(){
    int n=input_size();
    double** mass = new double*[n];
    for(int i = 0; i < n; i++) {
        mass[i] = new double[n];
    }
    input(mass,n);
    output(mass,n);
    for(int i = 0; i < n; i++) {
        delete[] mass[i];
    }
    delete[] mass;
}