#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

typedef struct btree Btree;
typedef struct key Key;
struct key{
    char chave[6];
    long posicao;
};

struct btree{
    Btree* ponteiros;
    Key *chaves;
    int ordem;
    char Filho;
};

int CalcularOrdem(double);

Btree* CriarArvore(double tamanho){
    int ordem;
    Btree *arvore;
    arvore = (Btree*)malloc(sizeof(arvore));
    ordem = CalcularOrdem(tamanho);
    arvore->ordem = ordem;
    // arvore->ponteiros = (Btree*)calloc(ordem, (sizeof &arvore));
    // arvore->chaves = (Key*) calloc(ordem-1, sizeof(Key));
    arvore->Filho=1;
    return arvore;
}
int CalcularOrdem(double tamanho){
    double m=3.0;
    double n,temp;
    n = tamanho/2.0;
    n = log(n);
    temp=ceil(m/2.0);
    temp=log(temp);
    while((1+n/temp)>=4.0){
        m++;
        temp=ceil(m/2);
        temp=log(temp);
    }
    return (int) m;
}
void AdicionarChave(Btree* raiz, Key *chave){
}

void OrdenarNo(Btree* no){

}