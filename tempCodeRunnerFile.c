#include "ArvoreB.h"
#include <math.h>
void parse(FILE*,struct Btree*);
int main (){
    struct Btree *arvore= NULL;
    FILE *DB= NULL;
    double i;
    i = log(2345);
    parse(DB, arvore);
    printf("tamanho da arvore= %d\n", arvore->QuantidadeDeChaves);
   return 0;
}
void parse(FILE *DB,struct Btree* arvore){
    char str[150];
    int tamanho, ordem, i;
    struct Key chaveprim;
    DB = fopen("data.txt","r");
    fseek(DB,0, 0);
    tamanho=0;
     while(fgets(str,150, DB)!=NULL){
         tamanho++;
     }
    fseek(DB,0,0);
    ordem = CalcularTamanho(tamanho);
    arvore=CriarArvore(tamanho, ordem);
    while(fgets(str,150, DB)!=NULL){
        for(i=0;i<6;i++){
            chaveprim.chave[i]=str[i];
        }
        chaveprim.posicao = (long)ftell;
    }
}

int CalcularTamanho(int tamanho){
    double m=0.0;
    double n,temp;
    n = tamanho/2;
    n = log(n);
    temp=ceil(m/2);
    temp=log(temp);
    while(4.0>(n/temp)){
        m++;
        temp=ceil(m/2);
        temp=log(temp);
    }
    printf("m=%f\n", m);
    sleep(1);
    return (int) m;
}