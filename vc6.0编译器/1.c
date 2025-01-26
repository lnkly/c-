#include"../Include/stdio.h"

int add(int a,int b){
    return a+b;
}


int main(void){
    int c = add(1,2);
    __asm{
        push eax;
        mov eax,c;
        inc eax;
        mov c,eax;
        pop eax
    }
    printf("hello world %d",c);
    return 0;
}