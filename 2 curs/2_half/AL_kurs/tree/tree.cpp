#include "tree.h"
#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

Tree::Tree() {}

Tree::~Tree() {
    delete_tree(root);
}


int Tree::get_value_from_hash(const std::string& hash) {
    std::string s = hash;
    s.erase(std::remove(s.begin(), s.end(), '.'), s.end());
    return std::stoi(s);
}

Tree::List* Tree::create_node(Book* book) {
    return new List{0, get_value_from_hash(book->hash), book, nullptr, nullptr, nullptr};
}

int Tree::get_balance(List* node) {
    if (!node) return 0;
    int l = node->left ? node->left->high : -1;
    int r = node->right ? node->right->high : -1;
    return l - r;
}

void Tree::update_high(List* node) {
    if (!node) return;
    int l = node->left ? node->left->high : -1;
    int r = node->right ? node->right->high : -1;
    node->high = max(l, r) + 1;
}

void Tree::rotateLeft(List* node) {
    List* r = node->right;
    node->right = r->left;

    if (r->left) r->left->parent = node;

    r->parent = node->parent;

    if (!node->parent) root = r;
    else if (node == node->parent->left) node->parent->left = r;
    else node->parent->right = r;

    r->left = node;
    node->parent = r;

    update_high(node);
    update_high(r);
}

void Tree::rotateRight(List* node) {
    List* l = node->left;
    node->left = l->right;

    if (l->right) l->right->parent = node;

    l->parent = node->parent;

    if (!node->parent) root = l;
    else if (node == node->parent->left) node->parent->left = l;
    else node->parent->right = l;

    l->right = node;
    node->parent = l;

    update_high(node);
    update_high(l);
}

void Tree::balance(List* node) {
    while (node) {
        update_high(node);
        int b = get_balance(node);

        if (b > 1) {
            if (get_balance(node->left) < 0)
                rotateLeft(node->left);
            rotateRight(node);
        }
        else if (b < -1) {
            if (get_balance(node->right) > 0)
                rotateRight(node->right);
            rotateLeft(node);
        }

        node = node->parent;
    }
}

Tree::List* Tree::insert(List* node, Book* book, List* parent) {
    if (!node) {
        List* n = create_node(book);
        n->parent = parent;
        return n;
    }

    int value = get_value_from_hash(book->hash);

    if (value < node->value)
        node->left = insert(node->left, book, node);
    else
        node->right = insert(node->right, book, node);

    return node;
}

void Tree::add(Book* book) {
    if (find_by_hash(book->hash) != nullptr) {
        cout << "Книга с таким шифром уже существует."<<endl;
        delete book;
        return;
    }

    root = insert(root, book, nullptr);
    List* cur = root;
    int value = get_value_from_hash(book->hash);

    while (cur) {
        if (value == cur->value) break;
        if (value < cur->value) cur = cur->left;
        else cur = cur->right;
    }

    balance(cur);
}

Tree::List* Tree::min_value(List* node) {
    while (node and node->left)
        node = node->left;
    return node;
}

Tree::List* Tree::remove(List* node, int value) {
    if (!node) return nullptr;

    if (value < node->value)
        node->left = remove(node->left, value);
    else if (value > node->value)
        node->right = remove(node->right, value);
    else {
        if (!node->left || !node->right) {
            List* temp = node->left ? node->left : node->right;

            if (!temp) {
                delete node;
                return nullptr;
            } else {
                temp->parent = node->parent;
                delete node;
                return temp;
            }
        }

        List* temp = min_value(node->right);
        node->value = temp->value;
        node->book = temp->book;

        node->right = remove(node->right, temp->value);
    }

    balance(node);
    return node;
}

void Tree::remove_by_value(int value) {
    root = remove(root, value);
}

void Tree::inorder(List* node) {
    if (!node) return;
    inorder(node->left);
    cout << node->value << " ";
    inorder(node->right);
}


void Tree::delete_tree(List* node) {
    if (!node) return;
    delete_tree(node->left);
    delete_tree(node->right);
    delete node->book;
    delete node;
}


static bool boyer_moore(const std::string& text, const std::string& pattern) {
    if (pattern.empty()) return true;
    int m = pattern.length();
    int n = text.length();
    if (m > n) return false;

    int badchar[256];
    for (int i = 0; i < 256; i++) badchar[i] = -1;
    for (int i = 0; i < m; i++)
        badchar[(unsigned char)pattern[i]] = i;

    int s = 0;
    while (s <= n - m) {
        int j = m - 1;
        while (j >= 0 and pattern[j] == text[s + j]) j--;
        if (j < 0) return true;
        s += std::max(1, j - badchar[(unsigned char)text[s + j]]);
    }
    return false;
}


Book* Tree::find_by_hash(const std::string& hash) {
    int value;
    try {
        value = get_value_from_hash(hash);
    } catch (const std::exception&) {
        return nullptr;
    }

    List* cur = root;
    while (cur) {
        if (value == cur->value) return cur->book;
        if (value < cur->value) cur = cur->left;
        else cur = cur->right;
    }
    return nullptr;
}

void Tree::print_all_books() {
    inorder_print(root);
    cout << endl;
}

void Tree::inorder_print(List* node) {
    if (!node) return;
    inorder_print(node->left);
    cout << "Шифр: " << node->book->hash 
        << " | Автор: " << node->book->author 
        << " | Название: " << node->book->name 
        << " | В наличии: " << node->book->assetExample << endl;
    inorder_print(node->right);
}

void Tree::clear() {
    delete_tree(root);
    root = nullptr;
    cout << "АВЛ-дерево книг очищено."<<endl;
}

void Tree::search_by_fragment(const std::string& fragment) {
    search_fragment_inorder(root, fragment);
}

void Tree::search_fragment_inorder(List* node, const std::string& fragment) {
    if (!node) return;
    search_fragment_inorder(node->left, fragment);

    if (boyer_moore(node->book->author, fragment) or
        boyer_moore(node->book->name, fragment)) {
        cout << "Найдено → Шифр: " << node->book->hash 
            << " | Автор: " << node->book->author 
            << " | Название: " << node->book->name 
            << " | Изд-во: " << node->book->publishing 
            << " | В наличии: " << node->book->assetExample << endl;
    }

    search_fragment_inorder(node->right, fragment);
}

void Tree::remove_by_hash(const std::string& hash) {
    Book* removed_book = find_by_hash(hash);
    if (removed_book == nullptr) {
        cout << "Книга с таким шифром не найдена."<<endl;
        return;
    }
    remove_by_value(get_value_from_hash(hash));
    delete removed_book;
    cout << "Книга успешно удалена из АВЛ-дерева."<<endl;
}
