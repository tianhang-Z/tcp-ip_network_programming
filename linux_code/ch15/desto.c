#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

int main(){

    FILE *fp;
    int fd=open("data.txt",O_WRONLY|O_CREAT|O_TRUNC);
    if(fd==-1){
        fputs("error \n",stdout);
        exit(1);
    }

    fp=fdopen(fd,"w");
    fputs("network c programming \n",fp);
    fclose(fp);
    return 0;
}