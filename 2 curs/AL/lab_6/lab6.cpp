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

void update_high(Tree* node){
    node->high=max((node->left==nullptr ? -1:node->left->high),(node->right==nullptr ? -1:node->right->high))+1;
    if(node->parent==nullptr){
        return;
    }
    return update_high(node->parent);
}

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






void rotateLeft(Tree*& list){
    Tree* sup = list;
    list->right->parent=nullptr;
    sup->right = list->right->left;
    sup->parent = list->right;
    sup->right->parent = sup;
    list->right->left = sup;
    list = sup->parent;
    update_high(sup);
}

void rotateRight(Tree*& list){
    Tree* sup = list;
    list->left->parent=nullptr;
    sup->left = list->left->right;
    sup->parent = list->left;
    sup->left->parent = sup;
    list->left->right = sup;
    list = sup->parent;
    update_high(sup);
}

int get_balance(Tree* list){
    if (list == nullptr) return 0;
    int left_bal = (list->left==nullptr ? -1:list->left->high);
    int right_bal = (list->right==nullptr ? -1:list->right->high);
    int result = left_bal - right_bal;
    return result;
}

void balance(Tree*& list){
    if(list==nullptr){
        return;
    }
    int result = get_balance(list);
    if (result==1){
        rotateLeft(list->left);
        rotateRight(list);
    }
    if (result==-1){
        rotateRight(list->right);
        rotateLeft(list);
    }
    if (abs(result) < 2) {
        return;
    }
    if (result > 0) {
        rotateLeft(list);
        return;
    }
    if (result < 0) {
        rotateRight(list);
    };
    return;
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
    balance(list);

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
    if (node == nullptr) return;

    Tree* parent = node->parent;

    if (node->left == nullptr && node->right == nullptr) {

        if (parent) {
            if (parent->left == node) parent->left = nullptr;
            else parent->right = nullptr;
        } else {
            root = nullptr;
        }

        delete node;

        if (parent) {
            update_high(parent);
            balance(parent);
        }

        return;
    }

    if (node->left == nullptr || node->right == nullptr) {

        Tree* child = (node->left ? node->left : node->right);
        child->parent = parent;

        if (parent) {
            if (parent->left == node) parent->left = child;
            else parent->right = child;
        } else {
            root = child;
        }

        delete node;

        if (parent) {
            update_high(parent);
            balance(parent);
        } else {
            update_high(child);
            balance(child);
        }

        return;
    }
    Tree* min_right = min_value(node->right);

    node->value = min_right->value;

    delete_list(root, min_right);
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
