#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_THREAD 100
#define RANGE 50000
void *thread_inc(void *arg);
void *thread_dec(void *arg);
long long num=0;
pthread_mutex_t mutex;

int main(int argc,char *argv[]){

    pthread_t t_id[NUM_THREAD];
    pthread_mutex_init(&mutex,NULL);
    printf("sizeof(long long) is %ld bytes \n",sizeof(num));
    for(int i=0;i<NUM_THREAD;i++){
        if(i%2) pthread_create(&(t_id[i]),NULL,thread_inc,NULL);
        else pthread_create(&(t_id[i]),NULL,thread_dec,NULL);
    }
    for(int i=0;i<NUM_THREAD;i++){
        pthread_join(t_id[i],NULL);
    }
    printf("num is %lld \n",num);
    pthread_mutex_destroy(&mutex);

    return 0;
}


void *thread_inc(void *arg){
    pthread_mutex_lock(&mutex);
    for(int i=0;i<RANGE;i++){
        num+=i;  //临界区
    }
    pthread_mutex_unlock(&mutex);

    return NULL;
}
void *thread_dec(void *arg){
    pthread_mutex_lock(&mutex);
    for(int i=0;i<RANGE;i++){
        num-=i;  //临界区
    }
    pthread_mutex_unlock(&mutex);

    return NULL;
}
