#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

#define BUFFER_SIZE 10

sem_t slotCheio, slotVazio;
sem_t mutexProd, mutexCons;


int buffer[BUFFER_SIZE];

void insere(int item, int id){

    printf("Produtor %d tentando produzir\n", id);

    static int index = 0;
    sem_wait(&slotVazio);
    sem_wait(&mutexProd);

    printf("Produtor %d produzindo com index %d\n", id, index);
    buffer[index]  = item;
    index = (index+1)%BUFFER_SIZE;
    printf("Produtor %d produziu com index %d\n", id, index);

    sem_post(&mutexProd);
    sem_post(&slotCheio);
}

int* retira(int id){
    printf("Consumidor %d tentando consumir\n",id);

    int* result = (int *) malloc(sizeof(int) * BUFFER_SIZE);
    sem_wait(&mutexCons);

    printf("Consumidor %d consumindo tudo\n", id);
    for(int i = 0; i < BUFFER_SIZE; i++){
        sem_wait(&slotCheio);
        result[i] = buffer[i];
        sem_post(&slotVazio);
    }
    printf("Consumidor %d, acabou\n", id);

    sem_post(&mutexCons);
    return result;
}

void* produtor(void* args){
    int id = (long long int) args;
    while(1){
        printf("Produtor %d está produzindo\n", id);
        insere(id, id);
    }
    pthread_exit(NULL);
}

void* consumidor(void* args){
    int id = (long long int) args;
    int* saida;
    while(1){
        printf("Consumidor %d está consumindo\n", id);
        saida = retira(id);
    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[]){
    int nCons, nProd;

    //verificação de entradas
    if(argc < 3){
        printf("Entrada correta:\n %s <número de threads consumidoras> <número de threads produtoras>\n",argv[0]);
        return 1;
    }

    nCons = atoi(argv[1]);
    nProd = atoi(argv[2]);

    //aloca as tids
    pthread_t* tidCons = (pthread_t *) malloc(sizeof(pthread_t) * nCons);
    pthread_t* tidProd = (pthread_t *) malloc(sizeof(pthread_t) * nProd);

    //inicio os semaforos
    sem_init(&mutexCons, 0, 1);
    sem_init(&mutexProd, 0, 1);
    sem_init(&slotCheio, 0, 0);
    sem_init(&slotVazio, 0, BUFFER_SIZE);

    //crio as threads
    for(int i = 0; i < nProd; i++){
        if(pthread_create(&tidProd[i], NULL, produtor, (void*)i)){
            printf("ERROR... pthread_create\n");
            return 2;
        }
    }
    for(int i = 0; i < nCons; i++){
        if(pthread_create(&tidCons[i], NULL, consumidor, (void*)i)){
            printf("ERROR... pthread_create\n");
            return 2;
        }
    }

    //aguardo o fim das threads
    for(int i = 0; i < nProd; i++){
        if(pthread_join(tidProd[i], NULL)){
            printf("ERROR... pthread_join\n");
            return 3;
        }
    }
    for(int i = 0; i < nCons; i++){
        if(pthread_join(tidCons[i], NULL)){
            printf("ERROR... pthread_join\n");
            return 3;
        }
    }
    return 0;
}