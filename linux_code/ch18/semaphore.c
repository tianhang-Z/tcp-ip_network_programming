#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

// 输入数据 然后读取它并累加 利用多线程实现  
// 需要两个信号量  输入和读取是互斥的 先输入,之后才允许读取  读取时，不允许输入

void* input_num(void* arg);
void* read_accu(void* arg);
int num;  
sem_t sem_input;
sem_t sem_accu;

int main(int argc,char *argv[]){

    pthread_t input_num_id,read_aacu_id;

    sem_init(&sem_input,0,1);
    sem_init(&sem_accu,0,0);

    pthread_create(&input_num_id,NULL,input_num,NULL);
    pthread_create(&read_aacu_id,NULL,read_accu,NULL);
    
    pthread_join(input_num_id,NULL);
    pthread_join(read_aacu_id,NULL);

    sem_destroy(&sem_input);
    sem_destroy(&sem_accu);

    return 0;
}

void* input_num(void* arg){
    for(int i=0;i<3;i++){
        puts("input a num:");
        sem_wait(&sem_input);
        scanf("%d",&num);
        sem_post(&sem_accu);
    }
    return NULL;
}
void* read_accu(void* arg){
    int sum=0;
    for(int i=0;i<3;i++){
        sem_wait(&sem_accu);
        sum+=num;
        sem_post(&sem_input);
    }
    printf("sum is %d \n",sum);
    return NULL;    
}

