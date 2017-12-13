#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

typedef struct btree Btree;
typedef struct key Key;
struct key{
    char *chave;
    long posicao;

};

struct btree{
    Btree* filhos;
    Btree *pai;
    Key *chaves;
    int ordem, numerodechaves, numerodoNo, numerodefilhos;
};

int CalcularOrdem(double);

Btree* CriarArvore(int tamanho){
    Btree *arvore=NULL;
    arvore = malloc(sizeof(Btree));
    arvore->ordem = CalcularOrdem((double)tamanho);
    arvore->chaves = NULL;
    arvore->filhos = NULL;
    arvore->numerodechaves=0;
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

void GravarArvore(Btree* atual, FILE *arquivo){
    int i;
    fwrite(&atual->numerodoNo,sizeof(int),1,arquivo);
    fwrite(&atual->numerodechaves,sizeof(int),1,arquivo);
    fwrite(&atual->numerodefilhos,sizeof(int),1,arquivo);
    fwrite(&atual->chaves,sizeof(Key),atual->numerodechaves,arquivo);
    if(atual->numerodefilhos>0){
        for(i=0;i<atual->numerodefilhos;i++){
            GravarArvore(&atual->filhos[i],arquivo);
        }
    }
}

void PreGravar(Btree* raiz){
    FILE* arquivo;
    arquivo=fopen("indicelista.bt","wb");
    fwrite(&raiz->ordem,sizeof(int),1,arquivo);
    GravarArvore(raiz,arquivo);
    fclose(arquivo);
    
}

void LerArvore(Btree* atual, FILE *arquivo, int ordem){
    int i;
    atual->ordem=ordem;
    fread(&atual->numerodoNo,sizeof(int),1,arquivo);
    fread(&atual->numerodechaves,sizeof(int),1,arquivo);
    fread(&atual->numerodefilhos,sizeof(int),1,arquivo);
    atual->chaves = calloc(atual->numerodechaves,sizeof(Key));
    atual->filhos = calloc(atual->numerodefilhos,sizeof(Btree*));
    fread(&atual->chaves,sizeof(Key),atual->numerodechaves,arquivo);
    if(atual->numerodefilhos>0){
        for(i=0;i<atual->numerodefilhos;i++){
            LerArvore(&atual->filhos[i],arquivo,ordem);
        }
    }
}

int PreLer(Btree* raiz, char* nome){
    FILE* arquivo;
    arquivo=fopen(nome,"rb");
    if (arquivo == NULL){
        return 0;
    }
    raiz = malloc(sizeof(Btree));
    fread(&raiz->ordem,sizeof(int),1,arquivo);
    LerArvore(raiz,arquivo, raiz->ordem);
    return 1;
}

// long Busca(Btree* no, char* chave){
//     int i,a;
//     i=0;
//     a=strcmp(chave, no->chaves[i].chave);
//     while(a>=0 && i< no->numerodechaves){
//         if(a==0){
//             return no->chaves[i].posicao;
//         }
//         i++;
//         if(i != no->numerodechaves){
//             a=strcmp(chave, no->chaves[i].chave);
//         }
//     }
//     if(a<0 || i == no->numerodechaves){
//         return Busca(&no->filhos[i],chave);
//     }
//     return -1.0;
// }

Btree* BuscaNo(Btree* no, char* chave, int *local){
    int i,a;
    i=0;
    a=strcmp(chave, no->chaves[i].chave);
    while(a>=0 && i< no->numerodechaves){
        if(a==0){
            local =i;
            return no;
        }
        i++;
        if(i != no->numerodechaves){
            a=strcmp(chave, no->chaves[i].chave);
        }
    }
    if(a<0 || i == no->numerodechaves){
        return Busca(&no->filhos[i],chave);
    }
    return NULL;
}

void dealloc(Btree* no){
    int i;
    if(no->numerodefilhos>0){
        for(i=0;i<no->numerodefilhos;i++){
            dealloc(&no->filhos[i]);
        }
    }
    free(no);
}


int cabe_no(Btree *raiz) {//verifica se ainda ha espaco para inserir um novo registro na pagina
	if (raiz->numerodechaves < raiz->ordem) 
		return 1;
	else
		return 0;
}

int lotacao_min(Btree* no){//verifica se o no esta com a lotação minima antes de ser necessaria uma redistribuição/concatenação
    	if (no->numerodechaves < no->ordem /2) 
		return 1;
	else
		return 0;
}

void spliting(Btree **raiz) {//funcao que realiza o split precisa arrumar
	int pos, pos2, i;
	Btree *paizao;
	paizao = (*raiz)->pai;
	if (paizao == NULL) {//Caso nao tenha pai
		malocaARV(&paizao);
		paizao->filhos[0] = raiz;
		(*raiz)->pai = paizao;
	}

	pos = insere_simples(&paizao, (*raiz)->chaves[2]);//insere o elemento que foi promovido


	malocaARV(&(paizao->filhos[pos + 1]));//maloca o novo no resultante do split
	paizao->filhos[pos + 1].pai = paizao;

	//realizando a divisao de registros
	paizao->filhos[pos + 1].chaves[0].posicao = (*raiz)->chaves[3].posicao;
	strcpy(paizao->filhos[pos + 1].chaves[0].chave, (*raiz)->chaves[3].chave);
	paizao->filhos[pos + 1].chaves[1].posicao = (*raiz)->chaves[4].posicao;
	strcpy(paizao->filhos[pos + 1].chaves[1].chave, (*raiz)->chaves[4].chave);

	paizao->filhos[pos + 1].filhos[0] = (*raiz)->filhos[3];
	paizao->filhos[pos + 1].filhos[1] = (*raiz)->filhos[4];
	paizao->filhos[pos + 1].filhos[2] = (*raiz)->filhos[5];

	//registros agora possuem novos tamanhos
	paizao->filhos[pos + 1].tamanho = 2;
	(*raiz)->tamanho = 2;

	//se o pai encher, precisamos de um novo split
	if (paizao->numerodechaves == paizao->ordem/2)
		spliting(&paizao);
}


void desloca_arv(Btree **raiz, int pos) {//desloca os registros em um no para a insercao de um novo registro
	int i;
	for (i = (*raiz)->numerodechaves; i>pos; i--)
	{
		(*raiz)->filhos[i + 1] = (*raiz)->filhos[i];
		(*raiz)->chaves[i].posicao = (*raiz)->chaves[i - 1].posicao;
		strcpy((*raiz)->chaves[i].chave, (*raiz)->chaves[i - 1].chave);
	}
}

int insere_simples(Btree *local, Key *novo) {//simplesmente insere um novo registro em um no
	int i;
	for (i = 0; i<local->numerodechaves; i++)
	{
		if (local->chaves[i].chave< novo->chave)
		{
			break;
		}
	}
	desloca_arv(local, i);
	local->chaves[i].posicao = novo->posicao;
    local->chaves[i].chave = novo->chave;
	local->numerodechaves += 1;//aumenta-se o tamanho do no
	return i;
}
void insere(Btree *raiz, Key *novo) {
	Btree *local;
	int i;
	local = busca(raiz, novo->chave);// busca local apropriado retorna local
	if (cabe_no(local))//se cabe insere normal
	{
		i = insere_simples(&local, novo);
	}
	else {
		i = insere_simples(&local, novo);//vai forcar a insercao e vai ficar com o tamanho 5
		spliting(&local);//chama a funcao de split
	}
}