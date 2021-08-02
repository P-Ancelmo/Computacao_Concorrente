#include<stdio.h>
#include<stdlib.h>
#include <pthread.h>
#include "timer.h"
#include<math.h>

float *vetor;
long long int dim =10;
int expo;
int nThreads;

typedef struct{
    float maior;
    float menor;
}maiorEmenor;


void * tarefa(void* arg){
    maiorEmenor *resultado;
    resultado = (maiorEmenor *) malloc(sizeof(maiorEmenor));
    if(resultado == NULL){
        fprintf(stderr, "ERRO.. malloc");
        return 2;
    }
    resultado->maior = 0;
    resultado->menor = 1000;
    int id = (int) arg;
    int tamBloco = dim/nThreads;
    int inicio = id * nThreads;
    int fim;
    if(id == nThreads-1) fim = dim;
    else fim = inicio + tamBloco;

    for(int i = inicio; i < fim; i++){
        if(vetor[i] > resultado->maior)
            resultado->maior = vetor[i];
        if(vetor[i] < resultado->menor)
            resultado->menor = vetor[i];
    }
    
    pthread_exit((void*) resultado);
}


int main(int argc, char*argv[]){   
    pthread_t *tid;
    double inicio, fim, tempSeq, tempConq, mediaSeq = 0, mediaConc = 0;
    int numeroTestes;    
    

    //verifica as entrdas e atribui valores
    if(argc < 3){
        fprintf(stderr,"Digite %s <expoente da dimensao do vetor> <numero de threads> opcional !--<numero de testes>--!\n",argv[0]);
        return 1;
    }
    expo = (double)atoll(argv[1]);
    nThreads = atoll(argv[2]);
    if(argv[3] != NULL)
        numeroTestes = atoll(argv[3]);
    else numeroTestes = 1;
    for(int k=0;k<expo-1;k++){
        dim*=10;
    }
    printf("dimensão: %lld\n", dim);
    
    //aloca as EDs
      
    vetor = (float*) malloc(sizeof(float)*dim);
    if(vetor == NULL){
        fprintf(stderr,"ERRO.. malloc");
        return 2;
    }
    tid = (pthread_t *) malloc(sizeof(pthread_t)*dim);
    if(tid == NULL){
        fprintf(stderr,"ERRO.. malloc");
        return 2;
    }



    srand(time(NULL));
    //inicialização do vetor
    for(int i = 0; i < dim;i++){
        vetor[i] = rand()*9.0/RAND_MAX;
    }


    //fazedor de testes
    for(int j = 0; j < numeroTestes; j++){
        maiorEmenor* resultado;
        resultado = (maiorEmenor*) malloc(sizeof(maiorEmenor));
        if(resultado == NULL){
            fprintf(stderr,"ERRO.. malloc");
            return 2;
        }  
        maiorEmenor concorrente = {.maior = 0, .menor = 1000};
        maiorEmenor sequencial = {.maior = 0, .menor = 1000};

        GET_TIME(inicio);
        //Processo sequencial
        for(long int i = 0; i < dim; i++){
            if(vetor[i] > sequencial.maior)
                sequencial.maior = vetor[i];
            if(vetor[i] < sequencial.menor)
                sequencial.menor = vetor[i];
            
        }
        GET_TIME(fim);
        tempSeq = fim - inicio;


        GET_TIME(inicio);
        //processo concorrente
        //criar as threads
        for(int i = 0; i<nThreads; i++){
            if(pthread_create(tid+i,NULL, tarefa, (void*)i)){
                fprintf(stderr, "ERRO... pthread_create\n");
                return 3;
            }
        }
        //esperar o termino das threads
        for (int i = 0; i < nThreads; i++){
            if(pthread_join(*(tid+i),(void**)&resultado)){
                fprintf(stderr, "ERRO... pthread_join\n");
                return 4;
            }
            if( resultado->maior > concorrente.maior)
                concorrente.maior = resultado->maior;

            if( resultado->menor < concorrente.menor)
                concorrente.menor = resultado->menor;
        }
        GET_TIME(fim);
        tempConq = fim - inicio;
        

        //corretude
        if(sequencial.maior != concorrente.maior || sequencial.menor != concorrente.menor){
            printf("\nOps, algo errado\n");
            return 4;
        }
            
        
        //exibe os resultados        
        // printf("Sequencial:  \n tempo: %lf \n maior: %f \n menor: %f\n",tempSeq, sequencial.maior,sequencial.menor);
        // printf("\nConcorrente: \n tempo: %lf \n maior: %f \n menor: %f\n", tempConq, concorrente.maior,concorrente.menor);
        // printf("\nAproveitamento: %lf\n", tempSeq/tempConq);

        mediaSeq += tempSeq;
        mediaConc += tempConq;
        free(resultado);
    }

    mediaSeq  /= numeroTestes;
    mediaConc /= numeroTestes;
    printf("media sequencial:  %lf\n", mediaSeq);
    printf("media concorrente: %lf\n", mediaConc);
    printf("Aprovitamento médio: %lf\n", mediaSeq/mediaConc);


    //libera as EDs alocadas
    free(vetor);
    free(tid);
    

    return 0;
}