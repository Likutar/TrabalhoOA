#include "Parser.h"

int main (){
    Btree *arvore= NULL;
    FILE *DB= NULL;
    double i;
    i = log(2345);
    parse(DB, arvore);
    printf("tamanho da arvore= %d\n", arvore->QuantidadeDeChaves);
   return 0;
}
