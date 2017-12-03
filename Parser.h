#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ArvoreB.h"
void Parse(FILE *DB,struct Btree* arvore){
    char c;
    char str[512];
    int tamanho, NumeroDeChaves, i;
    struct Key chaveprim;
    struct Key *chaveiro;
    DB = fopen("data.txt","r");
    fseek(DB,0, 0);
    tamanho=0;
    while(fgets(str,1000, DB)!=NULL){
        for(i=0;i<6;i++){
            chaveprim.chave[i]=str[i];
        }
        chaveprim.posicao= (long)ftell;
        tamanho = ProjetarArvore(chaveiro,&chaveprim);
    }
    arvore=CriarArvore(tamanho);
    )
}