#include <iostream>

using namespace std;


// variant 7

struct Tree{
    int high;
    int value;
    Tree* parent;
    Tree* left;
    Tree* right;
};

bool select(Tree* list, int select_value, Tree*& select_list){
    if (list == nullptr) {
        select_list = list;
        return 0;
    }
    else if (list->value==select_value){
        select_list = list;
        return 1;
    }
    else if (list->value<select_value){
        return select(list->right, select_value,select_list);
    }
    return select(list->left, select_value,select_list);
}

void rotateLeft(Tree*& list){
    Tree* sup = list;
    list->right->parent=nullptr;
    sup->right = list->right->left;
    sup->parent = list->right;
    sup->right->parent = sup;
    list->right->left = sup;
    delete list;
    list = sup->parent;
}

void rotateRight(Tree*& list){
    Tree* sup = list;
    list->left->parent=nullptr;
    sup->left = list->left->right;
    sup->parent = list->left;
    sup->left->parent = sup;
    list->left->right = sup;
    delete list;
    list = sup->parent;
    return;
}

int get_balance(Tree* list){
    int left_bal = (list->left==nullptr ? -1:list->left->high);
    int right_bal = (list->right==nullptr ? -1:list->right->high);
    int result = left_bal - right_bal;
}

void balance(Tree*& list){
    if(list==nullptr){
        return;
    int result = get_balance(list);
    if (result==1){
        rotateLeft(list);
        rotateRight(list);
    }
    if (result==-1){
        rotateRight(list);
        rotateLeft(list);
    }
    if (abs(result) < 2) {
        return;
    }
    if (result > 0) {
        rotateLeft(list);
        return;
    }
    rotateRight(list);
}

void update_high(Tree* node){
    node->high=+1;
    if(node->parent==nullptr){
        return;
    }
    return update_high(node->parent);
}

void add_list(Tree*& list,int value){
    Tree* select_list;
    if (select(list, value, select_list))return;

    Tree* new_list = new Tree;
    new_list->value = value;
    new_list->high = 0;
    new_list->parent = select_list;
    new_list->left = nullptr;
    new_list->right = nullptr;

    if(select_list->right==nullptr and select_list->left==nullptr){
        update_high(select_list);
        balance(list);
    }

    if (select_list->value>value){
        select_list->left = new_list;
    }
    else if (select_list->value<value){
        select_list->right = new_list;
    }
}



int main(){

}