#include <stdio.h>
#include <arpa/inet.h>

int main(){
    char *addr="127.1.2.3";
    struct sockaddr_in addr_inet;
    if(inet_aton(addr,&addr_inet.sin_addr)){
        printf("1  addr:%#x\n",addr_inet.sin_addr.s_addr);
    }
    else printf("error\n");

    char *str;
    str=inet_ntoa(addr_inet.sin_addr);    
    printf("1 addr -dotted decimal :%s\n",str);
    addr_inet.sin_addr.s_addr=htonl(0x01020304);
    printf("2  addr:%#x\n",addr_inet.sin_addr.s_addr);
    inet_ntoa(addr_inet.sin_addr);
    printf("2  addr -dotted decimal:%s\n",str);
    return 0;
    
}