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

    int serv_sock,clnt_sock;
    char message[BUFF_SIZE];
    int str_len;

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
    clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_adr,&clnt_adr_sz);
    if(clnt_sock==-1) error_handling("accept() error");

    char echo[ECHO_SIZE]="OK";
    //读取个数
    int operator_num=0;
    if(str_len=read(clnt_sock,&operator_num,sizeof(operator_num))!=0){
        printf("oper num:%d \n",operator_num);
        write(clnt_sock,echo,sizeof(echo));
    }
    //读取数据
    int nums[10]={0};
    for(int i=0;i<operator_num;i++){
        if(str_len=read(clnt_sock,&nums[i],4)!=0){
            printf("num %d:%d \n",i,nums[i]);
        }        
        else error_handling("recv error");
        write(clnt_sock,echo,sizeof(echo));
    }
    //读取操作符
    char op;
    if(str_len=read(clnt_sock,&op,1)!=0){
        if((op=='+')||(op=='*'))
            write(clnt_sock,echo,sizeof(echo));
        else error_handling("+ * error");
    }
    int res=nums[0];
    for(int i=1;i<operator_num;i++){
        if(op=='+') res+=nums[i];
        else res*=nums[i];
    }
    write(clnt_sock,(char *)&res,sizeof(res));
    close(serv_sock);
    return 0;
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}