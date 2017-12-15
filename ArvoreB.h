#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include "structs.h"

Btree* raiz=NULL;
int ordem=0;
int CalcularOrdem(double);
Btree* insere_simples(Btree*, Key*);
Btree* buscaNo(Btree*, char*);


Btree* CriarArvore(int tamanho){
    raiz = malloc(sizeof(Btree));
    ordem = CalcularOrdem((double)tamanho);
    raiz->chaves = calloc(ordem-1,sizeof(Key));
    raiz->filhos = calloc(ordem,sizeof(Btree*));
    raiz->pai = NULL;
    raiz->numerodechaves=0;
    raiz->numerodefilhos=0;
    return raiz;
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
    return (int) m+2.0;
}

void GravarArvore(Btree* atual, FILE *arquivo){
    int i;
    size_t tam;
    fwrite(&atual->numerodechaves,sizeof(int),1,arquivo);
    fwrite(&atual->numerodefilhos,sizeof(int),1,arquivo);
    while(i<atual->numerodechaves){
        tam= strlen(atual->chaves[i].chave);
        fwrite(&tam,sizeof(size_t),1,arquivo);
        fwrite(&atual->chaves[i].chave,sizeof(char)*strlen(atual->chaves[i].chave),1,arquivo);
        fwrite(&atual->chaves[i].posicao,sizeof(long),1,arquivo);
        i++;
    }
    if(atual->numerodefilhos>0){
        for(i=0;i<atual->numerodefilhos;i++){
            GravarArvore(atual->filhos[i],arquivo);
        }
    }
}

void PreGravar(char *nomeindice){
    FILE* arquivo;
    arquivo=fopen(nomeindice,"wb");
    fwrite(&ordem,sizeof(int),1,arquivo);
    GravarArvore(raiz,arquivo);
    fclose(arquivo);
    
}

void LerArvore(Btree* atual, FILE *arquivo){
    int i;
    size_t temp;
    fread(&atual->numerodechaves,sizeof(int),1,arquivo);
    fread(&atual->numerodefilhos,sizeof(int),1,arquivo);
    atual->chaves = calloc(atual->numerodechaves,sizeof(Key));
    atual->filhos = calloc(atual->numerodefilhos,sizeof(Btree));
    while(i<atual->numerodechaves){
        fread(&temp,sizeof(size_t),1,arquivo);
        atual->chaves[i].chave = malloc(sizeof(char)*temp);
        fread(&atual->chaves[i].chave,sizeof(char)*temp,1,arquivo);
        fread(&atual->chaves[i].posicao,sizeof(long),1,arquivo);
        i++;
    }
    // fread(&atual->chaves,sizeof(Key),atual->numerodechaves,arquivo);
    if(atual->numerodefilhos>0){
        for(i=0;i<atual->numerodefilhos;i++){
            LerArvore(atual->filhos[i],arquivo);
        }
    }
}

int PreLer(char* nomeindice){
    FILE* arquivo;
    arquivo=fopen(nomeindice,"rb");
    if (arquivo == NULL){
        return 0;
    }
    raiz = malloc(sizeof(Btree));
    fread(ordem,sizeof(int),1,arquivo);
    LerArvore(raiz,arquivo);
    return 1;
}

long Busca(Btree* no, char* chave){
    int i,a;
    i=0;
    a=strcmp(chave, no->chaves[i].chave);
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
        if(no->numerodefilhos>0){
            return Busca(no->filhos[i],chave);
        }else{
            printf("merda.\n");
            return -1.0;
        }
    }
    return -1.0;
}

Btree* buscaNo(Btree* no, char* chave){
    int i,a;
    i=0;
    if(no->numerodechaves==0){
        return no;
    }
    a=strcmp(chave, no->chaves[0].chave);
    while(a>=0 && i< no->numerodechaves){
        if(a==0){
            return NULL;
        }
        i++;
        if(i != no->numerodechaves){
            a=strcmp(chave, no->chaves[i].chave);
        }
    }
    if(a<0 || i == no->numerodechaves){
        if(no->numerodefilhos>0){
            return buscaNo(no->filhos[i],chave);
        }
        else if(i== no->numerodechaves){
            return no;
        } else {
            return no;
        }
    }
    return NULL;
}

void dealloc(Btree* no){
    int i;
    if(no->numerodefilhos>0){
        for(i=0;i<no->numerodefilhos;i++){
            dealloc(no->filhos[i]);
        }
    }
    free(no);
}


int cabe_no(Btree *no) {//verifica se ainda ha espaco para inserir um novo registro na pagina
	if (no->numerodechaves < ordem-1) 
		return 1;
	else
		return 0;
}

int lotacao_min(Btree* no){//verifica se o no esta com a lotação minima antes de ser necessaria uma redistribuição/concatenação
    	if (no->numerodechaves < ordem /2) 
		return 1;
	else
		return 0;
}
Btree* AlocarArvore(){// inicializa um novo no
    Btree *no;
    no = malloc(sizeof(Btree));
    no->chaves = calloc(ordem-1,sizeof(Key*));
    no->filhos = calloc(ordem,sizeof(Btree*));
    no->pai = NULL;
    no->numerodechaves=0;
    no->numerodefilhos=0;
    return &no;
}
Key* Cleave(Btree* esquerda, Key **direita,int promovido){
    int i,a;
    Key *Chaveprom;
    i=promovido+1;
    Chaveprom = malloc(sizeof(Key));
    Chaveprom->chave = malloc(sizeof(char)*strlen(esquerda->chaves[promovido].chave)+1);
    strcpy(Chaveprom->chave,esquerda->chaves[promovido].chave);
    Chaveprom->posicao = esquerda->chaves[promovido].posicao;
    a=0;
    // *direita = calloc((esquerda->numerodechaves - i),sizeof(Key**));
    *direita = malloc(sizeof(Key)*(ordem-1));
    while (i< ordem-1){
        (*direita)[a].chave = malloc(sizeof(char)*(strlen(esquerda->chaves[i].chave)+1));
        strcpy((*direita)[a].chave,esquerda->chaves[i].chave);
        (*direita)[a].posicao = esquerda->chaves[i].posicao;
        esquerda->chaves[i].chave=NULL;
        esquerda->chaves[i].posicao=-1;
        i++;
        a++;  
	}
    esquerda->numerodechaves = promovido;
    printf("nchaves=%d\n", esquerda->numerodechaves);
    return Chaveprom;
}
int reposicionar_filhos(Btree *local, Key *novo) {//simplesmente insere um novo registro em um no
	int i, pos;
    Btree **filhostemp;
    i=0;
    filhostemp = calloc(ordem,sizeof(Btree*));
    pos=strcmp(novo->chave, local->chaves[i].chave);
	while (i<local->numerodefilhos-1 && strcmp(novo->chave, local->chaves[i].chave) >0 ){
        filhostemp[i]= &local->filhos[i];
        i++;  
        // printf("chave%d= %s\nnfilhos=%d\n",i, local->chaves[i].chave,local->numerodefilhos);
	}
    filhostemp[i] = NULL;
    pos=i;
    while(i< local->numerodefilhos){
        filhostemp[i+1] = &local->filhos[i];
        i++;
    }
    local->filhos = filhostemp;    
    // printf("pos = %d\n chave-1=%s\nchave=%s\nchave+1=%s\n", pos,local->chaves[pos-1].chave,novo->chave,local->chaves[pos+1].chave);
	return pos;
}
Btree* spliting(Btree **no, Key *chave) {//funcao que realiza o split precisa arrumar
	int pos, registros, i, promovido;
	Btree *nova_raiz= NULL;
    Key* outrofilho= NULL;
    Key* chaveprom= NULL;
	nova_raiz = (*no)->pai;
    if(no==raiz){
        printf("aye\n");
    }
	if (nova_raiz == NULL) {//Caso nao tenha pai
		nova_raiz= AlocarArvore();
		nova_raiz->filhos[0] = no;
		(*no)->pai = nova_raiz;
        nova_raiz->numerodefilhos=1;
	}
    no = insere_simples(no,chave);//força a insersão de um elemento alem do maximo
    promovido= (ordem-1)/2+1;
    chaveprom = Cleave(no, &outrofilho, promovido);
    pos=1;
    if(nova_raiz->numerodefilhos>1){
        pos = reposicionar_filhos(nova_raiz, chaveprom);
    }
    nova_raiz->filhos[pos] = AlocarArvore();//maloca o novo no resultante do split
    nova_raiz->filhos[pos]->pai = nova_raiz;
    nova_raiz->numerodefilhos++;
    for(i=0;i<(ordem-1)/2-2;i++){
        nova_raiz->filhos[pos] = insere_simples(nova_raiz->filhos[pos], &outrofilho[i]);
    }
    if((*no)->numerodefilhos>ordem){
        nova_raiz->filhos[pos]->filhos = calloc(((*no)->numerodefilhos-promovido),sizeof(Btree*));
        registros = (*no)->numerodefilhos;
        for(i=promovido;i<registros;i++){
            nova_raiz->filhos[pos]->filhos[i-promovido] = (*no)->filhos[i];
            (*no)->numerodefilhos--;
            (*no)->filhos[i]= NULL;
            nova_raiz->filhos[pos]->numerodefilhos++;
        }
        // no->filhos = realloc(no->filhos,sizeof(Btree*)*promovido);
    }
    if (nova_raiz->numerodefilhos > ordem){
        printf("numero de netos= %d\nnumerodefilhos= %d\n", nova_raiz->filhos[0]->numerodefilhos, nova_raiz->numerodefilhos);
        // sleep(2);
        return spliting(nova_raiz, chaveprom);
    }else{
        nova_raiz = insere_simples(nova_raiz, chaveprom);
        return nova_raiz;
    }
}

Btree* insere_simples(Btree *local, Key *novo) {//simplesmente insere um novo registro em um no
	int i, a;
    Key *chaveiro;
    i=0;
    if(local->numerodechaves== 0){
        local->chaves->chave = malloc((sizeof(char)*strlen(novo->chave))+1);
        strcpy(local->chaves->chave,novo->chave);
        local->chaves->posicao = novo->posicao;
        local->numerodechaves=1;
        return local;
    }
    chaveiro = calloc(ordem-1, sizeof(Key));
	while (i<local->numerodechaves&& i<ordem-1 && strcmp(novo->chave,local->chaves[i].chave) >0){
        chaveiro[i].chave = malloc(sizeof(char)*strlen(local->chaves[i].chave)+1);
        strcpy(chaveiro[i].chave,local->chaves[i].chave);
        chaveiro[i].posicao = local->chaves[i].posicao;
        i++;  
	}
    chaveiro[i].chave = malloc(sizeof(char)*strlen(novo->chave)+1);
    strcpy(chaveiro[i].chave,novo->chave);
    chaveiro[i].posicao = novo->posicao;
    a=1+i;
    while(i< local->numerodechaves && a<ordem-1){
        chaveiro[a].chave = malloc(sizeof(char)*strlen(local->chaves[i].chave)+1);
        strcpy(chaveiro[a].chave,local->chaves[i].chave);
        chaveiro[a].posicao = local->chaves[i].posicao;
        a++;
        i++;
    }
    free(local->chaves);
    local->chaves = chaveiro;
	local->numerodechaves += 1;//aumenta-se o tamanho do no
	return local;
}
void insere(Key *novo) {
	Btree *local= NULL;
	int i=0;
	local = buscaNo(raiz, novo->chave);// busca local apropriado retorna local
    if(local !=NULL){
        if (cabe_no(local))//se cabe insere normal
        {
            local = insere_simples(local, novo);
        }
        else {
            local = spliting(local, novo);//chama a funcao de split            
        }
    }
}

void concat(Btree *folha, Btree *irmao, int lado){

    if(lado == -1){//caso irmao seja o da esquerda



    }else{//caso irmao seja o da direita


    }

}
void redistribuicao(Btree *folha, Btree *irmao, int eu, int irma){//aplica a redistribuicao, i(1 d, -1 e) � o numero da folha do nova_raiz dela
    Key *nova_raiz, *substituto;
    int i;
    if(irma == -1){// para a irma sendo da esquerda
        nova_raiz = &folha->pai->chaves[eu - 1];
        substituto = &irmao->chaves[irmao->numerodechaves - 1];
        for(i=0 ; i < folha->numerodechaves ; i++ ){//abre espa�o para o nova_raiz q vai entrar
            folha->chaves[i+1] = folha->chaves[i];
        }
        folha->chaves[0] = *nova_raiz;
        folha->numerodechaves = folha->numerodechaves +1;

        //promove o substituto
        folha->pai->chaves[eu - 1] = *substituto;


        irmao->numerodechaves = irmao->numerodechaves - 1;
    }else{//para irma sendo da direita
        nova_raiz = &folha->pai->chaves[eu];
        substituto = &irmao->chaves[0];


        folha->chaves[folha->numerodechaves] = *nova_raiz;

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
            esquerda = folha->pai->filhos[i];
            direita = folha->pai->filhos[i+2];
            irma = esquerda;
            lado = -1;
        }else{
            i = 0;
            direita = folha->pai->filhos[1];
            irma = direita;
            lado = 1;
        }
    }
    if( (ordem / 2 ) -1 < esquerda->numerodechaves && esquerda->numerodechaves < (ordem - 1) ){//verifica se � possivel fazer redistribui�ao com a arvore da esquerda

    redistribuicao(folha, esquerda, i+1, -1);

    }
    else if((ordem / 2 ) -1 <direita->numerodechaves && direita->numerodechaves < (ordem - 1)){

    redistribuicao(folha, direita, i+1, 1);

    }else{

    concat(folha, irma, lado );

    }
    if(folha->pai->numerodechaves < (ordem / 2) ){
        verificacoes(folha->pai);

    }
}

void deleta(Btree *deletado, int posicao){//fornece o no onde esta a chave q queremos deletar e onde ela esta
    deletado->chaves[posicao].chave = NULL;
    deletado->chaves[posicao].posicao = -1;
    deletado->numerodechaves = deletado->numerodechaves-1;

}
Btree* busca_folha (Btree *deletado, int posicao){
    Btree *esquerda, *direita;
    esquerda = deletado->filhos[posicao];
    direita = deletado->filhos[posicao+1];
    while(esquerda->numerodefilhos != 0){
            esquerda = esquerda->filhos[esquerda->numerodechaves-1];
    }
    while(direita->numerodefilhos != 0){
            direita = direita->filhos[0];

    }
    if(esquerda->numerodechaves < direita->numerodechaves){
        return direita;
    }else{
        return esquerda;
    }
}

// Btree* troca_pai(Btree *deletado, int posicao){
//     char *chave_filho;
//     Btree *folha;

//     if(deletado->filhos[posicao]->numerodechaves < deletado->filhos[posicao+1]->numerodechaves){//caso ramo da direita
//         chave_filho = deletado->filhos[posicao+1]->chaves[0].chave;
//         deletado->filhos[posicao+1]->chaves[0].chave = deletado->chaves[posicao].chave;
//         deletado->chaves[posicao].chave = chave_filho;
//         folha = &deletado->filhos[posicao+1];
//         return folha;
//     }else{// caso filho da esquerda
//         chave_filho = deletado->filhos[posicao+1]->chaves[deletado->filhos->numerodechaves].chave;
//         deletado->filhos[posicao+1]->chaves[deletado->filhos->numerodechaves-1].chave = deletado->chaves[posicao].chave;
//         deletado->chaves[posicao].chave = chave_filho;
//         folha = &deletado->filhos[posicao+1];
//         return folha;
//     }
// }

// void exclui_tree(Btree *deletado, int posicao){
//     Btree *folha;
//     char *chave_filho;
//     if(deletado->numerodefilhos == 0){ //verifica se ela � nova_raiz
//         if(deletado->filhos[posicao].numerodefilhos != 0 && deletado->filhos[posicao+1].numerodefilhos != 0){//verifica se ela � nova_raiz de nova_raiz
//            folha = busca_folha(deletado, posicao);
//            if(folha->chaves[folha->numerodechaves+1].chave <deletado->chaves[posicao].chave){ //caso seja a folha da esquerda
//                 chave_filho = folha->chaves[folha->numerodechaves-1].chave;
//                 folha->chaves[folha->numerodechaves-1].chave = deletado->chaves[posicao].chave;
//                 deletado->chaves[posicao].chave = chave_filho;
//                 deleta(folha, folha->numerodechaves-1);

//            }else{//caso seja a folha da direita
//                 chave_filho = folha->chaves[0].chave;
//                 folha->chaves[0].chave = deletado->chaves[posicao].chave;
//                 deletado->chaves[posicao].chave = chave_filho;
//                 deleta(folha, 0);
//             }
//         }else{//caso ela seja nova_raiz, mas n�o nova_raiz de nova_raiz
//             folha = troca_pai(deletado, posicao);
//             if(folha->chaves[folha->numerodechaves-1].chave <deletado->chaves[posicao].chave){ //caso seja a folha da esquerda
//                deleta(folha, folha->numerodechaves-1);
//             }else{// caso seja a folha da direita
//                 deleta(folha,0);
//             }
//         }
//     }else{//caso ela n�o seja nova_raiz
//         folha = deletado;
//         deleta(folha, posicao);
//     }
//     //verifica��es
//     if(folha->numerodechaves > (folha->ordem/2)-1){
//         //fim
//     }else{//nao possui o minimo de chaves

//         verificacoes(folha);

//     }
// }


void exclui(Btree* raiz, FILE* DB) {
    // char *nome_do_arquivo= NULL;
	// long nreg=0;
	// int posicao=0;
	// Btree *deletado = NULL;
	// char *chave =NULL;
    // printf("\033[2J\033[1;1H");
	// printf("insira a chave do registro o qual deseja excluir: ");
	// scanf("%s", chave);
	// getchar();
    // //deletado = busca(*raiz ,chave, posicao);
    // if(deletado != NULL){
    //     nreg = deletado->chaves[posicao].posicao;
    //     // apagaLinha(nome_do_arquivo ,nreg);
    //     // apagaLinha(nome_do_arquivo ,nreg);
    //     exclui_tree(deletado, posicao);
    // }else{
    //     printf("A busca falhou");
    // }
}