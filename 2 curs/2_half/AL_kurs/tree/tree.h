#pragma once
#include "../domain/domain.h"
#include <string>

class Tree {
private:
    struct List {
        int high;
        int value;
        Book* book;
        List* parent;
        List* left;
        List* right;
    };

    List* root = nullptr;

    int get_value_from_hash(const std::string& hash);
    List* create_node(Book* book);
    List* insert(List* node, Book* book, List* parent);
    void rotateLeft(List* node);
    void rotateRight(List* node);
    void update_high(List* node);
    void balance(List* node);
    int get_balance(List* node);
    List* min_value(List* node);
    List* remove(List* node, int value);
    void inorder(List* node);
    void delete_tree(List* node);

    void inorder_print(List* node);
    void search_fragment_inorder(List* node, const std::string& fragment);

public:
    Tree();
    ~Tree();

    void add(Book* book);
    void remove_by_value(int value);

    Book* find_by_hash(const std::string& hash);
    void print_all_books();
    void clear();
    void search_by_fragment(const std::string& fragment);
    void remove_by_hash(const std::string& hash);
};