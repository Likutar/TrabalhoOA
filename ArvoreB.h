#include <stdio.h>
#include <stdlib.h>
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

struct Btree* CriarArvore(int tamanho){
    struct Btree *arvore;
    arvore = (struct Btree*)malloc(sizeof(arvore));
    arvore->QuantidadeDeChaves = tamanho;
    arvore->ponteiros = (struct Btree*)calloc(tamanho+1, (sizeof &arvore));
    arvore->chaves = (struct Key*) calloc(tamanho, sizeof(struct Key));
    arvore->Filho=1;
    return arvore;
}
int CalcularTamanho(int pos){
    int tamanho;
    
    return tamanho;
}
int ProjetarArvore(struct Key* chaveiro, struct Key* chaveprim){
    int tamanho;
    return tamanho;
}
void AdicionarChave(struct Btree* raiz,struct Key *chave){
    int i;
}