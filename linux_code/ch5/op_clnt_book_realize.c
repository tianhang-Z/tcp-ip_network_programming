#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>


#define BUFF_SIZE 1024
#define ECHO_SIZE 10
void error_handling(char *message);

int main(int argc,char *argv[]){

    int sock;
    int message[BUFF_SIZE]={0};
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

    printf("nums count:");
    scanf("%d",&message[0]);
    int nums_cnt=message[0];
    for(int i=1;i<=nums_cnt;i++){
        printf("input num %d :",i);
        scanf("%d",&message[i]);
    }  
    printf("operator : ");
    char op;
    getchar();
    scanf(" %c",&op);
    message[nums_cnt+1]=(int)op;
    write(sock,message,4*nums_cnt+8);
    int result=0;
    read(sock,&result,4);
    printf("result:%d\n",result);
    close(sock);
    return 0;
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}