#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUFF_SIZE 1024
#define NAME_SIZE 20
void error_handling(char *message);
void *recv_msg(void* arg);
void *send_msg(void* arg);

char name[NAME_SIZE];
char msg[BUFF_SIZE];

int main(int argc,char *argv[]){

    int sock;
    char message[BUFF_SIZE]={0};
    int str_len;

    struct sockaddr_in serv_adr;

    if(argc!=4){
        printf("usage :%s <ip> <port> <port> \n",argv[0]);
        exit(1);
    }
    sprintf(name,"[%s]",argv[3]);

    sock=socket(PF_INET,SOCK_STREAM,0);
    if(sock==-1) error_handling("socket() error\n");

    memset(&serv_adr,0,sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
    serv_adr.sin_port=htons(atoi(argv[2]));
    
    if(connect(sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
        error_handling("connect() error");


    pthread_t snd_id,recv_id;

    pthread_create(&snd_id,NULL,send_msg,(void*)&sock);
    pthread_create(&recv_id,NULL,recv_msg,(void*)&sock);

    pthread_join(snd_id,NULL);
    pthread_join(recv_id,NULL);

    close(sock);
    return 0;
}

//一直接收
void *recv_msg(void* arg){
    int sock=*((int*)arg);
    char name_msg[NAME_SIZE+BUFF_SIZE];
    int str_len;
    while (1)
    {
        str_len=read(sock,name_msg,BUFF_SIZE+NAME_SIZE-1);
        if(str_len==-1) 
            return (void*)-1;
        name_msg[str_len]=0;
        fputs(name_msg,stdout);
    }
    return NULL;
}

void *send_msg(void* arg){
    int sock=*((int*)arg);
    char name_msg[NAME_SIZE+BUFF_SIZE+4];
    while(1){
        fgets(msg,BUFF_SIZE,stdin);
        if(!(strcmp(msg,"q\n"))||!(strcmp(msg,"Q\n"))){
            close(sock);
            exit(0);
        }
        sprintf(name_msg,"%s : %s",name,msg);
        write(sock,name_msg,NAME_SIZE+BUFF_SIZE);
    }   
    return NULL;
}


void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}