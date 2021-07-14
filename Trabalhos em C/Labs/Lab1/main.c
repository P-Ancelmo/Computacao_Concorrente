#include <stdio.h>
#include<pthread.h>
#define NTHREADS 2
#define TAMANHOVET 100

int vetor[TAMANHOVET];

//estrutura para permitir aumentar o programa
struct inicioEfim{
  int inicio;
  int fim;
};

void * tarefa(void * arg){
  struct inicioEfim func = *(struct inicioEfim *) arg;
  for(int i = func.inicio; i < func.fim;i++){
    vetor[i] = vetor[i]*vetor[i];    
  }
  pthread_exit(NULL);
}

int main(){
  pthread_t tid[NTHREADS];
  int funcs[NTHREADS];

  struct inicioEfim t1 = {.inicio = 1,.fim = TAMANHOVET/2};

  struct inicioEfim t2 = {.inicio = TAMANHOVET/2,.fim = TAMANHOVET};

  int i = 0;
  for(i = 0; i < NTHREADS; i++){
    funcs[i] = i+1;
  }

  //inicializando o vetor que será elevado
  for(i = 0; i < TAMANHOVET;i++){
    vetor[i] = i+1;
  }
  
  if(pthread_create(&tid[0], NULL, tarefa, (void*)&t1)){
    printf("ERRO... pthread_create");
  }

  if(pthread_create(&tid[1], NULL, tarefa, (void*)&t2)){
    printf("ERRO... pthread_create");
  }

  for(int i = 0; i < NTHREADS;i++){    
    if(pthread_join(tid[i], NULL)){
      printf("ERRO... pthread_join");
    }
  }

  //imprimo o vetor para verificação
  for(i = 0; i < TAMANHOVET; i++){
    printf("%d ", vetor[i]);
  }
  printf("\n");
  pthread_exit(NULL);
  return 0;

}