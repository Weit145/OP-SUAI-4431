#pragma once
#include "../domain/domain.h"
#include <string>

class Hash
{
private:
    const int SIZE=2000;

    struct Segment{
        const Reader* reader;
        bool isEmpty;
        bool isDeleted;
    };

    Segment* hash_table;

    int hash_function(const string);
    int step_function(const int, int);
    bool is_this_key(const string, const Segment);

public:
    void add_segment( const Reader*);
    bool delete_segment(const Reader*);
    void show_segment(int);
    const Reader* find_by_number(const std::string& ticket);
    void search_by_fio(const std::string& fio_fragment);
    void print_all();
    void clear();

    Hash();
    ~Hash();
};

