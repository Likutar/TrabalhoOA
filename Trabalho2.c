#include "ArvoreB.h"

Btree* parse(FILE*);

int main (){
    Btree *arvore= NULL;
    FILE *DB= NULL;
    arvore = parse(DB);
    printf("tamanho da arvore= %d\n", arvore->ordem);
    dealloc(arvore);
   return 0;
}

Btree* parse(FILE *DB){
    char str[150];
    Btree *arvore= NULL;
    int i, tamanho;
    Key chaveprim;
    DB = fopen("data.txt","r");
    fseek(DB,0, 0);
    tamanho=0;
     while(fgets(str,150, DB)!=NULL){
         tamanho++;
     }
    fseek(DB,0,0);
    arvore=CriarArvore(tamanho);
    
    while(fgets(str,150, DB)!=NULL){
        for(i=0;i<6;i++){
            chaveprim.chave[i]=str[i];
        }
        chaveprim.posicao = (long)ftell;
    }
    printf("chaveprimaria= %s\n", chaveprim.chave);
    return arvore;
}


void insere_registro(Btree **raiz) {
	struct key novo;
	int k;
	char policyID[6];
	char statecode[2];
	char county[15];
	char eq_site_limit[15];
	char hu_site_limit[15];
	char fl_site_limit[15];
	char fr_site_limit[15];
	char tiv_2011[15];
	char tiv_2012[15];
	char eq_site_deductible[15];
	char hu_site_deductible[15];
	char fl_site_deductible[15];
	char fr_site_deductible[15];
	char point_latitude[15];
	char point_longitude[15];
	char line[15];
	char construction[15];
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
	if (p == NULL) {
		printf("Nao conseguiu abrir!\n");
	}
	do {
		c = getc(p);
		if (c == '\n') {
			k++;
		}
	} while (c != EOF);

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
	insere(raiz,novo);
}