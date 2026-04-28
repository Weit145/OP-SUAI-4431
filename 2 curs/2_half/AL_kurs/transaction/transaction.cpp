#include "transaction.h"
#include "../domain/domain.h"
#include <iostream>
#include <string>

using namespace std;

TransactionList::~TransactionList() {
    if (!head) return;

    Node* current = head->next;
    while (current != head) {
        Node* next = current->next;
        delete current->transaction;
        delete current;
        current = next;
    }

    delete head->transaction;
    delete head;
    head = nullptr;
}

void TransactionList::add(TransactionBook* t) {
    Node* newNode = new Node{t, nullptr};
    if (!head) {
        head = newNode;
        head->next = head;
        return;
    }
    Node* temp = head;
    while (temp->next != head) temp = temp->next;
    temp->next = newNode;
    newNode->next = head;

    selection_sort();
}

void TransactionList::remove(TransactionBook* t) {
    if (!head) return;
    Node* prev = nullptr;
    Node* curr = head;
    do {
        if (curr->transaction == t) {
            if (curr->next == curr) { 
                delete curr->transaction;
                delete curr;
                head = nullptr;
                return;
            }
            if (prev == nullptr) { 
                Node* last = head;
                while (last->next != head) last = last->next;
                head = curr->next;
                last->next = head;
            } else {
                prev->next = curr->next;
            }
            delete curr->transaction;
            delete curr;
            return;
        }
        prev = curr;
        curr = curr->next;
    } while (curr != head);
}

void TransactionList::show() {
    if (!head) {
        cout << "Список транзакций пуст."<<endl;
        return;
    }
    Node* temp = head;
    do {
        TransactionBook* t = temp->transaction;
        cout << "Билет: " << t->numberTicket 
            << " | Шифр: " << t->hash
            << " | Выдача: " << t->dataEnter
            << " | Возврат: " << (t->dataOut.empty() ? "не возвращена" : t->dataOut) << endl;
        temp = temp->next;
    } while (temp != head);
}

void TransactionList::selection_sort() {
    if (!head || head->next == head) return;
    Node* last = head;
    while (last->next != head) last = last->next;
    last->next = nullptr;

    Node* sorted_head = nullptr;
    Node* sorted_tail = nullptr;

    while (head) {
        Node* min_prev = nullptr;
        Node* min_node = head;
        Node* prev = nullptr;
        Node* curr = head;
        while (curr) {
            if (curr->transaction->hash < min_node->transaction->hash) {
                min_node = curr;
                min_prev = prev;
            }
            prev = curr;
            curr = curr->next;
        }
        if (min_prev == nullptr) head = min_node->next;
        else min_prev->next = min_node->next;

        min_node->next = nullptr;

        if (!sorted_head) {
            sorted_head = sorted_tail = min_node;
        } else {
            sorted_tail->next = min_node;
            sorted_tail = min_node;
        }
    }

    head = sorted_head;

    if (head) {
        Node* new_last = head;
        while (new_last->next) new_last = new_last->next;
        new_last->next = head;
    }
}

bool TransactionList::issue(const string& ticket, const string& hash, const string& dataEnter) {
    Node* temp = head;
    if (temp) {
        do {
            if (temp->transaction->numberTicket == ticket and
                temp->transaction->hash == hash and
                temp->transaction->dataOut.empty()) {
                cout << "Книга уже выдана этому читателю!"<<endl;
                return false;
            }
            temp = temp->next;
        } while (temp != head);
    }

    TransactionBook* t = new TransactionBook(ticket, hash, dataEnter, "");
    add(t);
    cout << "Книга успешно выдана."<<endl;
    return true;
}

bool TransactionList::return_book(const std::string& ticket, const std::string& hash, const std::string& dataOut) {
    if (!head) return false;
    Node* temp = head;
    do {
        if (temp->transaction->numberTicket == ticket and
            temp->transaction->hash == hash and
            temp->transaction->dataOut.empty()) {
            temp->transaction->dataOut = dataOut;
            cout << "Книга успешно возвращена."<<endl;
            return true;
        }
        temp = temp->next;
    } while (temp != head);
    cout << "Активная выдача не найдена!"<<endl;
    return false;
}

void TransactionList::print_for_reader(const std::string& ticket) {
    if (!head) { cout << "У читателя нет выданных книг."<<endl; return; }
    bool found = false;
    Node* temp = head;
    do {
        if (temp->transaction->numberTicket == ticket) {
            cout << "Шифр: " << temp->transaction->hash 
                << " | Выдача: " << temp->transaction->dataEnter
                << " | Возврат: " << (temp->transaction->dataOut.empty() ? "не возвращена" : temp->transaction->dataOut) << endl;
            found = true;
        }
        temp = temp->next;
    } while (temp != head);
    if (!found) cout << "У читателя нет выданных книг."<<endl;
}

void TransactionList::print_for_book(const std::string& hash) {
    if (!head) { cout << "Книга никому не выдана."<<endl; return; }
    bool found = false;
    Node* temp = head;
    do {
        if (temp->transaction->hash == hash) {
            cout << "Билет: " << temp->transaction->numberTicket 
                << " | Выдача: " << temp->transaction->dataEnter
                << " | Возврат: " << (temp->transaction->dataOut.empty() ? "не возвращена" : temp->transaction->dataOut) << endl;
            found = true;
        }
        temp = temp->next;
    } while (temp != head);
    if (!found) cout << "Книга никому не выдана."<<endl;
}

bool TransactionList::has_active_issuances() {
    if (!head) return false;
    Node* temp = head;
    do {
        if (temp->transaction->dataOut.empty())
            return true;
        temp = temp->next;
    } while (temp != head);
    return false;
}

bool TransactionList::has_active_issuances(const std::string& ticket) {
    if (!head) return false;
    Node* temp = head;
    do {
        if (temp->transaction->numberTicket == ticket and temp->transaction->dataOut.empty())
            return true;
        temp = temp->next;
    } while (temp != head);
    return false;
}

bool TransactionList::has_active_issuances_for_book(const std::string& hash) {
    if (!head) return false;
    Node* temp = head;
    do {
        if (temp->transaction->hash == hash and temp->transaction->dataOut.empty())
            return true;
        temp = temp->next;
    } while (temp != head);
    return false;
}
