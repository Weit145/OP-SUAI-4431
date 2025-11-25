#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

template <typename T>
class Example{
private:
    vector<T> vec;
    const int size;
public:
    Example(vector<T> & t,const int size): vec(t), size(size) {}

    T zad1(){
        int sum=0;
        for(T i : vec){
            double k = double(i);
            sum+=k;
        }
        return T(sum);   
    }

    void zad2(){
        for(int i=0;i<size;i++){
            double k = double(vec[i]);
            double val = 2*k+5;
            if(val>20){
                vec[i]=T(10);
                continue;
            }
            vec[i]=T(val);
        }
    }

    void st(){
        sort(vec.begin(), vec.end());
    }

    void show(){
        for(double val : vec){
            cout << val << " ";
        }
        cout << endl;
    }
};