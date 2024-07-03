#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

/*
解决了ch4客户端的问题 
*/
#define BUFF_SIZE 1024
void error_handling(char *message);

int main(int argc,char *argv[]){

    int sock;
    char message[BUFF_SIZE]={0};
    int str_len;

    struct sockaddr_in serv_adr;

    if(argc!=3){
        printf("usage :%s <ip> <port> \n",argv[0]);
        exit(1);
    }

    sock=socket(PF_INET,SOCK_STREAM,0);
    if(sock==-1) error_handling("socket() error\n");

    memset(&serv_adr,0,sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
    serv_adr.sin_port=htons(atoi(argv[2]));
    
    if(connect(sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
        error_handling("connect() error");

    while(1){
        fputs("input message (Q to quit)\n",stdout);
        fgets(message,BUFF_SIZE,stdin);

        if(!strcmp(message,"q\n")||!strcmp(message,"Q\n"))
            break;
        
        str_len=write(sock,message,strlen(message));
        printf("str_len: %d\n",str_len);
        int recv_len=0,recv_cnt=0;
        while(recv_len<str_len){
            recv_cnt=read(sock,&message[recv_len],BUFF_SIZE-1);
            printf("recv_cnt:%d\n",recv_cnt);
            if(recv_cnt==-1)
                error_handling("read() error");
            recv_len+=recv_cnt;
        }
        printf("message from server: %s\n",message);

    }
    close(sock);
    return 0;
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}