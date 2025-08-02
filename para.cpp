#include <iostream>
#include <math.h>
#include <string>
using namespace std;

void print(int first, ...){
    int* ptr = &first;
    while (*ptr != 0) {
        cout << *ptr << " ";
        ++ptr;
    }
    cout << endl;
}


struct Node{
    int num;
    Node* next;
};

void add_node(Node* &head,int value){
    Node* new_Node=new Node;
    new_Node->num=value;
    new_Node->next=nullptr;
    if (head==nullptr){
        head=new_Node;
    }
    else{
        Node* temp=head;
        while (temp->next!=nullptr)
        {
           temp=temp->next;
        }
        temp->next=new_Node;
    }
}

void pr(Node* head){
    Node* temp=head;
    while (temp!=nullptr)
    {
        cout<<temp->num<<endl;
        temp=temp->next;
    }
}

void del(Node* &head){
    Node* temp=head;
    while (temp!=nullptr)
    {
        Node* ct=temp->next;
        delete temp;
        temp=ct;
    }
    head=nullptr;
}

void fn(Node* head){
    Node* temp=head;
    if (head==nullptr){
        cout<<"Node empty"<<endl;
        return;
    }
    int sum=temp->num;
    while (temp!=nullptr)
    {
        if (sum<temp->num){
            sum=temp->num;
        }
        temp=temp->next;
    }
    cout<<sum<<endl;
}

void prres(Node* head){
    if (head==nullptr) return;
    prres(head->next);
    cout<<head->num<<endl;
}

int main(int argc, char* argv[]){
    // int* a;
    // int x=10;
    // a=&x;
    // *a=20;
    // cout<<sizeof(a)<<endl;
    // cout<<a<<endl;
    // cout<<*a<<endl;
    Node* head=nullptr;
    add_node(head,10);
    add_node(head,20);
    pr(head);
    fn(head);
    del(head);

    // print(1, 2, 3, 4, 0);
    // print(3, 8, 0);
    // cout << "Program: " << argv[0] <<endl;
    // cout << "Argument: " << argc-1 <<endl;
    // for(int i = 1; i < argc; ++i) {
    //     cout << i << ": " << argv[i] <<endl;
    // }
}
