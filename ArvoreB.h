#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

typedef struct btree Btree;
typedef struct key Key;
struct key{
    char chave[6];
    long posicao;
};

struct btree{
    Btree* ponteiros;
    Key *chaves;
    int ordem, numerodechaves, numerodoNo, numerodefilhos;
};

int CalcularOrdem(double);

Btree* CriarArvore(double tamanho){
    int ordem;
    Btree *arvore;
    arvore = (Btree*)malloc(sizeof(arvore));
    ordem = CalcularOrdem(tamanho);
    arvore->ordem = ordem;
    arvore->numerodechaves=0;
    // arvore->ponteiros = (Btree*)calloc(ordem, (sizeof arvore));
    // arvore->chaves = (Key*) calloc(ordem-1, sizeof(Key));
    arvore->numerodefilhos=0;
    arvore->numerodoNo=0;
    return arvore;
}
int CalcularOrdem(double tamanho){
    double m=3.0;
    double n,temp;
    n = tamanho/2.0;
    n = log(n);
    temp=ceil(m/2.0);
    temp=log(temp);
    while((1+n/temp)>=4.0){
        m++;
        temp=ceil(m/2);
        temp=log(temp);
    }
    return (int) m;
}
void AdicionarChave(Btree* raiz, Key *chave){
    Key *chaves;

}

void OrdenarNo(Btree* no){

}


void MontarHeap(Key* heap,int pos){//Metodo que efetivamente faz a montagem da heap para o heapsort
    int pai;
    Key temp;
    pai= (pos-1)/2;
    if(heap[pos].chave<heap[pai].chave&& pos!=0){
        temp=heap[pai];
        heap[pai]=heap[pos];
        heap[pos]=temp;
        if(pos!=0) MontarHeap(heap, pai);
    }
}

void Heapsort(Key* chaves, int numerodechaves){//Ordena as chaves secundarias da menor para a maior
    int i,a, heapsize;
    Key *heap, *temp;
    heap = calloc(numerodechaves, sizeof(Key));
    i=0;
    heap = chaves;
    heapsize = numerodechaves;
    i=0;
    MontarHeap(heap, heapsize-1);
    while(heapsize!=0){
        chaves[i]=heap[0];
        a=0;
        temp = calloc(heapsize-1,sizeof(Key));
        while(a<heapsize-1){
            temp[a]=heap[a+1];
            MontarHeap(temp,a);
            a++;
        }
        i++;
        heap=temp;
        heapsize--;
    }

}
void GravarArvore(Btree* atual, FILE *arquivo){
    int i;
    Btree filho;
    fwrite(atual->numerodoNo,sizeof(int),1,arquivo);
    fwrite(atual->numerodechaves,sizeof(int),1,arquivo);
    fwrite(atual->numerodefilhos,sizeof(int),1,arquivo);
    fwrite(atual->chaves,sizeof(Key),atual->numerodechaves,arquivo);
    if(atual->numerodefilhos>0){
        for(i=0;i<atual->numerodefilhos;i++){
            filho = atual->ponteiros[i];
            GravarArvore(&filho,arquivo);
        }
    }
}
void PreGravar(Btree* raiz){
    FILE* arquivo;
    arquivo=fopen("indicelista.bt","wb");
    fwrite(raiz->ordem,sizeof(int),1,arquivo);
    GravarArvore(raiz,arquivo);
    
}
Btree* LerArvore(Btree* atual, FILE *arquivo, int ordem){
    int i;
    Btree filho;
    atual->ordem=ordem;
    fread(atual->numerodoNo,sizeof(int),1,arquivo);
    fread(atual->numerodechaves,sizeof(int),1,arquivo);
    fread(atual->numerodefilhos,sizeof(int),1,arquivo);
    fread(atual->chaves,sizeof(Key),atual->numerodechaves,arquivo);
    if(atual->numerodefilhos>0){
        for(i=0;i<atual->numerodefilhos;i++){
            atual->ponteiros[i] = LerArvore(&filho,arquivo,ordem);
        }
    }
    return atual;
}
void PreLer(Btree* raiz){
    FILE* arquivo;
    arquivo=fopen("indicelista.bt","rb");
    fread(raiz->ordem,sizeof(int),1,arquivo);
    LerArvore(raiz,arquivo, raiz->ordem);
}

long Busca(Btree* no, char* chave){
    int i,a;
    a=strcmp(chave, no->chaves[i].chave);
    i=0;
    while(a>=0 && i< no->numerodechaves){
        if(a==0){
            return no->chaves[i].posicao;
        }
        i++;
        if(i != no->numerodechaves){
            a=strcmp(chave, no->chaves[i].chave);
        }
    }
    if(a<0 || i == no->numerodechaves){
        return Busca(&no->ponteiros[i],chave);
    }
    return -1.0;
}
