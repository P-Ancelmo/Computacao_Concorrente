#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include"timer.h"


int main(int argc, char* argv[]){

    if(argc < 2)
        return 1;

    int dim = atoi(argv[1]);
    int linha, coluna,i;
    int somaprod;
    float *mat1,*mat2,*mat3;
    double inicio, final, tempo;
    
    mat1 = (float*) malloc(sizeof(float)*dim*dim);
    mat2 = (float*) malloc(sizeof(float)*dim*dim);
    mat3 = (float*) malloc(sizeof(float)*dim*dim);

    for(int j = 0; j< dim;j++){
        for(int k=0;k<dim;k++){
            mat1[j*dim+k] = 1;
            mat2[j*dim+k] = 1;
            mat3[j*dim+k] = 0;
        }
    }

    GET_TIME(inicio);
    for(linha=0; linha<dim; linha++){
        for(coluna=0; coluna<dim; coluna++){
        somaprod=0;
        for(i=0; i<dim; i++) somaprod+=mat1[linha*dim+i]*mat2[i*dim+coluna];
        mat3[linha*dim+coluna]=somaprod;
        }
    }
    GET_TIME(final);
    tempo = final - inicio;

    printf("Multiplicação Sequencial: %lfs\n", tempo);

    return 0;

}