typedef struct btree Btree;
typedef struct key Key;
struct key{
    char *chave; 
    long posicao;

};

struct btree{
    Btree** filhos;
    Btree *pai;
    Key *chaves;
    int numerodechaves, numerodefilhos;
};