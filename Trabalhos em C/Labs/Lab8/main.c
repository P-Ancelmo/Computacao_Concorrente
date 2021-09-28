#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>

sem_t cond14, cond3;

void* t2(void* arg){
    printf("Seja bem-vindo!\n");
    sem_post(&cond14);
    sem_post(&cond14);
    pthread_exit(NULL);
}

void* t1(void* arg){
    sem_wait(&cond14);
    printf("Fique a vontade.\n");
    sem_post(&cond3);
    pthread_exit(NULL);
}

void* t4(void* arg){
    sem_wait(&cond14);
    printf("Sente-se por favor\n");
    sem_post(&cond3);
    pthread_exit(NULL);
}

void* t3(void* arg){
    sem_wait(&cond3);
    sem_wait(&cond3);
    printf("Volte sempre!\n");
    pthread_exit(NULL);
}



int main(){
    pthread_t tid[4];
    sem_init(&cond14,0,0);
    sem_init(&cond3,0,0);

    if(pthread_create(&tid[0], NULL,t1,NULL)){
        printf("ERROR... pthread_create");
        return 1;
    }
    if(pthread_create(&tid[1], NULL,t2,NULL)){
        printf("ERROR... pthread_create");
        return 1;
    }
    if(pthread_create(&tid[2], NULL,t3,NULL)){
        printf("ERROR... pthread_create");
        return 1;
    }
    if(pthread_create(&tid[3], NULL,t4,NULL)){
        printf("ERROR... pthread_create");
        return 1;
    }
    for(int i = 0; i < 4; i++){
        if(pthread_join(tid[i], NULL)){
            printf("ERROR... pthread_join");
            return 2;
        }
    }
}
