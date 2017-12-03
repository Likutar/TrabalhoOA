#include "parser.h"

int main (){
    struct Btree *arvore= NULL;
    FILE *DB= NULL;
    Parse(DB, arvore);
    printf("tamanho da arvore= %d\n", arvore->QuantidadeDeChaves);
   return 0;
}