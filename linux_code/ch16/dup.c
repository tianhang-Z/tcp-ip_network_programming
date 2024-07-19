#include <stdio.h>
#include <unistd.h>

int main(){
    int cfd1,cfd2;
    char str1[]="hi \n";
    char str2[]="good \n";

    cfd1=dup(1);
    cfd2=dup2(cfd1,7);

    printf("fd1=%d ,fd2=%d \n",cfd1,cfd2);
    write(cfd1,str1,sizeof(str1));
    write(cfd2,str2,sizeof(str2));

    close(cfd1);
    close(cfd2);
    write(1,str1,sizeof(str1));
    close(1);
    if((write(1,str2,sizeof(str2)))==0)
        printf("error\n");
    return 0;
    
}