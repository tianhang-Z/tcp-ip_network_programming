#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

void *thread_main(void *arg);

int main(int argc,char *argv[]){

    pthread_t t_id;
    int thread_param=2;
    void *t_ret;  //r_tet存放msg的首地址

    if(pthread_create(&t_id,NULL,thread_main,(void*)&thread_param)!=0){
        puts("pthread_create() error");
        return -1;
    }
    //pthread的第二个参数是二级指针 
    //指针作为参数时，可以修改指针指向的地址的值
    //那么二级指针作为参数时，二级指针的值是一级指针的地址 
    //通过二级指针 可以修改一级指针所在地址的值 也就是修改一级指针的指向
    //最终结果就是 把t_ret的指向改为thread_main的返回值  即（void*)msg的值
    //msg是个指针 也即t_ret和msg的指向相同了
    if(pthread_join(t_id,&t_ret)!=0){
        puts("pthread_join error ");
        return -1;
    }
    puts("end of main");
    printf("thread return value: %s \n",(char*)t_ret);
    free(t_ret);   //需要释放
    return 0;
}

void *thread_main(void* arg){

    int cnt=*((int *)arg);

    // int *msg=(int* )malloc(sizeof(int)*50);
    // msg[0]=1;
    // msg[1]=2;

    char *msg=(char* )malloc(sizeof(char)*50);
    strcpy(msg,"hello thread");
    for(int i=0;i<cnt;i++)
    {
        sleep(1);
        puts("running thread");
    }
    return (void* )msg;
}

