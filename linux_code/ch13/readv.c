#include <stdio.h>
#include <sys/uio.h>

int main(int argc,char *argv[]){
    struct iovec vec[2];
    char buf1[20]={0};
    char buf2[20]={0};
    int str_len;

    vec[0].iov_base=buf1;
    vec[0].iov_len=5;
    vec[1].iov_base=buf2;
    vec[1].iov_len=20;

    str_len=readv(0,vec,2);
    printf("read bytes :%d \n",str_len);
    printf("first msg :%s \n",buf1);
    printf("second msg :%s \n",buf2);
    return 0;
}