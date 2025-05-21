#include <fstream>
#include <iostream>
#include <locale.h>

using namespace std;

int prov(char l) {
    return (l==' ' or l=='\t' or l=='\n');
}
int zad(const char* line) {
    while (*line != '\0') {
        if (!prov(*line)) {
            return (*line == '-');
        }
        line++;
    }
    return 0;
}
int main(){
    setlocale(LC_ALL, "Russian"); 
    ifstream f1("text.txt");
    ofstream f2("output.txt");
    if (f1.fail() ){
        cout<<"Error open text.txt"<<endl;
        f2.close();
        return 1;
    }
    if (f2.fail()){
        cout<<"Error edit output.txt"<<endl;
        f1.close();
        return 1;
    }
    char l[1024];
    while (!f1.eof())
    {   
        f1.getline(l,1024);
        if (zad(l)) {
            f2<<l<<endl;
        }
    }
    f1.close();
    f2.close();
}