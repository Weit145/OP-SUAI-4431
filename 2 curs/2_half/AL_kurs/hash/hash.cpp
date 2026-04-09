#include <string>
#include <iostream>
#include "../domain/domain.h"
#include "hash.h"

Hash::Hash(){
    hash_table = new Segment[SIZE];
    for (int i = 0; i < SIZE; i++) {
        hash_table[i].reader = nullptr;
        hash_table[i].isEmpty = true;
        hash_table[i].isDeleted = false;
    }
}

Hash::~Hash() {
    for (int i = 0; i < SIZE; i++) {
        if (hash_table[i].reader != nullptr) {
            delete hash_table[i].reader;
        }
    }
    delete[] hash_table;
}

int Hash::hash_function(const string key) {
    int hash = 0;
    for (int i = 2; i < 9; i++) {
        if (i==6) continue;
        int digit = key[i] - '0';
        hash = (hash * 31 + digit) % SIZE;
    }
    return hash;
}

int Hash::step_function(const int hash, const int step) {
    return (hash + step * step) % SIZE;
}

void Hash::add_segment( const Reader* reader) {
    int hash = hash_function(reader->numberTicket);
    int step = 1;
    int start = hash;
    while (!hash_table[hash].isEmpty){
        if (!hash_table[hash].isDeleted and is_this_key(reader->numberTicket, hash_table[hash])) {
            cout << "Key already exists"<<endl;
            return;
        }
        if (hash_table[hash].isDeleted) {
            break;
        }
        hash = step_function(start, step);
        step++;
        if (step >= SIZE) {
            cout << "Hash table is full"<<endl;
            return;
        }   
    }
    hash_table[hash].reader = reader;
    hash_table[hash].isEmpty = false;
    hash_table[hash].isDeleted = false;
}

bool Hash::is_this_key(const string key, const Segment segment) {
    if (segment.reader == nullptr) return false;
    return segment.reader->numberTicket == key;
}

bool Hash::delete_segment(const Reader* reader) {
    int hash = hash_function(reader->numberTicket);
    int step = 1;
    int start = hash;
    while (!hash_table[hash].isEmpty) {
        if (!hash_table[hash].isDeleted and is_this_key(reader->numberTicket, hash_table[hash])) {
            hash_table[hash].isDeleted = true;
            return true;
        }
        hash = step_function(start, step);
        step++;
        if (step >= SIZE) {
            cout << "Not found segment for deletion."<<endl;
            return false;
        }
    }
    cout << "Not found segment for deletion."<<endl;
    return false;
}

void Hash::show_segment(int index) {
    if (!hash_table[index].isEmpty and !hash_table[index].isDeleted) {
        const Reader* reader = hash_table[index].reader;
        cout << "Number Ticket: " << reader->numberTicket << endl;
        cout << "FIO: " << reader->fio << endl;
        cout << "Year: " << reader->year << endl;
        cout << "Address: " << reader->adress << endl;
        cout << "Place: " << reader->place << endl;
    } else {
        cout << "Segment is empty or deleted."<<endl;
    }
}

const Reader* Hash::find_by_number(const std::string& ticket) {
    int hash = hash_function(ticket);
    int step = 1;
    int start = hash;
    while (!hash_table[hash].isEmpty) {
        if (!hash_table[hash].isDeleted and is_this_key(ticket, hash_table[hash])) {
            return hash_table[hash].reader;
        }
        hash = step_function(start, step);
        step++;
        if (step >= SIZE) break;
    }
    return nullptr;
}

void Hash::print_all() {
    bool found = false;
    for (int i = 0; i < SIZE; i++) {
        if (!hash_table[i].isEmpty and !hash_table[i].isDeleted) {
            const Reader* r = hash_table[i].reader;
            cout << "Билет: " << r->numberTicket 
                << " | ФИО: " << r->fio 
                << " | Год: " << r->year << endl;
            found = true;
        }
    }
    if (!found) cout << "Нет зарегистрированных читателей."<<endl;
}

void Hash::clear() {
    for (int i = 0; i < SIZE; i++) {
        hash_table[i].reader = nullptr;
        hash_table[i].isEmpty = true;
        hash_table[i].isDeleted = false;
    }
    cout << "Хеш-таблица читателей очищена."<<endl;
}

void Hash::search_by_fio(const std::string& fio_fragment) {
    bool found = false;
    for (int i = 0; i < SIZE; i++) {
        if (!hash_table[i].isEmpty and !hash_table[i].isDeleted) {
            const Reader* r = hash_table[i].reader;
            if (r->fio.find(fio_fragment) != std::string::npos) {
                cout << "Найден: " << r->fio 
                    << " | Билет: " << r->numberTicket << endl;
                found = true;
            }
        }
    }
    if (!found) cout << "Читателей с таким фрагментом ФИО не найдено."<<endl;
}