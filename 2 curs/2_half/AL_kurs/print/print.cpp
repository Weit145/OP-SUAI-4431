#include <iostream>
#include <limits>
#include <string>
#include "print.h"

using namespace std;

static bool read_int(const string& prompt, int& value) {
    cout << prompt;
    if (!(cin >> value)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Некорректное число."<<endl;
        return false;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return true;
}

Print::Print(){}
Print::~Print(){}

void Print::regUser(){
    string ticket, fio, adress, place; 
    int year;

    cout << "Номер билета (ANNNN-YY): "; 
    getline(cin, ticket);
    cout << "ФИО: "; 
    getline(cin, fio);
    if (!read_int("Год рождения: ", year)) return;
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
    if (transactions.has_active_issuances()) {
        cout << "Есть активные выдачи книг! Сначала примите книги обратно."<<endl;
        return;
    }
    readers.clear();
    cout<< "Данные о читателях очищены."<<endl;
}

void Print::findUserByNumber(){
    string ticket; 
    cout << "Номер билета: "; 
    getline(cin, ticket);
    const Reader* r = readers.find_by_number(ticket);
    if (r) {
        cout << "Номер билета: " << r->numberTicket << endl;
        cout << "ФИО: " << r->fio << endl;
        cout << "Год рождения: " << r->year << endl;
        cout << "Адрес: " << r->adress << endl;
        cout << "Место работы/учёбы: " << r->place << endl;
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
    if (!read_int("Год: ", y)) return;
    if (!read_int("Всего экземпляров: ", all)) return;
    if (!read_int("В наличии: ", asset)) return;
    Book* b = nullptr;
    try {
        b = new Book(hash, author, name, pub, y, all, asset);
        books.add(b);
    } 
    catch (const exception& e) {
        delete b;
        cout << e.what() << endl;
    }
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
    } else {
        cout << "Книга не найдена."<<endl;
    }
}

void Print::showAllBook(){
    books.print_all_books();
}

void Print::deleteAllBook(){
    if (transactions.has_active_issuances()) {
        cout << "Есть активные выдачи книг! Сначала примите книги обратно."<<endl;
        return;
    }
    books.clear();
    cout << "Данные о книгах очищены."<<endl;
}

void Print::findBookByHash(){
    string hash; cout << "Шифр: "; 
    getline(cin, hash);
    Book* b = books.find_by_hash(hash);
    if (b) {
        cout << "Шифр: " << b->hash << endl;
        cout << "Автор(ы): " << b->author << endl;
        cout << "Название: " << b->name << endl;
        cout << "Издательство: " << b->publishing << endl;
        cout << "Год издания: " << b->year << endl;
        cout << "Всего экземпляров: " << b->allExample << endl;
        cout << "В наличии: " << b->assetExample << endl;
        transactions.print_for_book(hash, readers);
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
    if (!readers.find_by_number(ticket)) {
        cout << "Читатель не найден."<<endl;
        return;
    }
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
    if (!readers.find_by_number(ticket)) {
        cout << "Читатель не найден."<<endl;
        return;
    }
    Book* b = books.find_by_hash(hash);
    if (!b) { 
        cout << "Книга не найдена."<<endl; 
        return; 
    }
    if (transactions.return_book(ticket, hash, date)) {
        b->assetExample++;
    }
}

void Print::loadTestData(){
    struct ReaderSeed {
        const char* ticket;
        const char* fio;
        int year;
        const char* adress;
        const char* place;
    };

    struct BookSeed {
        const char* hash;
        const char* author;
        const char* name;
        const char* publishing;
        int year;
        int allExample;
        int assetExample;
    };

    struct IssueSeed {
        const char* ticket;
        const char* hash;
        const char* date;
    };

    const ReaderSeed readerSeeds[] = {
        {"А0001-26", "Иванов Иван Иванович", 2004, "Санкт-Петербург, Невский пр., 10", "ГУАП"},
        {"Ч0002-26", "Петрова Анна Сергеевна", 2003, "Санкт-Петербург, ул. Ленина, 8", "СПбГУ"},
        {"В0003-26", "Сидоров Павел Олегович", 1998, "Санкт-Петербург, Московский пр., 21", "ИТМО"},
        {"А0004-26", "Кузнецова Мария Андреевна", 2001, "Пушкин, Октябрьский б-р, 5", "Библиотека им. Маяковского"},
        {"В0005-26", "Смирнов Дмитрий Викторович", 1995, "Колпино, ул. Тверская, 14", "ООО Север"}
    };

    const BookSeed bookSeeds[] = {
        {"001.001", "Страуструп Б.", "Язык программирования C++", "Бином", 2019, 5, 5},
        {"002.015", "Кормен Т., Лейзерсон Ч.", "Алгоритмы: построение и анализ", "Вильямс", 2022, 4, 4},
        {"003.120", "Кнут Д.", "Искусство программирования", "Вильямс", 2020, 3, 3},
        {"010.007", "Таненбаум Э.", "Современные операционные системы", "Питер", 2021, 6, 6},
        {"011.042", "Мартин Р.", "Чистый код", "Питер", 2023, 2, 2}
    };

    const IssueSeed issueSeeds[] = {
        {"А0001-26", "001.001", "12.05.2026"},
        {"Ч0002-26", "002.015", "12.05.2026"},
        {"В0003-26", "003.120", "13.05.2026"},
        {"А0004-26", "010.007", "14.05.2026"},
        {"В0005-26", "011.042", "15.05.2026"}
    };

    int addedReaders = 0;
    int addedBooks = 0;
    int addedIssues = 0;

    for (const ReaderSeed& seed : readerSeeds) {
        if (readers.find_by_number(seed.ticket)) continue;
        try {
            readers.add_segment(new Reader(seed.ticket, seed.fio, seed.year, seed.adress, seed.place));
            addedReaders++;
        } catch (const exception& e) {
            cout << "Читатель " << seed.ticket << ": " << e.what() << endl;
        }
    }

    for (const BookSeed& seed : bookSeeds) {
        if (books.find_by_hash(seed.hash)) continue;
        Book* book = nullptr;
        try {
            book = new Book(seed.hash, seed.author, seed.name, seed.publishing, seed.year, seed.allExample, seed.assetExample);
            books.add(book);
            addedBooks++;
        } catch (const exception& e) {
            delete book;
            cout << "Книга " << seed.hash << ": " << e.what() << endl;
        }
    }

    for (const IssueSeed& seed : issueSeeds) {
        if (!readers.find_by_number(seed.ticket)) {
            cout << "Выдача " << seed.ticket << " / " << seed.hash << ": читатель не найден."<<endl;
            continue;
        }

        Book* book = books.find_by_hash(seed.hash);
        if (!book) {
            cout << "Выдача " << seed.ticket << " / " << seed.hash << ": книга не найдена."<<endl;
            continue;
        }

        if (book->assetExample <= 0) {
            cout << "Выдача " << seed.ticket << " / " << seed.hash << ": нет свободных экземпляров."<<endl;
            continue;
        }

        if (transactions.issue(seed.ticket, seed.hash, seed.date)) {
            book->assetExample--;
            addedIssues++;
        }
    }

    cout << "Тестовые данные загружены: читателей добавлено " << addedReaders
        << ", книг добавлено " << addedBooks
        << ", выдач добавлено " << addedIssues << "."<<endl;
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
        cout << "15. Загрузить тестовый набор данных (по 5 записей)"<<endl;
        cout << "0. Выход"<<endl;
        cout << "Выбор: ";
        if (!(cin >> choice)) {
            if (cin.eof()) return;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Некорректный выбор."<<endl;
            choice = -1;
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
        case 15:
            loadTestData();
            break;
        default:
            break;
        }
    }
}
