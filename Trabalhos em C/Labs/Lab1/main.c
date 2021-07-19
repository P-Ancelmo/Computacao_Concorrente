#include <stdio.h>
#include<pthread.h>
#define NTHREADS 2
#define TAMANHOVET 10000

int vetor[TAMANHOVET];

//estrutura para permitir aumentar o programa
struct inicioEfim{
  int inicio;
  int fim;
};

int testeAoQuadrado(int vetEntrada[]){
  int cont = 0;
  for(int i=0;i<TAMANHOVET; i++){    
    if(vetor[i] == vetEntrada[i]*vetEntrada[i])
      cont++;
  }
  if(cont == TAMANHOVET)
    return 1;
  return 0;
}

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
  int vetCopia[TAMANHOVET];

  struct inicioEfim t1 = {.inicio = 1,.fim = TAMANHOVET/2};

  struct inicioEfim t2 = {.inicio = TAMANHOVET/2,.fim = TAMANHOVET};

  int i = 0;

  //inicializando o vetor que será elevado
  for(i = 0; i < TAMANHOVET;i++){
    vetor[i] = i+1;
    vetCopia[i] = i+1;
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
  
  if(testeAoQuadrado(vetCopia))
    printf("tudo certo pessoal\n");
  
  pthread_exit(NULL);
  return 0;

}