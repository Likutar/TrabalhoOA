#include <stdio.h>
#include <stdlib.h>
#include "ArvoreB.h"

int main (){
    struct Btree *arvore= NULL;
    arvore = CriarArvore(2);
    printf("tamanho da arvore= %d\n", arvore->QuantidadeDeChaves);
    return 0;
}