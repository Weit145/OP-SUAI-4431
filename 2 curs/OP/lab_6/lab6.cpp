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
            throw runtime_error("Ошибка: массив пустой");
        }
        int sum=0;
        for (int i=0;i<size_;i++){
            if (arr_[i]>=0){sum+=arr_[i];}
        }
        if (!sum){
            throw runtime_error("Ошибка: массив отрицательный");
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
                throw Error("Ошибка: массив пустой");
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
                throw Error("Ошибка: между минимальным и максимальным элементами нет других элементов");
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
            throw Error("Ошибка: массив отрицательный");
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
    try
    {
        int arr[10] = {1, -2, 3, -4, 5, -6, 7, -8, 9, -10};
        int size=10;
        Arr mas(arr,size);

        cout << "Массив:" << endl;
        mas.show();
        
        cout << "Сумма неотрицательных: " << mas.sum_unsined() << endl;
        cout << "Сумма между min и max по модулю: " << mas.mod_proz() << endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    try
    {
        int arr[0] = {};
        int size=0;
        Arr mas(arr,size);

        cout << "Массив:" << endl;
        mas.show();
        
        cout << "Сумма неотрицательных: " << mas.sum_unsined() << endl;
        cout << "Сумма между min и max по модулю: " << mas.mod_proz() << endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    try
    {
        int arr[3] = {-1,-3,-2};
        int size=3;
        Arr mas(arr,size);

        cout << "Массив:" << endl;
        mas.show();
        
        cout << "Сумма неотрицательных: " << mas.sum_unsined() << endl;
        cout << "Сумма между min и max по модулю: " << mas.mod_proz() << endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}