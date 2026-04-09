#include "../hash/hash.h"
#include "../tree/tree.h"
#include "../transaction/transaction.h"

class Print
{
private:
    Hash readers;
    Tree books;
    TransactionList transactions;

    void regUser();
    void deactivateUser();
    void showAllUser();
    void deleteAllUser();
    void findUserByNumber();
    void findUserByFIO();
    void addBook();
    void deleteBook();
    void showAllBook();
    void deleteAllBook();
    void findBookByHash();
    void findBookByNameOrAthor();
    void giveBook();
    void takeBook();

public:
    void run();


    Print();
    ~Print();
};
