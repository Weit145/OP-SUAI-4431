#include <iostream>
#include <string>

using namespace std;

class Variant
{
private:
    const int number_1 = 5;
    const int number_2 = 3;
    const int number_3 = 6;
    const int number_4 = 4;
    const int number_5 = 2;
    const int number_6 = 1;

    const int N_;

    const string pred_obl[6] = {
        "Обслуживание читателей в библиотеке", 
        "Обслуживание клиентов в бюро проката автомобилей", 
        "Регистрация постояльцев в гостинице", 
        "Регистрация больных в поликлинике", 
        "Продажа авиабилетов", 
        "Обслуживание клиентов оператора сотовой связи"
    };

    const string metod_hash[4] = {
        "Открытое хеширование",
        "Закрытое хеширование с линейным опробованием",
        "Закрытое хеширование с квадратичным опробованием",
        "Закрытое хеширование с двойным хешированием"
    };

    const string metod_sort[7] = {
        "Подсчетом", 
        "Включением",
        "Извлечением",
        "Шейкерная",
        "Быстрая (Хоара)",
        "Слиянием",
        "Распределением"
    };

    const string vid_list[5] = {
        "Линейный однонаправленный",
        "Линейный двунаправленный",
        "Циклический однонаправленный",
        "Циклический двунаправленный",
        "Слоеный"
    };

    const string metod_tree[3] = {
        "Симметричный",
        "Обратный",
        "Прямой"
    };

    const string find_word[2] = {
        "Боуера и Мура (БМ)",
        "Прямой"
    };

public:
    Variant(const int N);
    ~Variant();
    string getPredmet() const;
    string getPredmet(const int N) const;
    string getHash() const;
    string getHash(const int N) const;
    string getSort() const;
    string getSort(const int N) const;
    string getList() const;
    string getList(const int N) const;
    string getTree() const;
    string getTree(const int N) const;
    string getWord() const;
    string getWord(const int N) const;
    void printVar() const;
    static void printVar(const int N);
};
