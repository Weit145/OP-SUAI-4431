#include "domain.h"
#include "string"
#include <stdexcept>
#include <cctype>

using namespace std;

static bool is_valid_ticket(const string& ticket) {
    return ticket.size() == 8 &&
        isalpha(static_cast<unsigned char>(ticket[0])) &&
        isdigit(static_cast<unsigned char>(ticket[1])) &&
        isdigit(static_cast<unsigned char>(ticket[2])) &&
        isdigit(static_cast<unsigned char>(ticket[3])) &&
        isdigit(static_cast<unsigned char>(ticket[4])) &&
        ticket[5] == '-' &&
        isdigit(static_cast<unsigned char>(ticket[6])) &&
        isdigit(static_cast<unsigned char>(ticket[7]));
}

static bool is_valid_book_hash(const string& hash) {
    return hash.size() == 7 &&
        isdigit(static_cast<unsigned char>(hash[0])) &&
        isdigit(static_cast<unsigned char>(hash[1])) &&
        isdigit(static_cast<unsigned char>(hash[2])) &&
        hash[3] == '.' &&
        isdigit(static_cast<unsigned char>(hash[4])) &&
        isdigit(static_cast<unsigned char>(hash[5])) &&
        isdigit(static_cast<unsigned char>(hash[6]));
}

Reader::Reader(string numberTicket, string fio, int year, string adress, string place)
    : numberTicket(numberTicket), fio(fio), year(year), adress(adress), place(place)
{
    if (!is_valid_ticket(numberTicket)){
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
    if (!is_valid_book_hash(hash)){
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
    if (assetExample>allExample){
        throw invalid_argument("AssetExample invalid argument");
    }
}

TransactionBook::TransactionBook(string numberTicket, string hash, string dataEnter, string dataOut)
    : numberTicket(numberTicket), hash(hash), dataEnter(dataEnter), dataOut(dataOut)
{
    if (!is_valid_ticket(numberTicket)) {
        throw invalid_argument("numberTicket invalid argument");
    }
    if (!is_valid_book_hash(hash)){
        throw invalid_argument("Hash invalid argument");
    }
    if (dataEnter.empty()){
        throw invalid_argument("dataEnter invalid argument");
    }
}
