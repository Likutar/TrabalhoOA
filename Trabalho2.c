#include "ArvoreB.h"

Btree* parse(FILE*);

int main (){
    Btree *arvore= NULL;
    FILE *DB= NULL;
    arvore = parse(DB);
    printf("tamanho da arvore= %d\n", arvore->ordem);
   return 0;
}

Btree* parse(FILE *DB){
    char str[150];
    Btree *arvore;
    double tamanho;
    int i;
    Key chaveprim;
    DB = fopen("data.txt","r");
    fseek(DB,0, 0);
    tamanho=0.0;
     while(fgets(str,150, DB)!=NULL){
         tamanho++;
     }
    fseek(DB,0,0);
    arvore=CriarArvore(tamanho);
    while(fgets(str,150, DB)!=NULL){
        for(i=0;i<6;i++){
            chaveprim.chave[i]=str[i];
        }
        chaveprim.posicao = (long)ftell;
    }
    printf("chaveprimaria= %s\n", chaveprim.chave);
    return arvore;
}
