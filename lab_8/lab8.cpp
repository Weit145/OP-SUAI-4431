#include <iostream>
#include <stdio.h>
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
    char l;
    int n=0;
    bool f=0;
    bool j=0;
    while ((l=fgetc(inputFile)) != EOF) {
        if (zad(l) or f) {
            if (l == '.' or l == '!' or l == '?') {
                f = 0;
                fputc(l, outputFile);
                fputc('\n', outputFile);
                continue;
            }
            f = 1;
            n++;
            fputc(l, outputFile);
        }
    }
    if (n==0){
        cout<<"output.txt is clear"<<endl;
    }
    fclose(inputFile);
    fclose(outputFile);
}