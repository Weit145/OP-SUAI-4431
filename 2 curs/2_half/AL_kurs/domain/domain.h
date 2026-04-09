#include "string"

using namespace std;
#pragma once

struct Reader{
    Reader(string, string, int , string , string);

    string numberTicket;
    string fio;
    int year;
    string adress;
    string place;
};



struct Book{
    Book(string, string, string, string, int, int, int);

    string hash;
    string author;
    string name;
    string publishing;
    int year;
    int allExample;
    int assetExample;
};

struct TransactionBook{
    TransactionBook(string, string, string, string);

    string numberTicket;
    string hash;
    string dataEnter;
    string dataOut;
};