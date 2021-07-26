#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include"timer.h"

//matrizes de entrada
float *mat1;
float *mat2;
//matriz de saída
float *saida;

int dimensao;
int nThreads;

void corretude(){
    int contador =0;
    for(int i = 0; i< dimensao;i++){
        for(int j=0;j<dimensao; j++){
            if(saida[i*dimensao+j] == dimensao)
                contador++;            
        }
    }    
    
    if(contador == dimensao*dimensao){
        printf("Tudo certo pessoal!\n");
    }
    return ;
}

void* tarefa(void* arg){
    int id = *(int*) arg;    
    for(int i=id; i < dimensao*dimensao; i+=nThreads){
        for(int j=0;j<dimensao; j++){
            saida[i] += mat1[id*dimensao+j] * mat2[id*dimensao+j];
        }
    }   
    pthread_exit(NULL);
}

int main(int argc, char* argv[]){

    pthread_t *tid;
    double inicio, final,a,b,c, seq;
    int aux = 0;
    //checa entrada
    if(argc < 3){
        printf("Digite %s <dimensão das matrizes> <número de threads>", argv[0]);
        return 1;
    }

    dimensao = atoi(argv[1]);
    nThreads = atoi(argv[2]);

    if(nThreads > dimensao) nThreads = dimensao;

    //Alocação
    mat1 = (float *) malloc(sizeof(float)*dimensao*dimensao);
    if(mat1 == NULL){printf("ERRO.. malloc\n"); return 2;}
    mat2 = (float *) malloc(sizeof(float)*dimensao*dimensao);
    if(mat2 == NULL){printf("ERRO.. malloc\n"); return 2;}
    saida = (float *) malloc(sizeof(float)*dimensao*dimensao);
    if(saida == NULL){printf("ERRO.. malloc\n"); return 2;}
    
    GET_TIME(inicio);
    //Inicialização das matrizes
    for(int i=0; i < dimensao; i++){
        for(int j=0; j< dimensao;j++){
            mat1[i*dimensao+j] = 1;
            mat2[i*dimensao+j] = 1;
            saida[i*dimensao+j] = 0;  
        }
    }
    GET_TIME(final);
    a = final - inicio;
    GET_TIME(inicio);
    //alocação dos ids das threads
    tid = (pthread_t *) malloc(sizeof(pthread_t)*nThreads);
    if(tid == NULL){printf("ERRO... malloc"); return 2;}
    
    int id[nThreads];    

    for(int i=0; i<nThreads; i++){
        id[i] = i;
        
        if(pthread_create(tid+i, NULL, tarefa, (void *) &id[i])){
            printf("ERRO... pthread_create");
            return 3;
        }
    }
    
    for(int i = 0; i < nThreads; i++){
        pthread_join(*(tid+i),NULL);
    }
    
    GET_TIME(final);
    b = (final-inicio);

        
    GET_TIME(inicio);
    corretude();
    free(mat1);
    free(mat2);
    free(saida);
    free(tid);
    GET_TIME(final);
    c = final - inicio;

    printf("Tempo de iniciaçização: %lfs\n", a);
    printf("Tempo de multiplicação Paralela: %lfs\n", b); 
    printf("Tempo de finaçização: %lfs\n", c);
    return 0;
}