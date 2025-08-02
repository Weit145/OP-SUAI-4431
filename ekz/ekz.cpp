#include<iostream>

using namespace std;

struct Node{
    int num;
    Node* nex;
};

void add_node(Node* &head,int value){
    Node* new_node=new Node;
    new_node->num=value;
    new_node->nex=nullptr;
    if (head==nullptr){
        head=new_node;
    }
    else{
        Node* temp=head;
        while (temp->nex!=nullptr)
        {
            temp=temp->nex;
        }
        temp->nex=new_node;
    }
}

void del(Node* &head){
    Node* temp=head;
    while (temp!=nullptr)
    {
        Node* ct=temp->nex;
        delete temp;
        temp=ct;
    }
    head=nullptr;
}

void pr(Node* head){
    Node* temp=head;
    while (temp!=nullptr)
    {
        cout<<temp->num<<endl;
        temp=temp->nex;
    }
}

void prres(Node* head){
    Node* temp=head;
    if(temp==nullptr) return;
    prres(temp->nex);
    cout<<temp->num<<endl;
}

int fn(Node* head){
    if(head==nullptr) return -1;
    Node* temp=head;
    int sum=temp->num;
    while (temp!=nullptr){
        if(sum<temp->num){
            sum=temp->num;
        }
        temp=temp->nex;
    }
    return sum;
}

int bin(int* mas,int n,int target){
    int left=0;
    int right=n-1;
    while (left<=right){
        int mid = left + (right - left) / 2;
        if(mas[mid]==target) {
            return mid;
        }
        else if(mas[mid]<target){
            left=mid+1;
        }
        else{
            right=mid-1;
        }
    }
    return -1;
}


void input(char* &mas,int &n){
    char a;
    int k;
    int i=0;
    cout<<"Input string"<<endl;
    for(i;cin.get(a) and a!='\n';i++){
        if(i>=n){
            k=n*2;
            char* mass=new char[k];
            for(int k=0;k<n;k++){
                mass[k]=mas[k];
            }
            delete []mas;
            mas=mass;
            n=k;
        }
        mas[i]=a;
    }
    mas[i]='\0';
}       

int main() {
    Node* head = nullptr;
    
    add_node(head, 3);
    add_node(head, 1);
    add_node(head, 4);
    add_node(head, 2);
    
    cout << "List: ";
    pr(head);       
    
    cout << "Reversed: ";
    prres(head);
    cout << endl;
    
    cout << "Max value: " << fn(head) << endl;
    
    del(head);
    
    int n=5;
    int mas[n]{3,4,5,6,7};
    
    cout<<bin(mas,n,7)<<endl;
    cout<<bin(mas,n,6)<<endl;
    cout<<bin(mas,n,5)<<endl;
    cout<<bin(mas,n,4)<<endl;
    cout<<bin(mas,n,3)<<endl;

    int n=8;
    char* mass=new char[n];
    input(mass,n);
    return 0;
}