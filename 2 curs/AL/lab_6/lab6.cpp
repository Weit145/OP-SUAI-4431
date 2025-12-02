#include <iostream>
#include <algorithm>
#include <cmath>

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
        return 0;
    }
    else if (list->value==select_value){
        select_list = list;
        return 1;
    }
    else if (list->value<select_value and list->right!=nullptr){
        return select(list->right, select_value,select_list);
    }
    else if (list->value>select_value and list->left!=nullptr){
        return select(list->left, select_value,select_list);
    }
    select_list = list;
    return 0;
}



void update_high(Tree* node){
    if (!node) return;

    node->high = std::max(
        node->left ? node->left->high : -1,
        node->right ? node->right->high : -1
    ) + 1;

    if (node->parent != nullptr) {
        update_high(node->parent);
    }
}


void rotateLeft(Tree*& root, Tree* x) {
    if (!x || !x->right) return;
    
    Tree* y = x->right;
    x->right = y->left;
    
    if (y->left)
        y->left->parent = x;
    
    y->parent = x->parent;
    
    if (!x->parent) {
        root = y;
    }
    else if (x == x->parent->left) {
        x->parent->left = y;
    }
    else {
        x->parent->right = y;
    }
    
    y->left = x;
    x->parent = y;
    
    update_high(x);
    update_high(y);
}

void rotateRight(Tree*& root, Tree* y) {
    if (!y || !y->left) return;
    
    Tree* x = y->left;
    y->left = x->right;
    
    if (x->right)
        x->right->parent = y;
    
    x->parent = y->parent;
    
    if (!y->parent) {
        root = x;
    }
    else if (y == y->parent->left) {
        y->parent->left = x;
    }
    else {
        y->parent->right = x;
    }
    
    x->right = y;
    y->parent = x;
    
    update_high(y);
    update_high(x);
}

int get_balance(Tree* list){
    if (list == nullptr) return 0;
    int left_bal = (list->left==nullptr ? -1:list->left->high);
    int right_bal = (list->right==nullptr ? -1:list->right->high);
    int result = left_bal - right_bal;
    return result;
}

void balance(Tree*& root, Tree* node) {
    if (!node) return;

    while (node) {
        update_high(node);
        int bf = get_balance(node);

        if (bf > 1) {
            if (get_balance(node->left) >= 0) {
                rotateRight(root, node);
            }
            else {
                rotateLeft(root, node->left);
                rotateRight(root, node);
            }
        }
        else if (bf < -1) {
            if (get_balance(node->right) <= 0) {
                rotateLeft(root, node);
            }
            else {
                rotateRight(root, node->right);
                rotateLeft(root, node);
            }
        }
        
        node = node->parent;
    }
}


void add_list(Tree*& list,int value){
    Tree* select_list;
    if (list==nullptr){
        list = new Tree;
        list->value = value;
        list->high = 0;
        list->parent = nullptr;
        list->left = nullptr;
        list->right = nullptr;
        return;
    }
    if (select(list, value, select_list))return;

    Tree* new_list = new Tree;
    new_list->value = value;
    new_list->high = 0;
    new_list->parent = select_list;
    new_list->left = nullptr;
    new_list->right = nullptr;

    if (select_list->value>value){
        select_list->left = new_list;
    }
    else if (select_list->value<value){
        select_list->right = new_list;
    }

    update_high(select_list);
    balance(list, new_list);

}

Tree* min_value(Tree* list){
    if (list==nullptr){
        return nullptr;
    }
    if (list->left==nullptr){
        return list;
    }
    return min_value(list->left);
}

void delete_list(Tree*& root, Tree* node) {
    if (!node) return;

    Tree* parent = node->parent;

    // Случай 1: Узел без детей
    if (!node->left && !node->right) {
        if (parent) {
            if (parent->left == node)
                parent->left = nullptr;
            else
                parent->right = nullptr;
            
            delete node;
            update_high(parent);
            balance(root, parent);
        }
        else {
            delete node;
            root = nullptr;
        }
        return;
    }

    // Случай 2: Узел с одним ребенком
    if (!node->left || !node->right) {
        Tree* child = node->left ? node->left : node->right;
        
        if (parent) {
            if (parent->left == node)
                parent->left = child;
            else
                parent->right = child;
            
            child->parent = parent;
            delete node;
            update_high(child);
            balance(root, child);
        }
        else {
            root = child;
            child->parent = nullptr;
            delete node;
            update_high(root);
            balance(root, root);
        }
        return;
    }

    // Случай 3: Узел с двумя детьми
    Tree* successor = min_value(node->right);
    node->value = successor->value;
    delete_list(root, successor);
}

void delete_tree(Tree* list){
    if (list==nullptr){
        return;
    }
    delete_tree(list->left);
    delete_tree(list->right);
    delete list;
}



void show(Tree* node, string indent = "", bool last = true) {
    if (node == nullptr) return;

    cout << indent;

    if (last) {
        cout << "└─";
        indent += "  ";
    } else {
        cout << "├─";
        indent += "│ ";
    }

    cout << node->value << "\n";

    if (node->left || node->right) {
        if (node->left) show(node->left, indent, false);
        if (node->right) show(node->right, indent, true);
    }
}


int arifm(Tree* list, int& sum, int& count){
    if (!list) return 0;

    if (!list->left && !list->right){
        sum += list->value;
        count++;
    }

    arifm(list->left, sum, count);
    arifm(list->right, sum, count);

    if (count == 0) return 0;
    if (list->parent == nullptr) return sum / count;
    return 0;
}


void vechet(Tree* list, int num){
    if (list==nullptr){
        return;
    }
    if (list->left==nullptr and list->right==nullptr){
        list->value-=num;
    }
    vechet(list->left,num);
    vechet(list->right,num);
    return;
}

void zad_del(Tree*& root, Tree* node){
    if (!node) return;
    zad_del(root, node->left);
    zad_del(root, node->right);

    if (node->value % 3 == 0) {
        delete_list(root, node);
    }
}


void zad(Tree*& root){
    if (!root) return;

    int sum = 0, count = 0;
    arifm(root, sum, count);

    int avg = (count > 0) ? sum / count : 0;

    vechet(root, avg);

    zad_del(root, root);
}


int main() {
    Tree* root = nullptr;
    int menu = 0;

    while (true) {
        cout << "\n===== MENU =====\n";
        cout << "1 - Exit\n";
        cout << "2 - Add\n";
        cout << "3 - Delete\n";
        cout << "4 - Show (in-order)\n";
        cout << "5 - Zad (арифм. ср + уменьшение листьев + удаление %3)\n";
        cout << "Enter: ";

        cin >> menu;

        if (menu == 1) {
            cout << "Exit.\n";
            break;
        }

        switch (menu) {

        case 2: {
            int x;
            cout << "Enter value to add: ";
            cin >> x;
            add_list(root, x);
            cout << "Added.\n";
            break;
        }

        case 3: {
            int x;
            cout << "Enter value to delete: ";
            cin >> x;

            Tree* to_del = nullptr;
            if (select(root, x, to_del) && to_del != nullptr) {
                delete_list(root, to_del);
                cout << "Deleted.\n";
            } else {
                cout << "Not found.\n";
            }
            break;
        }

        case 4:
            cout << "Tree (in-order): "<<endl;
            show(root);
            cout << endl;
            break;

        case 5:
            if (root == nullptr) {
                cout << "Tree empty.\n";
                break;
            }
            zad(root);
            cout << "Zad applied.\n";
            break;

        default:
            cout << "Wrong option.\n";
            break;
        }
    }

    delete_tree(root);
    return 0;
}
