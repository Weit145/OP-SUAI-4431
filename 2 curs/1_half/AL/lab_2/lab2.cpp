#include <iostream>

using namespace std;

struct Node{
    int number;
    Node* next;
    Node* prev;
};

void add_node(Node*& head, Node*& tail,int num,int& n){
    Node* node = head;
    while (node != nullptr) {
        if (node->number == num){
            cout<<"Node have this number"<<endl;
            return;
        } 
        node = node->next;
    }
    if (head==nullptr){
        head=new Node;
        head->next=nullptr;
        head->prev=nullptr;
        tail=head;
        head->number=num;
        n=1;
        return;
    }

    Node* new_node =new Node;
    tail->next=new_node;
    new_node->prev=tail;
    tail=new_node;
    new_node->number=num;
    n++;
}


void delete_node(Node*&head, Node*&tail, int index,int& n){
    if (head==nullptr || index>n || index<1){
        cout<<"Error"<<endl;
        return;
    }
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
        if(index<=n/2){
            node=head;
            for(int i=1;i<index;i++){
                node=node->next;
            }
        }
        else{
            node=tail;
            for(int i=n;i>index;i--){
                node=node->prev;
            }
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
    if (index<1||index>n+1){
        cout<<"Problem whith index"<<endl;
        return;
    }
    Node* node = head;
    while (node != nullptr) {
        if (node->number == num) return;
        node = node->next;
    }
    if(index==n+1){add_node(head,tail,num,n);return;}
    if(index==1){
        Node* new_node=new Node;
        new_node->number=num;
        new_node->prev=nullptr;
        new_node->next=head;
        if(head!=nullptr){
            head->prev=new_node;
        }
        head=new_node;
        if(n==0){
            tail=head;
        }
        n++;
        return;
    }
    if(index<=n/2){
        node=head;
        for(int i=1;i<index;i++){
            node=node->next;
        }
    }
    else{
        node=tail;
        for(int i=n;i>index;i--){
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


void delete_all(Node*&head,Node*&tail,int&n){
    Node* node=head;
    while (node!=nullptr){
        head=node->next;
        delete node;
        node=head;
    }
    tail=nullptr;
    n=0;
}

void show_all(Node*head){
    Node* node=head;
    cout<<"******************"<<endl;
    for(int i=0;node!=nullptr;i++){
        cout<<"Node "<<i+1<<"\n"<<"Number: "<<node->number<<endl;
        node=node->next;
        if(node!=nullptr)cout<<"\t|"<<endl;
    }
    cout<<"******************"<<endl;
}


void zad(Node*&head,Node*&tail,int&n){  
    if(n<=4){
        cout<<"Error: N<=4"<<endl;
        return;
    }
    for (int k=0;k<2;k++){
        Node* next_node=head;
        Node* min=head;
        while(next_node!=nullptr){
            if (next_node->number<min->number){
                min=next_node;
            }
            next_node=next_node->next;
        }
        if(min->prev){
            min->prev->next=min->next;
        }
        if(min->next){
            min->next->prev=min->prev;
        }
        if (min==head){
            head=min->next;
        }
        if(min==tail){
            tail=min->prev;
        }
        delete min;
        n--;

        next_node=head;
        Node*max=head;
        while(next_node!=nullptr){
            if(next_node->number>max->number){
                max=next_node;
            }
            next_node=next_node->next;
        }
        if(max->prev){
            max->prev->next=max->next;
        }
        if(max->next){
            max->next->prev=max->prev;
        }
        if (max==head){
            head=max->next;
        }
        if(max==tail){
            tail=max->prev;
        }
        delete max;
        n--;
    }
}



int main(){
    Node* head=nullptr;
    Node* tail= nullptr;
    int n=0,num=0,index=0,i=0;
    cout<<"******************"<<endl;
    cout<<"1 - Exit \n2 - Add Node \n3 - Delete one Node \n4 - Enter Node \n5 - Delete ALL Nodes \n6 - Show Nodes \n7 - Zadanie"<<endl;
    cin>>i;
    while (i!=1)
    {
        switch (i){
        case 2:
            cout<<"Number = ";
            cin>>num;
            add_node(head,tail,num,n);
            break;
        case 3:
            cout<<"Size Node: "<<n<<endl;
            cout<<"Index = ";
            cin>>index;
            delete_node(head,tail,index,n);
            break;
        case 4:
            cout<<"Size Node: "<<n<<endl;
            cout<<"Number = ";
            cin>>num;
            cout<<"Index = ";
            cin>>index;
            enter_node(head,tail,index,num,n);
            break;
        case 5:
            delete_all(head,tail,n);
            break;
        case 6:
            show_all(head);
            break;
        case 7:
            zad(head,tail,n);
            break;
        default:
            break;
        }
        cout<<"******************"<<endl;
        cout<<"1 - Exit \n2 - Add Node \n3 - Delete one Node \n4 - Enter Node \n5 - Delete ALL Nodes \n6 - Show Nodes \n7 - Zadanie"<<endl;
        cin>>i;
    }
    delete_all(head,tail,n);



    // Node* head=nullptr;
    // Node* tail=nullptr;
    // int n=0;
    // for (int i=0;i<4;i++){
    //     add_node(head,tail,i,n);
    // }
    // show_all(head);
    // delete_node(head,tail,n,n);
    // show_all(head);
    // delete_all(head,tail);
}