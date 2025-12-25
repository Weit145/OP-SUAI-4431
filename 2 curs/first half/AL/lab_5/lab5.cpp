#include<iostream>

using namespace std;

void sort(int*&arr, int n){
    int cmp = 0;
    int swap = 0;
    int gap=n/2;
    for(gap;gap>0;gap/=2){
        for(int i=0;i<n;i++){
            int temp = arr[i];
            int j = i;
            for (j=i;j >= gap and (++cmp and arr[j - gap] > temp);j-=gap, swap++){
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
    }
    cout<<"Swap: "<<swap<<endl;
    cout<<"Comparisons: "<<cmp<<endl;
}

int crate_mas(int*& arr){
    int n=0;
    cout<<"Input size: "<<endl;
    cin>>n;
    arr = new int [n];
    for (int i=0;i<n;i++){
        cout<<"Input num in "<<i<<" index: "<<endl;
        cin>>arr[i];
    }
    return n;
}

void delete_index(int*& arr,int n, int index){
    arr[index]=0;
}  
void add_index(int*& arr,int n, int index,int num){
    arr[index]=num;
}

void add_num_in_mas(int*&arr,int& n,int num){
    int new_size=n+1;
    int* mas=new int [new_size];
    for(int i=0;i<n;i++){
        mas[i]=arr[i];
    }
    mas[n]=num;
    delete[] arr;
    n=new_size;
    arr=mas;
}

void show(int* arr,int n){
    for (int i=0;i<n;i++){
        cout<<arr[i]<<endl;
    }
}

int main(){
    int* arr;
    int menu=0;
    int n=0;
    int index=0;
    int num=0;
    while (true)
    {
        cout<<"1-Exit\n2-Sort\n3-Delete index\n4-Add index\n5-Create masiv\n6-Show mas\n7-Add number"<<endl;
        cin>>menu;
        switch (menu)
        {
        case 1:
            delete[] arr;
            return 0;
        case 2:
            sort(arr,n);
            break;
        case 3:
            cout<<"Input index delete"<<endl;
            cin>>index;
            delete_index(arr,n,index);
            sort(arr,n);
            break;
        case 4:
            cout<<"Input index add and number"<<endl;
            cin>>index;
            cin>>num;
            add_index(arr,n,index,num);
            sort(arr,n);
            break;
        case 5:
            n=crate_mas(arr);
            break;
        case 6:
            show(arr,n);
            break;
        case 7:
            cout<<"Input number for add in arr"<<endl;
            cin>>num;
            add_num_in_mas(arr,n,num);
            break;
        default:
            cout<<"Erorr"<<endl;
            break;
        }
    }
    
    delete[] arr;
}