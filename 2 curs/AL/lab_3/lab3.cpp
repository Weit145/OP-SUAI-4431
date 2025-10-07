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
    head=(head+1)%n;
    count--;
    return 0;
}

void show_queue(int queue[][3],int n,int& head,int& count){
    if (count==0){
        cout << "Queue is empty" << endl;
        return;
    }
    int d_head=head;
    for (int i=0;i<count;i++){
        cout<<"Task "<<queue[d_head][0]<<" type "<<queue[d_head][1]<<" tacts "<<queue[d_head][2]<<endl;
        d_head=(d_head+1)%n;
    }
}

void add_stack(Stack*&head,int num, int tip, int tact){
    Stack* new_node = new Stack;
    new_node->num=num;
    new_node->tip=tip;
    new_node->tact=tact;
    new_node->next=head;
    head=new_node;
}

void dell_stack(Stack*&head){
    if(head==nullptr) return;
    Stack* next=head->next;
    delete head;
    head=next;
}

void dell_all_stack(Stack*&head){
    while (head!=nullptr)
    {
        Stack* next=head->next;
        delete head;
        head=next;
    }
}

void show_stack(Stack*head){
    if(head==nullptr){
        cout << "Stack is empty" << endl;
        return;
    }
    Stack*current=head;
    while (current!=nullptr)
    {
        cout<<"Task "<<current->num<<" type "<<current->tip<<" tacts "<<current->tact<<endl;
        current=current->next;
    }
}

void process_tact(Processor* P1,Processor* P2,Processor* P3){ 
    if(P1->tact>0) P1->tact--;
    if(P2->tact>0) P2->tact--;
    if(P3->tact>0) P3->tact--;
}

void add_process_queue(Processor* P1,Processor* P2,Processor* P3,int queue[][3],int n,int& head,int& count,Stack*& head_stack){
    if (count==0) return;
    int current_head=head;
    for(int i=0;i<count;i++){
        switch (queue[current_head][1]){
        case 1:
            if(P1->tact==0){
                P1->num=queue[current_head][0];
                P1->tact=queue[current_head][2];
                cout<<"Task "<<queue[current_head][0]<<" to P1"<<endl;
                head=(head+1)%n;
                count--;
                return;
            }
            break;
        case 2:
            if(P2->tact==0){
                P2->num=queue[current_head][0];
                P2->tact=queue[current_head][2];
                cout<<"Task "<<queue[current_head][0]<<" to P2"<<endl;
                head=(head+1)%n;
                count--;
                return;
            }
            break;
        case 3:
            if(P3->tact==0){
                P3->num=queue[current_head][0];
                P3->tact=queue[current_head][2];
                cout<<"Task "<<queue[current_head][0]<<" to P3"<<endl;
                head=(head+1)%n;
                count--;
                return;
            }
            break;
        }
        current_head=(current_head+1)%n;
    }
}

void add_process_stack(Processor* P1,Processor* P2,Processor* P3,Stack*& head){
    if (head==nullptr)return;
    Stack* current=head;
    Stack* prev=nullptr;
    while(current!=nullptr){
        bool assigned=false;
        switch (current->tip){
        case 1:
            if(P1->tact==0){
                P1->num=current->num;
                P1->tact=current->tact;
                cout<<"Task "<<current->num<<" from stack to P1"<<endl;
                assigned=true;
            }
            break;
        case 2:
            if(P2->tact==0){
                P2->num=current->num;
                P2->tact=current->tact;
                cout<<"Task "<<current->num<<" from stack to P2"<<endl;
                assigned=true;
            }
            break;
        case 3:
            if(P3->tact==0){
                P3->num=current->num;
                P3->tact=current->tact;
                cout<<"Task "<<current->num<<" from stack to P3"<<endl;
                assigned=true;
            }
            break;
        }
        
        if(assigned){
            if(prev==nullptr){
                Stack* temp=head;
                head=head->next;
                delete temp;
            } 
            else{
                prev->next=current->next;
                delete current;
            }
            return;
        }
        
        prev=current;
        current=current->next;
    }
}

void show_process(Processor* P1,Processor* P2,Processor* P3){
    cout<<"P1: ";
    if (P1->tact>0){
        cout<<"Busy with task "<<P1->num<<" tacts "<<P1->tact<<endl;
    }
    cout<<"P1: ";
    if (P2->tact>0){
        cout<<"Busy with task "<<P2->num<<" tacts "<<P2->tact<<endl;
    }
    cout<<"P1: ";
    if (P3->tact>0){
        cout<<"Busy with task "<<P3->num<<" tacts "<<P3->tact<<endl;
    }
}

void check_and_move_to_stack(Processor* P1,Processor* P2,Processor* P3,int queue[][3],int n,int& head,int& count,Stack*& head_stack){
    if (count==0)return;
    switch (queue[head][1])
    {
    case 1:
        if(P1->tact!=0){
            cout<<"Task "<<queue[head][0]<<" to stack P1"<<endl;
            add_stack(head_stack, queue[head][0], queue[head][1], queue[head][2]);
            dell_queue(queue, n, head, count);
        }
        break;
    case 2:
        if(P2->tact!=0){
            cout<<"Task "<<queue[head][0]<<" to stack P2"<<endl;
            add_stack(head_stack, queue[head][0], queue[head][1], queue[head][2]);
            dell_queue(queue, n, head, count);
        }
        break;
    case 3:
        if(P3->tact!=0){
            cout<<"Task "<<queue[head][0]<<" to stack P3"<<endl;
            add_stack(head_stack, queue[head][0], queue[head][1], queue[head][2]);
            dell_queue(queue, n, head, count);
        }
        break;
    }
}

int main(){
    int head=0,tail=0,count=0,num=1,tip=0,tact=0;
    int const n=5;
    int queue[n][3] = {0};
    Stack* stack_head=nullptr;
    Processor* P1=new Processor{0, 0};
    Processor* P2=new Processor{0, 0};
    Processor* P3=new Processor{0, 0};
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
            cout<<"Enter type (1-3):";
            cin>>tip;
            if (tip<1 or tip>3){
                cout<<"Error: type must be 1, 2 or 3"<<endl;
                break;
            }
            cout<<"Enter tacts:";
            cin>>tact;
            if (tact <= 0){
                cout<<"Error: tacts must be positive"<<endl;
                break;
            }
            if (add_queue(queue,n,head,tail,num,tip,count,tact)){
                cout<<"Error: queue is full"<<endl;
            }
            break;
        case 3:
            process_tact(P1,P2,P3);
            check_and_move_to_stack(P1,P2,P3,queue,n,head,count,stack_head);
            add_process_queue(P1,P2,P3,queue,n,head,count,stack_head);
            add_process_stack(P1,P2,P3,stack_head);
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
            cout<<"Error"<<endl;
            break;
        }
    }
    dell_all_stack(stack_head);
    delete P1;
    delete P2;
    delete P3;
}