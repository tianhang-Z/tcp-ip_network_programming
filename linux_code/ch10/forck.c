#include <stdio.h>
#include <unistd.h>

int gval=10;
int main(int argc,char* argv[]){
    __pid_t pid;
    int lval=20;
    gval++,lval++;

    pid=fork();
    if(pid==0){
        gval+=2,lval+=2;
    }
    else{
        gval-=2,lval-=2;
    }

    if(pid==0){
        printf("child proc : gval %d, lval %d \n",gval,lval);
    }
    else{
        printf("parent proc : gval %d, lval %d \n",gval,lval);
    }

    return 0;
}

