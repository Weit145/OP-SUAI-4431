#include <iostream>
#include <cmath>
#include <random>

using namespace std;

//6 вариант


unsigned input_size(){
    int new_size;
    cout<<"Input size:"<<endl;
    cin>>new_size;
    while(!cin.good() or new_size<0){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout<<"Problem with size, repyt"<<endl;
        cin>>new_size;
    }
    return new_size;
}

void input_random(int* arr, int size,int& sum){
    int min_r=-((size/2) - 1);
    int max_r=size/2;
    random_device rd;
    mt19937 gen(rd());
    for (int i=0;i<size;i++){
        uniform_int_distribution<int> dist(min_r, max_r);
        int random_num=dist(gen);
        if (random_num==0){sum++;}
        arr[i]=random_num;
    }
}

void output(int* arr, int size){
    for(int i=0;i<size;i++){
        cout<<arr[i]<<" ";
    }
    cout<<endl;
}

void zad_1(int* arr, int size){
    int sum=0;
    for (int i=0;i<size;i++){
        if(arr[i]<0){ sum+=arr[i];}
    }
    cout<<"Sum number<0:"<<sum<<endl;
}

void zad_2(int sum){
    cout<<"Number zero:"<<sum<<endl;
}


int main(){
    int size= input_size();
    int* mas = new int [size];
    int sum=0;
    input_random(mas,size,sum);
    output(mas,size);
    bool flag=1;
    while(flag){
        cout<<"Enter 1 for Sum number<0 or 2 for Number zero or anything for end"<<endl;
        int x;
        cin>>x;
        switch (x)
        {
        case 1:
            zad_1(mas,size);
            break;
        case 2:
            zad_2(sum);
            break;
        default:
            flag=0;
            break;
        }
        
    }
    delete[] mas;
}