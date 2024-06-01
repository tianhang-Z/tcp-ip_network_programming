#include <stdio.h>
#include <arpa/inet.h>

int main(){

    char *addr1="1.2.3.4";
    char addr2[]="1.2.3.256";

    unsigned long conv_addr=inet_addr(addr1);
    if(conv_addr==INADDR_NONE){
        printf("error\n");
    }
    else {
        printf("conv_addr:%#x \n",conv_addr);
    }
    unsigned long conv_addr1=inet_addr(addr2);
    if(conv_addr1==INADDR_NONE){
        printf("error\n");
    }
    else {
        printf("conv_addr1:%#x\n",conv_addr1);
    }
    return 0;
}