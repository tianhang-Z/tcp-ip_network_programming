#include <stdio.h>
#include <time.h>
#define BUF_SIZE 3

int main(){
    FILE * fp1;
    FILE * fp2;

    char buf[BUF_SIZE];
    clock_t start,end;
    start = clock();

    for(int i=0;i<1000;i++){
        fp1=fopen("news.txt","r");
        fp2=fopen("cpy2.txt","w");
        while (fgets(buf,BUF_SIZE,fp1)!=NULL)
            fputs(buf,fp2);
    }
    fclose(fp1);
    fclose(fp2);
    end=clock();
    printf("time=%f\n",(double)(end-start)/CLOCKS_PER_SEC);
    return 0;
}
