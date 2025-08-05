#include <stdio.h>
#include <stdlib.h>

int main(){
    char *hello;
    hello = (char *)malloc(sizeof(char)*100);
    printf("Enter: ");
    scanf("%s", hello);
    printf("%s", hello);
    return 0;
}