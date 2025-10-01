//variant 7

#include <iostream>

using namespace std;

struct Stack
{
    int num;
    int tip;
    int tact;
    Stack* next;
};

struct Processor{
    int num;
    int tact;
};


bool add_queue(int queue[][3],int n,int& head,int& tail,int& num,int tip,int& count,int tact){
    if (count==n)return 1;
    queue[tail][0]=num;
    queue[tail][1]=tip;
    queue[tail][2]=tact;
    tail=(tail+1)%n;
    num++;
    count++;
    return 0;
}

bool dell_queue(int queue[][3],int n,int& head,int& count){
    if(count==0)return 1;
    queue[head][0]=0;
    queue[head][1]=0;
    queue[head][2]=0;
    head=(head+1)%n;
    count--;
    return 0;
}

void show_queue(int queue[][3],int n,int& head,int& count){
    if (count==0)return;
    int d_head=head;
    for (int i=0;i<count;i++){
        for (int j=0;j<3;j++){
            cout<<queue[d_head][j]<<" ";
        }
        cout<<endl;
        d_head=(d_head+1)%n;
    }
}

void add_stack(Stack*&head,int num, int tip, int tact){
    if(head==nullptr){
        head=new Stack;
        head->next=nullptr;
    }
    else{
        Stack* prev=head;
        head=new Stack;
        head->next=prev;
    }
    head->num=num;
    head->tip=tip;
    head->tact=tact;
    return;
}

void dell_stack(Stack*&head){
    Stack* next=head;
    head=head->next;
    delete next;
}

void dell_all_stack(Stack*&head){
    if (head==nullptr)return;
    while (head!=nullptr)
    {
        Stack* next=head;
        head=head->next;
        delete next;
    }
}

void show_stack(Stack*head){
    Stack*current=head;
    while (current!=nullptr)
    {
        cout<<current->num<<" "<<current->tip<<" "<<current->tact<<endl;
        current=current->next;
    }
    
}

void process_tact(Processor* P1,Processor* P2,Processor* P3){ 
    if(P1->tact>0) P1->tact--;
    if(P2->tact>0) P2->tact--;
    if(P3->tact>0) P3->tact--;
}

void add_process_queue(Processor* P1,Processor* P2,Processor* P3,int queue[][3],int n,int& head,int&count,Stack*&   head_stack){
    if (count==0)return;
    switch (queue[head][1])
    {
    case 1:
        if(P1->tact!=0){
            add_stack(head_stack,queue[head][0],queue[head][1],queue[head][2]);
            dell_queue(queue,n,head,count);
            return;
        }
        P1->num=queue[head][0];
        P1->tact=queue[head][2];
        dell_queue(queue,n,head,count);
        break;
    case 2:
        if(P2->tact!=0){
            add_stack(head_stack,queue[head][0],queue[head][1],queue[head][2]);
            dell_queue(queue,n,head,count);
            return;
        }
        P2->num=queue[head][0];
        P2->tact=queue[head][2];
        dell_queue(queue,n,head,count);
        break;
    case 3:
        if(P3->tact!=0){
            add_stack(head_stack,queue[head][0],queue[head][1],queue[head][2]);
            dell_queue(queue,n,head,count);
            return;
        }
        P3->num=queue[head][0];
        P3->tact=queue[head][2];
        dell_queue(queue,n,head,count);
        break;
    default:
        add_stack(head_stack,queue[head][0],queue[head][1],queue[head][2]);
        dell_queue(queue,n,head,count);
        break;
    }
}

void add_process_stack(Processor* P1,Processor* P2,Processor* P3,Stack*&head){
    if (head==nullptr)return;
    switch (head->tip)
    {
    case 1:
        if(P1->tact!=0)return;
        P1->num=head->num;
        P1->tact=head->tact;
        dell_stack(head);
        break;
    case 2:
        if(P2->tact!=0)return;
        P2->num=head->num;
        P2->tact=head->tact;
        dell_stack(head);
        break;
    case 3:
        if(P3->tact!=0)return;
        P3->num=head->num;
        P3->tact=head->tact;
        dell_stack(head);
        break;
    default:
        break;
    }
    
}

void show_process(Processor* P1,Processor* P2,Processor* P3){
    cout<<P1->num<<" "<<P1->tact<<endl;
    cout<<P2->num<<" "<<P2->tact<<endl;
    cout<<P3->num<<" "<<P3->tact<<endl; 
}


int main(){
    int head=0,tail=0,count=0,num=1,tip=0,tact=0;
    int const n=5;
    int queue[n][3];
    Stack* stack_head=nullptr;
    Processor* P1=new Processor;
    Processor* P2=new Processor;
    Processor* P3=new Processor;
    bool flag=1;
    while (flag)
    {   
        int i;
        cout<<"******************"<<endl;
        cout<<"1 - Exit \n2 - Add Task \n3 - Next tact \n4 - Show Stack \n5 - Show Queue \n6 - Show Processor"<<endl;
        cin>>i;
        switch (i)
        {
        case 1:
            flag=false;
            break;
        case 2:
            cout<<"Enter tip:";
            cin>>tip;
            cout<<"Enter tact:";
            cin>>tact;
            add_queue(queue,n,head,tail,num,tip,count,tact);
            break;
        case 3:
            process_tact(P1,P2,P3);
            add_process_stack(P1,P2,P3,stack_head);
            add_process_queue(P1,P2,P3,queue,n,head,count,stack_head);
            break;
        case 4:
            show_stack(stack_head);
            break;
        case 5:
            show_queue(queue,n,head,count);
            break;
        case 6:
            show_process(P1,P2,P3);
            break;
        default:
            break;
        }
    }
    dell_all_stack(stack_head);
    delete P1;
    delete P2;
    delete P3;
}