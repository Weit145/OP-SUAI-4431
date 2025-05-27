#include <fstream>
#include <iostream>
#include <locale.h>

using namespace std;


bool zad(char line) {
    bool f;
    while (line != '\0') {
        f=(line==' ' or line=='\t' or line=='\n');
        if (!f) {
            return (line == '-');
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
    char l;
    int n=0;
    bool f=0;
    while (!f1.eof()){   
        f1.get(l);
        if (zad(l) or f) {
            if(l=='.' or l=='!' or l=='?'){
                f=0;
                f2<<l<<endl;
                continue;
            }
            f=1;
            n++;
            f2<<l;
        }
    }
    if (l!='.'){
        cout<<"Error"<<endl;
    }
    if (n==0){
        cout<<"output.txt is clear"<<endl;
    }
    f1.close();
    f2.close();
}