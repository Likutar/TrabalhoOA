#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

struct Key{
    char chave[6];
    long posicao;
};
struct Btree{
    struct Btree* ponteiros;
    struct Key *chaves;
    int QuantidadeDeChaves;
    char Filho;
};

int CalcularTamanho(int);
struct Btree* CriarArvore(int tamanho){
    struct Btree *arvore;
    int ordem;
    arvore = (struct Btree*)malloc(sizeof(arvore));
    ordem = CalcularTamanho(tamanho);
    arvore->QuantidadeDeChaves = tamanho;
    arvore->ponteiros = (struct Btree*)calloc(ordem, (sizeof &arvore));
    arvore->chaves = (struct Key*) calloc(ordem-1, sizeof(struct Key));
    arvore->Filho=1;
    return arvore;
}
int CalcularTamanho(int tamanho){
    int m=0;
    double N;
    N = tamanho/2;
    N = log(N);
    while(4.0>(N/log(ceil(m/2)))){
        m++;
    }
    printf("m=%d\n", m);
    sleep(1);
    return m;
}
void AdicionarChave(struct Btree* raiz,struct Key *chave){
}