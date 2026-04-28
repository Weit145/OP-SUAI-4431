#include "variant.h"

Variant::Variant(const int N):N_(N){
}

Variant::~Variant() {}

string Variant::getPredmet() const{
    return getPredmet(N_);
}
string Variant::getPredmet(const int N) const{
    return pred_obl[N%number_1];
}


string Variant::getHash() const{
    return getHash(N_);
}
string Variant::getHash(const int N) const{
    return metod_hash[N%number_2];
}



string Variant::getSort() const{
    return getSort(N_);
}
string Variant::getSort(const int N) const{
    return metod_sort[N%number_3];
}



string Variant::getList() const{
    return getList(N_);
}
string Variant::getList(const int N) const{
    return vid_list[N%number_4];
}




string Variant::getTree() const{
    return getTree(N_);
}
string Variant::getTree(const int N) const{
    return metod_tree[N%number_5];
}



string Variant::getWord() const{
    return getWord(N_);
}
string Variant::getWord(const int N) const{
    return find_word[N%number_6];
}

void Variant::printVar() const{
    cout << "Предметная область: " << getPredmet() << endl;
    cout << "Метод хеширования: " << getHash() << endl;
    cout << "Метод сортировки: " << getSort() << endl;
    cout << "Вид списка: " << getList() << endl;
    cout << "Метод обхода дерева: " << getTree() << endl;
    cout << "Алгоритм поиска слова в тексте: " << getWord() << endl;
}

void Variant::printVar(const int N) {
    Variant temp(N);    
    temp.printVar(); 
}
