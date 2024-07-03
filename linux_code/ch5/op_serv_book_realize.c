#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFF_SIZE 1024
void error_handling(char *message);
int cal(int num_cnt,int nums[],int op);

int main(int argc,char *argv[]){

    int serv_sock,clnt_sock;

    struct sockaddr_in serv_adr,clnt_adr;
    socklen_t clnt_adr_sz;

    if(argc!=2){
        printf("usage :%s <port> \n",argv[0]);
        exit(1);
    }

    serv_sock=socket(PF_INET,SOCK_STREAM,0);
    if(serv_sock==-1) error_handling("socket() error\n");

    memset(&serv_adr,0,sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_adr.sin_port=htons(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
        error_handling("bind() error");

    if(listen(serv_sock,5)==-1)
        error_handling("listen() error");
    
    clnt_adr_sz=sizeof(clnt_adr);
    //支持五次计算
    for(int i=0;i<5;i++){
        /*
        nums_cnt:int 
        opinfo:int *
        op: int
        */
        int num_cnt=0;
        clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_adr,&clnt_adr_sz);
        read(clnt_sock,&num_cnt,4);

        int recv_len=0,recv_cnt=0;
        int num_size=4;
        int opinfo[BUFF_SIZE]={0};
        while (num_cnt*num_size>recv_len)
        {
            recv_cnt=read(clnt_sock,&opinfo[recv_len/4],BUFF_SIZE*4);
            recv_len+=recv_cnt;
        }
        printf("recv_len:%d\n",recv_len);
        int result;
        result=cal(num_cnt,opinfo,opinfo[num_cnt]);
        write(clnt_sock,&result,sizeof(result));
        close(clnt_sock);
    }
 
    close(serv_sock);
    return 0;
}

int cal(int num_cnt,int nums[],int op){
    int res=nums[0];
    switch (op){
        case '+':
            for(int i=1;i<num_cnt;i++) res+=nums[i];
            break;
        case '*':
            for(int i=1;i<num_cnt;i++) res*=nums[i];
            break;
        default:
            error_handling("op error");
    }
    return res;
}
void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}