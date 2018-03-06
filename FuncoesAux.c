#include "FuncoesAux.h"
#include <stdio.h>
#include <stdlib.h>

//COMPARAÇÕES
int cmpFuncReg(const void **a, const void **b) {
	Reg **a1, **b1;
	a1 =(Reg**) a;
	b1 =(Reg**) b;
	return (**a1).chave - (**b1).chave;
}

int cmpFuncChar(const void** a, const void** b)
{
	char **a1, **b1;
	a1 = (char**)a;
	b1 = (char**)b;
	return (**a1 - **b1);
}

int cmpFuncInt(const void **a, const void **b) {
	int **a1, **b1;
	a1 =(int**) a;
	b1 =(int**) b;
	return (**a1) - (**b1);
}

//GERADOR ALEATORIO
void ramdomReg(char *name, int n) {
	int i, j, valor = 2;
	for (i = 1; i < n; i++) {
		valor = valor * 2;
	}
	Reg *aux = calloc(sizeof(Reg), 1);
	FILE *f;
	f = fopen(name, "wb");

	for (i = 0; i < valor; i++) {
		aux->chave = rand();
		for (j = 0; j < 27; j++) {
			aux->lixo[j] = rand();
		}
		fwrite(aux, sizeof(Reg), 1, f);
	}
	free(aux);
	fclose(f);
}

void ramdomInt(char *name, int n) {
	int i, valor = 2;
	for (i = 1; i < n; i++) {
		valor = valor * 2;
	}
	int aux;
	FILE *f;
	f = fopen(name, "wb");
	srand(1);
	for (i = 0; i < valor; i++) {
		aux = rand();
		fwrite(&aux, sizeof(int), 1, f);
	}
	fclose(f);
}

//PRINTS
void printInt() {
	FILE *f;
	int x;
	f = fopen("43690", "rb");
	do {
		fread(&x, sizeof(int), 1, f);
		printf("%i\n", x);
	} while (!feof(f));
}

void printReg(int n, char *fn) {
	int i;
	FILE *f;
	f = fopen(fn, "rb");
	Reg aux;
	for (i = 0; i < n; i++) {
		fread(&aux, sizeof(Reg), 1, f);
		printf("%i\n", aux.chave);
	}
	fclose(f);
}

//checagem
int checkReg(char *nameIn, char *nameOut) {
	FILE *fOut, *fIn;
	Reg r;
	int aux = 0, numIn = 0, numOut = 0, test = 0;

	fIn = fopen(nameIn, "rb");
	fOut = fopen(nameOut, "rb");
	do {
		test = fread(&r, sizeof(Reg), 1, fIn);
		if (!test) break;
		numIn++;
	} while (1);

	do {
		test = fread(&r, sizeof(Reg), 1, fOut);
		if (aux > r.chave) {
			return 0;
		}
		if (!test) break;
		aux = r.chave;
		numOut++;
	} while (1);
	if (numIn == numOut) {
		return 1;
	}
	else {
		return 0;
	}
}

//entrada e saida personalizada
void* ler(FILE *arquivo) {
	Reg *c = calloc(sizeof(Reg), 1);
	if (arquivo) {
		do {
			fscanf(arquivo, "%c", &(c->chave));
			if (feof(arquivo)) {
				free(c);
				return NULL;
			}
		} while (c->chave == '\n' || c->chave == '\r');
	}
	return c;
}

void escreve(FILE *arquivo, void* reg) {
	Reg *aux = reg;
	if (arquivo) {
		fprintf(arquivo, "%c\n", aux->chave);
	}
}