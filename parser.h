#include "arvoreb.h"

void Parse(FILE *DB,struct Btree* arvore){
    char str[150];
    int tamanho, i;
    struct Key chaveprim;
    DB = fopen("data.txt","r");
    fseek(DB,0, 0);
    tamanho=0;
     while(fgets(str,150, DB)!=NULL){
         tamanho++;
     }
    fseek(DB,0,0);
    arvore=CriarArvore(tamanho);
    sleep(10);
    while(fgets(str,150, DB)!=NULL){
        for(i=0;i<6;i++){
            chaveprim.chave[i]=str[i];
        }
        chaveprim.posicao= (long)ftell;
    }
    
}