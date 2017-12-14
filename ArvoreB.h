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
    while(i<atual->numerodechaves){
        fwrite(strlen(atual->chaves[i].chave),sizeof(size_t),1,arquivo);
        fwrite(&atual->chaves[i].chave,sizeof(char)*strlen(atual->chaves[i].chave),1,arquivo);
        fwrite(&atual->chaves[i].posicao,sizeof(long),1,arquivo);
        i++;
    }
    if(atual->numerodefilhos>0){
        for(i=0;i<atual->numerodefilhos;i++){
            GravarArvore(&atual->filhos[i],arquivo);
        }
    }
}

void PreGravar(Btree* raiz, char *nomeindice){
    FILE* arquivo;
    arquivo=fopen(nomeindice,"wb");
    fwrite(&raiz->ordem,sizeof(int),1,arquivo);
    GravarArvore(raiz,arquivo);
    fclose(arquivo);
    
}

void LerArvore(Btree* atual, FILE *arquivo, int ordem){
    int i;
    size_t temp;
    atual->ordem=ordem;
    fread(&atual->numerodoNo,sizeof(int),1,arquivo);
    fread(&atual->numerodechaves,sizeof(int),1,arquivo);
    fread(&atual->numerodefilhos,sizeof(int),1,arquivo);
    atual->chaves = calloc(atual->numerodechaves,sizeof(Key));
    atual->filhos = calloc(atual->numerodefilhos,sizeof(Btree));
    while(i<atual->numerodechaves){
        fread(temp,sizeof(size_t),1,arquivo);
        atual->chaves[i].chave = malloc(sizeof(char)*temp);
        fread(&atual->chaves[i].chave,sizeof(char)*temp,1,arquivo);
        fread(&atual->chaves[i].posicao,sizeof(long),1,arquivo);
        i++;
    }
    // fread(&atual->chaves,sizeof(Key),atual->numerodechaves,arquivo);
    if(atual->numerodefilhos>0){
        for(i=0;i<atual->numerodefilhos;i++){
            LerArvore(&atual->filhos[i],arquivo,ordem);
        }
    }
}

int PreLer(Btree* raiz, char* nomeindice){
    FILE* arquivo;
    arquivo=fopen(nomeindice,"rb");
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
	int pos, pos2, i, counter, j, state;
	Btree *paizao;
	paizao = (*raiz)->pai;
	if (paizao == NULL) {//Caso nao tenha pai
		malocaARV(&paizao);
		paizao->filhos[0] = **raiz;
		(*raiz)->pai = paizao;
	}
	pos = insere_simples(&paizao, (*raiz)->chaves[((*raiz)->ordem)/2]);//insere o elemento que foi promovido
	malocaARV(&(paizao->filhos[pos + 1]));//maloca o novo no resultante do split
	paizao->filhos[pos + 1].pai = paizao;
	//realizando a divisao de registros
    counter = 0;
    for(j=(int)((*raiz)->ordem)/2+1;j< ((*raiz)->ordem);j++){
	   paizao->filhos[pos + 1].chaves[counter].posicao = (*raiz)->chaves[j].posicao;
	   strcpy(paizao->filhos[pos + 1].chaves[counter].chave, (*raiz)->chaves[j].chave);
       counter++;

    }

	//registros agora possuem novos tamanhos
	paizao->filhos[pos + 1].numerodechaves = (((*raiz)->ordem) % 2 == 0? ((*raiz)->ordem)/2-1 : ((*raiz)->ordem)/2);
	(*raiz)->numerodechaves = (int)(((*raiz)->ordem)/2);

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
		if (local->chaves[i].chave< novo->chave){
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
		i = insere_simples(local, novo);
	}
	else {
        spliting(local);//chama a funcao de split
		i = insere_simples(local, novo);//vai forcar a insercao e vai ficar com o tamanho 5
	}

}
void malocaARV(Btree *no){// inicializa um novo no
    int i;
    no = malloc(sizeof(Btree));
    no->numerodechaves = 0;//tamanho inicial vazio=0
}
void concat(Btree *folha, Btree *irmao, int lado){

    if(lado == -1){//caso irmao seja o da esquerda



    }else{//caso irmao seja o da direita


    }

}
void redistribuicao(Btree *folha, Btree *irmao, int eu, int irma){//aplica a redistribuicao, i(1 d, -1 e) � o numero da folha do pai dela

    Key *pai, *substituto;
    int i;

    if(irma == -1){// para a irma sendo da esquerda
        pai = &folha->pai->chaves[eu - 1];
        substituto = &irmao->chaves[irmao->numerodechaves - 1];


        for(i=0 ; i < folha->numerodechaves ; i++ ){//abre espa�o para o pai q vai entrar
            folha->chaves[i+1] = folha->chaves[i];
        }
        folha->chaves[0] = *pai;
        folha->numerodechaves = folha->numerodechaves +1;

        //promove o substituto
        folha->pai->chaves[eu - 1] = *substituto;


        irmao->numerodechaves = irmao->numerodechaves - 1;
    }else{//para irma sendo da direita
        pai = &folha->pai->chaves[eu];
        substituto = &irmao->chaves[0];


        folha->chaves[folha->numerodechaves] = *pai;

        folha->numerodechaves = folha->numerodechaves + 1;

        for(i=0; i < irmao->numerodechaves;i++){ //retira o espa� do substituto
                irmao->chaves[i] = irmao->chaves[i+1];
        }
        folha->pai->chaves[eu] = *substituto;
    }
}

void verificacoes(Btree *folha){
    int i, lado;
    Btree *direita, *esquerda, *irma;
    char *maior, *menor;
    maior = folha->chaves[folha->numerodechaves - 1].chave;
    menor = folha->chaves[0].chave;
    for(i=0; i<folha->pai->numerodechaves ;i++){//acha chaves irmas
        if(folha->pai->chaves[i].chave < menor){
            esquerda = &folha->pai->filhos[i];
            direita = &folha->pai->filhos[i+2];
            irma = esquerda;
            lado = -1;
        }else{
            i = 0;
            direita = &folha->pai->filhos[1];
            irma = direita;
            lado = 1;
        }
    }
    if( (esquerda->ordem / 2 ) -1 < esquerda->numerodechaves < (esquerda->ordem - 1) ){//verifica se � possivel fazer redistribui�ao com a arvore da esquerda

    redistribuicao(folha, esquerda, i+1, -1);

    }
    else if((direita->ordem / 2 ) -1 < direita->numerodechaves < (direita->ordem - 1)){

    redistribuicao(folha, direita, i+1, 1);

    }else{

    concat(folha, irma, lado );

    }
    if(folha->pai->numerodechaves < (folha->ordem / 2) ){
        verificacoes(folha->pai);

    }
}

void deleta(Btree *deletado, int posicao){//fornece o no onde esta a chave q queremos deletar e onde ela esta
    deletado->chaves[posicao].chave = NULL;
    deletado->chaves[posicao].posicao = NULL;
    deletado->numerodechaves = deletado->numerodechaves-1;

}
Btree* busca_folha (Btree *deletado, int posicao){
    Btree *esquerda, *direita;
    esquerda = &deletado->filhos[posicao];
    direita = &deletado->filhos[posicao+1];
    while(esquerda->numerodefilhos != 0){
            esquerda = &esquerda->filhos[esquerda->numerodechaves-1];
    }
    while(direita->numerodefilhos != 0){
            direita = &direita->filhos[0];

    }
    if(esquerda->numerodechaves < direita->numerodechaves){
        return direita;
    }else{
        return esquerda;
    }
}

Btree* troca_pai(Btree *deletado, int posicao){
    char *chave_filho;
    Btree *folha;

    if(deletado->filhos[posicao].numerodechaves < deletado->filhos[posicao+1].numerodechaves){//caso ramo da direita
        chave_filho = deletado->filhos[posicao+1].chaves[0].chave;
        deletado->filhos[posicao+1].chaves[0].chave = deletado->chaves[posicao].chave;
        deletado->chaves[posicao].chave = chave_filho;
        folha = &deletado->filhos[posicao+1];
        return folha;
    }else{// caso filho da esquerda
        chave_filho = deletado->filhos[posicao+1].chaves[deletado->filhos->numerodechaves].chave;
        deletado->filhos[posicao+1].chaves[deletado->filhos->numerodechaves-1].chave = deletado->chaves[posicao].chave;
        deletado->chaves[posicao].chave = chave_filho;
        folha = &deletado->filhos[posicao+1];
        return folha;
    }
}

void exclui_tree(Btree *deletado, int posicao){
    Btree *folha;
    char *chave_filho;
    if(deletado->numerodefilhos == 0){ //verifica se ela � pai
        if(deletado->filhos[posicao].numerodefilhos != 0 && deletado->filhos[posicao+1].numerodefilhos != 0){//verifica se ela � pai de pai
           folha = busca_folha(deletado, posicao);
           if(folha->chaves[folha->numerodechaves+1].chave <deletado->chaves[posicao].chave){ //caso seja a folha da esquerda
                chave_filho = folha->chaves[folha->numerodechaves-1].chave;
                folha->chaves[folha->numerodechaves-1].chave = deletado->chaves[posicao].chave;
                deletado->chaves[posicao].chave = chave_filho;
                deleta(folha, folha->numerodechaves-1);

           }else{//caso seja a folha da direita
                chave_filho = folha->chaves[0].chave;
                folha->chaves[0].chave = deletado->chaves[posicao].chave;
                deletado->chaves[posicao].chave = chave_filho;
                deleta(folha, 0);
            }
        }else{//caso ela seja pai, mas n�o pai de pai
            folha = troca_pai(deletado, posicao);
            if(folha->chaves[folha->numerodechaves-1].chave <deletado->chaves[posicao].chave){ //caso seja a folha da esquerda
               deleta(folha, folha->numerodechaves-1);
            }else{// caso seja a folha da direita
                deleta(folha,0);
            }
        }
    }else{//caso ela n�o seja pai
        folha = deletado;
        deleta(folha, posicao);
    }
    //verifica��es
    if(folha->numerodechaves > (folha->ordem/2)-1){
        //fim
    }else{//nao possui o minimo de chaves

        verificacoes(folha);

    }
}


void exclui() {
    char *nome_do_arquivo;
	long nreg;
	int posicao;
	Btree *deletado;
	char *chave;
	printf("insira a chave do registro o qual deseja excluir: ");
	scanf("%c", &chave);
	getchar();
    //deletado = busca(*raiz ,chave, posicao);
    if(deletado != NULL){
        nreg = deletado->chaves[posicao].posicao;
        apagaLinha(nome_do_arquivo ,nreg);
        apagaLinha(nome_do_arquivo ,nreg);
        exclui_tree(deletado, posicao);
    }else{
        printf("A busca falhou");
    }
}