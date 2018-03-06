#ifndef FUNCOESAUX_H_
#define FUNCOESAUX_H_

typedef struct {
	int chave;
	char lixo[27];
}Reg;

int cmpFuncReg(const void **a, const void **b);
int cmpFuncChar(const void** a, const void** b);
int cmpFuncInt(const void **a, const void **b);

void ramdomReg(char *name, int n);
void ramdomInt(char *name, int n);

void printInt();
void printReg(int n, char *fn);

//checagens	
int checkReg(char *, char*);

//entrada e saida personalizada
void* ler(FILE *);
void escreve(FILE *, void *);
#endif