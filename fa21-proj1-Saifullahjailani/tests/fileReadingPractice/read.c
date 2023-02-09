#include<stdlib.h>
#include<stdio.h>


int main(){
    FILE *a = fopen("dict.txt", "r");
    if(!a){
        printf("could not open the file\n");
    }
    char c;
    while((c = getc(a)) && c != EOF){
        printf("%c", c);
    }

    return 0;
}