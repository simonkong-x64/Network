#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

int main(){
    int pid;
    if( (pid = fork()) == 0 ){
        while(true){
            printf("Parent");
        }
    }
    else{
        while(true){
            printf("Child");
        }
    }
}