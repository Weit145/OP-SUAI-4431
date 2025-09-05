#include<iostream>
#include<stdio.h>

using namespace std;

int main(){
    FILE* input=fopen("input.txt","r");
    FILE* out=fopen("out.txt","w");
    char l;
    while ((l=fgetc(input))!=EOF){
        fputc(l,out);
    }
    fputc('\n',out);
    fclose(input);
    fclose(out);
}