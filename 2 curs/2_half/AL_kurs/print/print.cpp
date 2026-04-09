#include <iostream>
#include <string>
#include <math.h>
#include "print.h"

using namespace std;


Print::Print(){}
Print::~Print(){}

void Print::regUser(){
    string ticket, fio, adress, place; 
    int year;

    cout << "Номер билета (ANNNN-YY): "; 
    getline(cin, ticket);
    cout << "ФИО: "; 
    getline(cin, fio);
    cout << "Год рождения: "; cin >> year;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
    cout << "Адрес: "; 
    getline(cin, adress);
    cout << "Место работы/учёбы: "; 
    getline(cin, place);

    try
    {
        Reader* r = new Reader(ticket, fio, year, adress, place);
        readers.add_segment(r);
    }
    catch(const exception& e)
    {
        cout << e.what() << '\n';
    }
    
}

void Print::deactivateUser(){
    string ticket;
    cout << "Номер билета: "; 
    getline(cin, ticket);
    const Reader* r = readers.find_by_number(ticket);
    if (!r) { 
        cout << "Читатель не найден."<<endl; 
        return; 
    }
    if (transactions.has_active_issuances(ticket)) {
        cout << "У читателя есть выданные книги! Сначала верните их."<<endl;
        return;
    }
    if (readers.delete_segment(r)) cout << "Читатель снят с обслуживания."<<endl;
}

void Print::showAllUser(){
    readers.print_all();
}

void Print::deleteAllUser(){
    readers.clear();
    cout<< "Данные о читателях очищены."<<endl;
}

void Print::findUserByNumber(){
    string ticket; 
    cout << "Номер билета: "; 
    getline(cin, ticket);
    const Reader* r = readers.find_by_number(ticket);
    if (r) {
        cout << "Найден: " << r->fio << " (" << r->numberTicket << ")"<<endl;
        transactions.print_for_reader(ticket);
    } 
    else cout << "Не найден."<<endl;
}

void Print::findUserByFIO(){
    string fio; 
    cout << "ФИО (фрагмент): "; 
    getline(cin, fio);
    readers.search_by_fio(fio);
}

void Print::addBook(){
    string hash, author, name, pub; 
    int y, all, asset;
    cout << "Шифр (NNN.MMM): "; 
    getline(cin, hash);
    cout << "Автор: "; 
    getline(cin, author);
    cout << "Название: "; 
    getline(cin, name);
    cout << "Издательство: "; 
    getline(cin, pub);
    cout << "Год: "; 
    cin >> y;
    cout << "Всего экземпляров: "; 
    cin >> all;
    cout << "В наличии: "; 
    cin >> asset; 
    cin.ignore();
    try {
        Book* b = new Book(hash, author, name, pub, y, all, asset);
        books.add(b);
    } 
    catch (const exception& e) { cout << e.what() << endl; }
}

void Print::deleteBook(){
    string hash;
    cout << "Шифр: "; 
    getline(cin, hash);
    if (transactions.has_active_issuances_for_book(hash)) {
        cout << "Книга выдана читателям! Сначала верните все экземпляры."<<endl;
        return;
    }
    if (books.find_by_hash(hash)) {
        books.remove_by_hash(hash);
        cout << "Книга удалена."<<endl;
    } else {
        cout << "Книга не найдена."<<endl;
    }
}

void Print::showAllBook(){
    books.print_all_books();
}

void Print::deleteAllBook(){
    books.clear();
    cout << "Данные о книгах очищены."<<endl;
}

void Print::findBookByHash(){
    string hash; cout << "Шифр: "; 
    getline(cin, hash);
    Book* b = books.find_by_hash(hash);
    if (b) {
        cout << "Найдена: " << b->author << " - " << b->name << " (" << b->hash << ")"<<endl;
        transactions.print_for_book(hash);
    } else cout << "Не найдена."<<endl;
}

void Print::findBookByNameOrAthor(){
    string frag; cout << "Фрагмент автора или названия: "; 
    getline(cin, frag);
    books.search_by_fragment(frag);
}

void Print::giveBook(){
    string ticket, hash, date;
    cout << "Номер билета: "; 
    getline(cin, ticket);
    cout << "Шифр книги: "; 
    getline(cin, hash);
    cout << "Дата выдачи: "; 
    getline(cin, date);
    Book* b = books.find_by_hash(hash);
    if (!b) { 
        cout << "Книга не найдена."<<endl; 
        return; 
    }
    if (b->assetExample <= 0) {
        cout << "Нет свободных экземпляров!"<<endl; 
        return; 
    }
    if (transactions.issue(ticket, hash, date)) {
        b->assetExample--;
    }
}

void Print::takeBook(){
    string ticket, hash, date;
    cout << "Номер билета: "; 
    getline(cin, ticket);
    cout << "Шифр книги: "; 
    getline(cin, hash);
    cout << "Дата возврата: "; 
    getline(cin, date);
    Book* b = books.find_by_hash(hash);
    if (!b) { 
        cout << "Книга не найдена."<<endl; 
        return; 
    }
    if (transactions.return_book(ticket, hash, date)) {
        b->assetExample++;
    }
}

void Print::run(){
    int choice=-1;
    while(choice!=0){
        cout << "\n=== БИБЛИОТЕКА ==="<<endl;
        cout << "1. Регистрация нового читателя"<<endl;
        cout << "2. Снятие читателя с обслуживания"<<endl;
        cout << "3. Просмотр всех читателей"<<endl;
        cout << "4. Очистка данных о читателях"<<endl;
        cout << "5. Поиск читателя по номеру билета"<<endl;
        cout << "6. Поиск читателя по ФИО"<<endl;
        cout << "7. Добавление новой книги"<<endl;
        cout << "8. Удаление книги"<<endl;
        cout << "9. Просмотр всех книг"<<endl;
        cout << "10. Очистка данных о книгах"<<endl;
        cout << "11. Поиск книги по шифру"<<endl;
        cout << "12. Поиск книги по фрагменту автора/названия (Бойера-Мура)"<<endl;
        cout << "13. Выдача книги читателю"<<endl;
        cout << "14. Приём книги от читателя"<<endl;
        cout << "0. Выход"<<endl;
        cout << "Выбор: ";
        cin >> choice;
        cin.ignore();
        switch (choice)
        {
        case 1:
            regUser();
            break;
        case 2:
            deactivateUser();
            break;
        case 3:
            showAllUser();
            break;
        case 4:
            deleteAllUser();
            break;
        case 5:
            findUserByNumber();
            break;
        case 6:
            findUserByFIO();
            break;
        case 7:
            addBook();
            break;
        case 8:
            deleteBook();
            break;
        case 9:
            showAllBook();
            break;
        case 10:
            deleteAllBook();
            break;
        case 11:
            findBookByHash();
            break;
        case 12:
            findBookByNameOrAthor();
            break;
        case 13:
            giveBook();
            break;
        case 14:
            takeBook();
            break;
        default:
            break;
        }
    }
}