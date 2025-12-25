#include <iostream>
#include <cmath> 
#include <stdexcept>
#include "lab6.h"

using namespace std;

Arr::Arr(int* arr, int size):arr_(arr), size_(size){}

int Arr::sum_unsined(){
    try
    {
        if (size_==0){
            throw runtime_error("Error: array is empty");
        }
        int sum=0;
        for (int i=0;i<size_;i++){
            if (arr_[i]>=0){sum+=arr_[i];}
        }
        if (!sum){
            throw runtime_error("Error: array is negative");
        }
        return sum;
    }
    catch(const std::exception& e)
    {
        throw;
    }
}

int Arr::mod_proz(){
    try
    {
        if (size_==0){
            throw Error("Error: array is empty");
        }
        int sum=0;
        int min=abs(arr_[0]);
        int max=abs(arr_[0]);
        int index_min=0;
        int index_max=0;
        for (int i=1;i<size_;i++){
            if (min>abs(arr_[i])){
                min=abs(arr_[i]);
                index_min=i;
            }
            if (max<abs(arr_[i])){
                max=abs(arr_[i]);
                index_max=i;
            }
        }
        if (abs(index_max - index_min) <= 1) {
            throw Error("Error: no elements between min and max elements");
        }
        int j=0;
        int l=size_;
        if (index_max>index_min){
            j=index_min+1;
            l=index_max;
        }
        else{
            l=index_min;
            j=index_max+1;
        }
        for (j;j<l;j++){
            sum+=arr_[j];
        }
        if (!sum){
            throw Error("Error: array is negative");
        }
        return sum;
    }
    catch(const std::exception& e)
    {
        throw;
    }
}

void Arr::show(){
    for (int i=0;i<size_;i++){
        cout<<arr_[i]<<"   ";
    }
    cout<<endl;
}

int main(){

    cout << "===== EXAMPLE 1: Mixed positive and negative =====" << endl;
    try
    {
        int arr[10] = {1, -2, 3, -4, 5, -6, 7, -8, 9, -10};
        int size=10;
        Arr mas(arr,size);

        cout << "Array:" << endl;
        mas.show();
        
        cout << "Sum of non-negative elements: " << mas.sum_unsined() << endl;
        cout << "Sum between min and max by absolute value: " << mas.mod_proz() << endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Exception caught: " << e.what() << '\n';
    }
    

    cout << "\n===== EXAMPLE 2: Empty array =====" << endl;
    try
    {
        int arr[1] = {};
        int size=0;
        Arr mas(arr,size);

        cout << "Array:" << endl;
        mas.show();
        
        cout << "Sum of non-negative elements: " << mas.sum_unsined() << endl;
        cout << "Sum between min and max by absolute value: " << mas.mod_proz() << endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Exception caught: " << e.what() << '\n';
    }
    

    cout << "\n===== EXAMPLE 3: Only negative numbers =====" << endl;
    try
    {
        int arr[3] = {-1,-3,-2};
        int size=3;
        Arr mas(arr,size);

        cout << "Array:" << endl;
        mas.show();
        
        cout << "Sum of non-negative elements: " << mas.sum_unsined() << endl;
        cout << "Sum between min and max by absolute value: " << mas.mod_proz() << endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Exception caught: " << e.what() << '\n';
    }
    
    cout << "\n===== EXAMPLE 4: Only positive numbers =====" << endl;
    try
    {
        int arr[5] = {10, 20, 30, 40, 50};
        int size=5;
        Arr mas(arr,size);

        cout << "Array:" << endl;
        mas.show();
        
        cout << "Sum of non-negative elements: " << mas.sum_unsined() << endl;
        cout << "Sum between min and max by absolute value: " << mas.mod_proz() << endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Exception caught: " << e.what() << '\n';
    }

    cout << "\n===== EXAMPLE 5: Array with zeros =====" << endl;
    try
    {
        int arr[5] = {0, 5, 0, -3, 0};
        int size=5;
        Arr mas(arr,size);

        cout << "Array:" << endl;
        mas.show();
        
        cout << "Sum of non-negative elements: " << mas.sum_unsined() << endl;
        cout << "Sum between min and max by absolute value: " << mas.mod_proz() << endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Exception caught: " << e.what() << '\n';
    }
    
    cout << "\n===== EXAMPLE 6: Min and max adjacent (no elements between) =====" << endl;
    try
    {
        int arr[4] = {1, -100, 50, -2};
        int size=4;
        Arr mas(arr,size);

        cout << "Array:" << endl;
        mas.show();
        
        cout << "Sum of non-negative elements: " << mas.sum_unsined() << endl;
        cout << "Sum between min and max by absolute value: " << mas.mod_proz() << endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Exception caught: " << e.what() << '\n';
    }
    
    cout << "\n===== EXAMPLE 7: Large range of values =====" << endl;
    try
    {
        int arr[8] = {-1000, 50, -500, 100, 75, -200, 25, 10};
        int size=8;
        Arr mas(arr,size);

        cout << "Array:" << endl;
        mas.show();
        
        cout << "Sum of non-negative elements: " << mas.sum_unsined() << endl;
        cout << "Sum between min and max by absolute value: " << mas.mod_proz() << endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Exception caught: " << e.what() << '\n';
    }

    cout << "\n===== EXAMPLE 8: Single element array =====" << endl;
    try
    {
        int arr[1] = {42};
        int size=1;
        Arr mas(arr,size);

        cout << "Array:" << endl;
        mas.show();
        
        cout << "Sum of non-negative elements: " << mas.sum_unsined() << endl;
        cout << "Sum between min and max by absolute value: " << mas.mod_proz() << endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Exception caught: " << e.what() << '\n';
    }
    
    cout << "\n===== All examples completed =====" << endl;
    return 0;
}
