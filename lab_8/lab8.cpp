#include <iostream>
#include <stdio.h>
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
    FILE* inputFile = fopen("text.txt", "r");
    FILE* outputFile = fopen("output.txt", "w");
    if (!inputFile) {
        cout<<"Error open text.txt"<<endl;
        fclose(outputFile);
        return 1;
    }
    if (!outputFile) {
        cout<<"error edit output.txt"<<endl;
        fclose(inputFile);  
        return 1;
    }
    char l[1024];
    while (fgets(l, sizeof(l), inputFile) != NULL) {
        if (zad(l)) {
            fputs(l, outputFile);
        }
    }
    fclose(inputFile);
    fclose(outputFile);
}