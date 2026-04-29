#include <iostream>
#include <string>
#include "variant/variant.h"
#include "hash/hash.h"
#include "tree/tree.h"
#include "transaction/transaction.h"
#include "print/print.h"
#include <math.h>
#include <clocale>

using namespace std;

int main() {

    setlocale(LC_ALL, "ru_RU.UTF-8");

    Variant::printVar(230);

    
    Print pr;
    pr.run();
    return 0;
}