#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_THREAD 100
#define RANGE 50000
void *thread_inc(void *arg);
void *thread_dec(void *arg);
long long num=0;

int main(int argc,char *argv[]){

    pthread_t t_id[NUM_THREAD];

    printf("sizeof(long long) is %ld bytes \n",sizeof(num));
    for(int i=0;i<NUM_THREAD;i++){
        if(i%20) pthread_create(&(t_id[i]),NULL,thread_inc,NULL);
        else pthread_create(&(t_id[i]),NULL,thread_dec,NULL);
    }
    for(int i=0;i<NUM_THREAD;i++){
        pthread_join(t_id[i],NULL);
    }

    printf("num is %lld \n",num);
    return 0;
}


void *thread_inc(void *arg){
    for(int i=0;i<RANGE;i++){
        num+=i;  //临界区
    }
    return NULL;
}
void *thread_dec(void *arg){
    for(int i=0;i<RANGE;i++){
        num-=i;  //临界区
    }
    return NULL;
}
