#include <iostream>

using namespace std;

struct Node{
    int number;
    Node* next;
    Node* prev;
};

void add_node(Node*& head, Node*& tail,int num,int& n){
    if (head==nullptr){
        head=new Node;
        head->next=nullptr;
        head->prev=nullptr;
        tail=head;
        head->number=num;
        n=1;
        return;
    }


    Node* prov_node=head;
    while (prov_node!=nullptr){
        if(num==prov_node->number){return;}
        prov_node=prov_node->next;
    }

    Node* new_node =new Node;
    tail->next=new_node;
    new_node->prev=tail;
    tail=new_node;
    new_node->number=num;
    n++;
}


void delete_node(Node*&head, Node*&tail, int index,int& n){
    if (head==nullptr || index>n || index<1){return;}
    else if(n==1){
        delete head;
        head =nullptr;
        tail = nullptr;
        n--;
        return;
    }
    else if (index==1){
        Node* next_node=head;
        head=next_node->next;
        head->prev=nullptr;
        delete next_node;
        n--;
        return;
    }
    else if(index==n){
        Node* next_node=tail;
        tail=tail->prev;
        tail->next=nullptr;
        delete next_node;
        n--;
        return;
    }
    else{
        Node* node=head;
        for(int i=1;i<index;i++){
            node=node->next;
        }
        Node* prev_node=node->prev;
        Node* next_node=node->next;
        prev_node->next=next_node;
        next_node->prev=prev_node;
        delete node;
        n--;
        return;
    }
}

void enter_node(Node*& head,Node*&tail,int index,int num,int&n){
    Node* node;
    if (index<1||index>n+1){return;}
    else if(index==n+1){add_node(head,tail,num,n);return;}
    else if(index==1){
        Node* new_node=new Node;
        head->prev=new_node;
        new_node->next=head;
        head=new_node;
        new_node->number=num;
        new_node->prev=nullptr;
        n++;
        return;
    }
    if(n/2>index){
        node=head;
        for(int i=1;i<index;i++){
            node=node->next;
        }
    }
    else{
        node=tail;
        for(int i=n;i>=index;i--){
            node=node->prev;
        }
    }
    Node* prev_node=node->prev;
    Node* new_node=new Node;
    prev_node->next=new_node;
    node->prev=new_node;
    new_node->prev=prev_node;
    new_node->next=node;
    new_node->number=num;
    n++;
}


void delete_all(Node*&head,Node*&tail){
    Node* node=head;
    while (node!=nullptr)
    {
        head=node->next;
        delete node;
        node=head;
    }
    tail=nullptr;
}

void show_all(Node*head){
    Node* node=head;
    cout<<"******************"<<endl;
    for(int i=0;node!=nullptr;i++){
        cout<<"Node "<<i<<"\n"<<"Number: "<<node->number<<endl;
        node=node->next;
    }
    cout<<"******************"<<endl;
}




int main(){
    Node* head=nullptr;
    Node* tail=nullptr;
    int n=0;
    for (int i=0;i<4;i++){
        add_node(head,tail,i,n);
    }
    show_all(head);
    delete_node(head,tail,n,n);
    show_all(head);
    delete_all(head,tail);
}