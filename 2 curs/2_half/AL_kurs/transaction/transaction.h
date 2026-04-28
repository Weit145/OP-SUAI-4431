#include "../domain/domain.h"
#include <string>

#pragma once

using namespace std;

class TransactionList {
private:
    struct Node {
        TransactionBook* transaction;
        Node* next;
    };
    Node* head;

    
    void selection_sort();
    
public:
    TransactionList() : head(nullptr) {}
    ~TransactionList();
    void add(TransactionBook* t);
    void remove(TransactionBook* t);
    bool issue(const string&, const string&, const string&);
    void show();
    bool return_book(const string&, const string&, const string&);
    void print_for_reader(const string&);
    void print_for_book(const string&);
    bool has_active_issuances();
    bool has_active_issuances(const string&);
    bool has_active_issuances_for_book(const string&);
};
