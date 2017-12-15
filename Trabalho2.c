#include "ArvoreB.h"

void parse(FILE*);

void escolher_busca();
void insere_registro();
int main (){
    // Btree *arvore= NULL;
    FILE *DB= NULL;
    char loop=1;
    char nome[30];
    char *nomeindice;
    int sel;
    printf("\033[2J\033[1;1H");
    printf("Qual arquivo deseja abrir?\n");
    printf("1) data.txt\n2) outro\n");
    scanf("%d", &sel);
    if (sel== 2){
        printf("Qual o nome do arquivo?\n");
        scanf("%s",nome);
        nomeindice = malloc(sizeof(char)*(strlen(nome)+12));
        sel=0;
        while(nome[sel]!='.'){
            nomeindice[sel]= nome[sel];
            sel++;
        }
        strcat(nomeindice, "_indicelista.bt");
        if(PreLer(nomeindice)==0){
            DB = fopen(nome,"r");
            parse(DB);
        }
    }
    else{
        if(PreLer("indicelista.bt")==0){
            DB = fopen("data.txt","r");
            parse(DB);
        }
    }
    while (loop == 1){
        printf("\033[2J\033[1;1H");
        printf("Qual operação deseja realizar\n1) Buscar\n2) adicionar\n3)excluir\n4)alterar\n5)sair");
        scanf("%d", &sel);
        
        switch(sel){
            case 1:
                escolher_busca();
                break;
            case 2:
                insere_registro();
                break;
            case 3:
                exclui(raiz,DB);
                break;
            case 4:
            default:
                loop = 0;
        }
    }
    printf("tamanho da arvore= %d\n", ordem);
    dealloc(raiz);
   return 0;
}

void parse(FILE *DB){
    char str[200];
    int i, tamanho;
    Key chaveprim;
    fseek(DB,0, 0);
    tamanho=0;
     while(fgets(str,200, DB)!=NULL){
         tamanho++;
     }
    fseek(DB,0,0);
    CriarArvore(tamanho);
    while(fgets(str,200, DB)!=NULL){
        i=0;
        while(str[i]!=',' && str[i]!= '\n'){
            i++;
        }
        if(str[i]!= ','){
            printf("ERRO: arquivo no formato errado, ou chave primaria muito grande\n");
            return NULL;
        }
        tamanho =i+1;
        chaveprim.chave = malloc(sizeof(char)* tamanho);
        strncpy(chaveprim.chave,str, i);
        chaveprim.chave[i]='\0';
        chaveprim.posicao = (long)ftell;
        insere(&chaveprim);

    }
}
void strload(char* str, char *temp, int *i){
    int posicao=0;
    memset(temp, ' ', 15);
    while(str[*i]!=','){
        temp[posicao]=str[*i];
        *i++;
    }
    *i++;
}
void escolher_busca(){
    char chave[7], str[150], temp[15], sel;
    long posicao;
    int i=0;
    FILE *DB;
    sel ='s';
    while(sel =='s'){
        printf("\033[2J\033[1;1H");
        printf("Qual a chave do registro que deseja achar?\n");
        scanf("%s", chave);
        posicao = Busca(raiz, chave);
        if(posicao != -1.0){
            DB = fopen("data.txt", "r");
            fseek(DB,posicao , 0);
            fgets(str,150, DB);
            strload(str, temp, &i);
            printf("\033[2J\033[1;1H");
            printf("policyID: %s\n", temp);
            strload(str, temp, &i);
            printf("Statecode:%s\n", temp);
            strload(str, temp, &i);
            printf("County: %s\n", temp);
            strload(str, temp, &i);
            printf("eq_site_limit: %s\n", temp);
            strload(str, temp, &i);
            printf("hu_site_limit: %s\n", temp);
            strload(str, temp, &i);
            printf("fl_site_limit: %s\n", temp);
            strload(str, temp, &i);
            printf("fr_site_limit: %s\n", temp);
            strload(str, temp, &i);
            printf("tiv_2011: %s\n", temp);
            strload(str, temp, &i);
            printf("tiv_2012: %s\n", temp);
            strload(str, temp, &i);
            printf("eq_site_deductible: %s\n", temp);
            strload(str, temp, &i);
            printf("hu_site_deductible: %s\n", temp);
            strload(str, temp, &i);
            printf("fl_site_deductible: %s\n", temp);
            strload(str, temp, &i);
            printf("fr_site_deductible: %s\n", temp);
            strload(str, temp, &i);
            printf("point_latitude: %s\n", temp);
            strload(str, temp, &i);
            printf("point_longitude: %s\n", temp);
            strload(str, temp, &i);
            printf("line: %s\n", temp);
            strload(str, temp, &i);
            printf("construction: %s\n", temp);
            strload(str, temp, &i);
            printf("point_granularity: %s\n", temp);
            printf("deseja procurar outro?[s/n]\n");
            scanf("%c", &sel);
        }
        else{
            printf("\033[2J\033[1;1H");
            printf("registro nao encontrado, deseja procurar outro?[s/n]\n");
            scanf("%c",&sel);
        }
    }
}

void insere_registro() {
	Key novo;
	int k=0;
	char policyID[6], statecode[2], county[15], eq_site_limit[15], hu_site_limit[15];
	char fl_site_limit[15], fr_site_limit[15], tiv_2011[15], tiv_2012[15];
	char eq_site_deductible[15], hu_site_deductible[15], fl_site_deductible[15], fr_site_deductible[15];
	char point_latitude[15], point_longitude[15], line[15], construction[15];
	char point_granularity[15];

	printf("Digite o policyID");
	scanf("%s", policyID);
	getchar();
	printf("Digite o statecode");
	scanf("%s", statecode);
	getchar();
	printf("Digite o county");
	scanf("%s", county);
	getchar();
	printf("Digite o eq_site_limit");
	scanf("%s", eq_site_limit);
	getchar();
	printf("Digite o hu_site_limit");
	scanf("%s", hu_site_limit);
	getchar();
	printf("Digite o fl_site_limit");
	scanf("%s", fl_site_limit);
	getchar();
	printf("Digite o fr_site_limit");
	scanf("%s", fr_site_limit);
	getchar();
	printf("Digite o tiv_2011");
	scanf("%s", tiv_2011);
	getchar();
	printf("Digite o tiv_2012");
	scanf("%s", tiv_2012);
	printf("Digite o eq_site_deductible");
	scanf("%s", eq_site_deductible);
	getchar();
	printf("Digite o hu_site_deductible");
	scanf("%s", hu_site_deductible);
	getchar();
	printf("Digite o fl_site_deductible");
	scanf("%s", fl_site_deductible);
	getchar();
	printf("Digite o fr_site_deductible");
	scanf("%s", fr_site_deductible);
	getchar();
	printf("Digite o point_latitude");
	scanf("%s", point_latitude);
	getchar();
	printf("Digite o point_longitude");
	scanf("%s", point_longitude);
	getchar();
	printf("Digite o line");
	scanf("%s", line);
	getchar();
	printf("Digite o construction");
	scanf("%s", construction);
	getchar();
	printf("Digite o point_granularity");
	scanf("%s", point_granularity);
	getchar();

	FILE *p;
	p = fopen("FL_insurance_sample.csv", "a+");
	// if (p == NULL) {
	// 	printf("Nao conseguiu abrir!\n");
	// }
	// do {
	// 	c = getc(p);
	// 	if (c == '\n') {
	// 		k++;
	// 	}
	// } while (c != EOF);

	fprintf(p, "%s,", policyID);
	fprintf(p, "%s,", statecode);
	fprintf(p, "%s,", county);
	fprintf(p, "%s,", eq_site_limit);
	fprintf(p, "%s,", hu_site_limit);
	fprintf(p, "%s,", fl_site_limit);
	fprintf(p, "%s,", fr_site_limit);
	fprintf(p, "%s,", tiv_2011);
	fprintf(p, "%s,", tiv_2012);
	fprintf(p, "%s,", eq_site_deductible);
	fprintf(p, "%s,", hu_site_deductible);
	fprintf(p, "%s,", fl_site_deductible);
	fprintf(p, "%s,", fr_site_deductible);
	fprintf(p, "%s,", point_latitude);
	fprintf(p, "%s,", point_longitude);
	fprintf(p, "%s,", line);
	fprintf(p, "%s,", construction);
	fprintf(p, "%s\n", point_granularity);

	fclose(p);
	// inserir na arvore
	novo.posicao = k;
    strcpy(novo.chave, policyID);
	insere(&novo);
}


