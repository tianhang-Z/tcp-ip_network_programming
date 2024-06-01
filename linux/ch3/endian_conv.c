#include <stdio.h>
#include <arpa/inet.h>

int main(int argc,char* argv[]){
    unsigned short host_port=0x1234;
    unsigned short net_port;
    unsigned long host_addr=0x12345678;
    unsigned long net_addr;

    struct  
    {
        /* data */
    };
    

    net_port=htons(host_port);
    net_addr=htonl(host_addr);

    printf("host ordered port : %#x \n" ,host_port);
    printf("net ordered port : %#x \n" ,net_port);
    printf("host ordered addr : %#x \n" ,host_addr);
    printf("net ordered addr : %#x \n" ,net_addr);
    return 0;

}