#include <stdio.h>
#include <stdlib.h>

struct Btree{
    struct Btree* ponteiros;
    char **chaves;
    int QuantidadeDeChaves;
    char Filho;
};

struct Btree* CriarArvore(int tamanho){
    int i;
    struct Btree *arvore;
    arvore = (struct Btree*)malloc(sizeof(arvore));
    arvore->QuantidadeDeChaves = tamanho;
    arvore->ponteiros = (struct Btree*)calloc(tamanho+1, (sizeof &arvore));
    arvore->chaves = (char**) malloc(sizeof(char*)*tamanho);
    for(i=0;i<tamanho;i++){
        arvore->chaves[i] = (char*) malloc(sizeof(char)*7);
    }
    arvore->Filho=1;
    return arvore;
}