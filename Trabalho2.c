#include "Parser.h"

int main (){
    Btree *arvore= NULL;
    FILE *DB= NULL;
    arvore = parse(DB);
    printf("tamanho da arvore= %d\n", arvore->ordem);
   return 0;
}
