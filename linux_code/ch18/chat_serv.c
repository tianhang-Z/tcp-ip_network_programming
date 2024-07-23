#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFF_SIZE 1024
#define MAX_CLNT 10

void error_handling(char *message);
void send_msg(char* msg,int len);
void* handle_clnt(void* arg);


pthread_mutex_t mutex;
int clnt_socks[MAX_CLNT];   //对该变量的读写是临界区
int clnt_cnt=0;

/*
向所有客户端转发消息
*/
int main(int argc,char*argv[]){

    int serv_sock,clnt_sock;
    struct sockaddr_in serv_addr,clnt_addr;

    if(argc!=2){
        printf("usage %s <port> \n",argv[0]);
        exit(1);
    }

    pthread_mutex_init(&mutex,NULL);   //互斥量
    serv_sock=socket(PF_INET,SOCK_STREAM,0);

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_port=htons(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1) 
        error_handling("bind() error");
    if(listen(serv_sock,5)==-1)
        error_handling("listen() error");

    while(1){
        int clnt_adr_size=sizeof(clnt_addr);
        int clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_adr_size);

        pthread_mutex_lock(&mutex);
        clnt_socks[clnt_cnt++]=clnt_sock;   //临界区
        pthread_mutex_unlock(&mutex);

        pthread_t t_id;            //处理新连接
        pthread_create(&t_id,NULL,handle_clnt,(void*)&clnt_sock);
        pthread_detach(t_id);

        printf("clnt connect,sock is %d \n",clnt_sock);
        printf("ip is %s \n",inet_ntoa(clnt_addr.sin_addr));

    }

    close(serv_sock);
}

void* handle_clnt(void* arg){
    int clnt_sock=*((int*)arg);
    int str_len=0;
    char msg[BUFF_SIZE]={0};         
    while((str_len=read(clnt_sock,msg,sizeof(msg)))!=0){
        send_msg(msg,str_len);
    }

    //连接结束后 从clnt_socks删除
    pthread_mutex_lock(&mutex);
    for(int i=0;i<clnt_cnt;i++){
        if(clnt_socks[i]==clnt_sock){
            while(i++<clnt_cnt-1)
                clnt_socks[i]=clnt_socks[i+1];
            break;
        }
    }
    clnt_cnt--;
    pthread_mutex_unlock(&mutex);
    close(clnt_sock);
    return NULL;
}


//send to all
void send_msg(char* msg,int len){
    pthread_mutex_lock(&mutex);
    for(int i=0;i<clnt_cnt;i++){
        write(clnt_socks[i],msg,len);
    }
    pthread_mutex_unlock(&mutex);

}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}