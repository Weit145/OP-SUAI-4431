#include "domain.h"
#include "string"
#include <stdexcept>
#include <cctype>

using namespace std;

Reader::Reader(string numberTicket, string fio, int year, string adress, string place)
    : numberTicket(numberTicket), fio(fio), year(year), adress(adress), place(place)
{
    if (numberTicket.empty()){
        throw invalid_argument("numberTicket invalid argument");
    }
    if (fio.empty()){
        throw invalid_argument("FIO invalid argument");
    }
    if (year<1926 or year>2026){
        throw invalid_argument("Year invalid argument");
    }
    if (adress.empty()){
        throw invalid_argument("Adress invalid argument");
    }
    if (place.empty()){
        throw invalid_argument("Place invalid argument");
    }
}

Book::Book(string hash, string author, string name, string publishing, int year, int allExample, int assetExample)
    : hash(hash), author(author), name(name), publishing(publishing), year(year), allExample(allExample), assetExample(assetExample)
{
    if (hash.empty()){
        throw invalid_argument("Hash invalid argument");
    }
    if (author.empty()){
        throw invalid_argument("Author invalid argument");
    }
    if (name.empty()){
        throw invalid_argument("Name invalid argument");
    }
    if (publishing.empty()){
        throw invalid_argument("Publishing invalid argument");
    }
    if (year >2027 or year<703){
        throw invalid_argument("Year invalid argument");
    }
    if (allExample<0){
        throw invalid_argument("AllExample invalid argument");
    }
    if (assetExample<0){
        throw invalid_argument("AssetExample invalid argument");
    }
}

TransactionBook::TransactionBook(string numberTicket, string hash, string dataEnter, string dataOut)
    : numberTicket(numberTicket), hash(hash), dataEnter(dataEnter), dataOut(dataOut)
{
    if (numberTicket.empty()) {
        throw invalid_argument("numberTicket invalid argument");
    }
    if (hash.empty() ){
        throw invalid_argument("Hash invalid argument");
    }
    if (dataEnter.empty()){
        throw invalid_argument("dataEnter invalid argument");
    }
}