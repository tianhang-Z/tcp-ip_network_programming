#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *buf);

int main(int argc,char* argv[]){
    int send_sock;
    struct sockaddr_in brd_adr;
    char buf[BUF_SIZE];

    if(argc!=3){
        printf("usage : %s <group_ip> <port> \n",argv[0]);
        exit(1);
    }

    send_sock=socket(PF_INET,SOCK_DGRAM,0);
    memset(&brd_adr,0,sizeof(brd_adr));
    brd_adr.sin_family=AF_INET;
    brd_adr.sin_addr.s_addr=inet_addr(argv[1]);
    brd_adr.sin_port=htons(atoi(argv[2]));

    int so_brd=1;
    setsockopt(send_sock,SOL_SOCKET,SO_BROADCAST,(void*)&so_brd,sizeof(so_brd));

    FILE *fp;
    if((fp=fopen("news.txt","r"))==NULL) 
        error_handling("fopen() error");

    while(!feof(fp)){
        fgets(buf,BUF_SIZE,fp);
        sendto(send_sock,buf,strlen(buf),0,(struct sockaddr*)&brd_adr,sizeof(brd_adr));
        fputs(buf,stdout);
        printf("--------\n");
        sleep(2);
    }

    fclose(fp);
    close(send_sock);
    return 0;
}

void error_handling(char *buf){
    fputs(buf,stderr);
    fputc('\n',stderr);
    exit(1);
}