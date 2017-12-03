#include <stdio.h>
#include <stdlib.h>
#include "Parser.h"

int main (){
    struct Btree *arvore= NULL;
    FILE *DB;
    Parse(DB, arvore);
    printf("tamanho da arvore= %d\n", arvore->QuantidadeDeChaves);
    return 0;
}