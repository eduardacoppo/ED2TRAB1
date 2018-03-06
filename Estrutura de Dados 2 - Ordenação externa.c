// Estrutura de Dados 2 - Ordena��o externa.cpp : Define o ponto de entrada para a aplica��o de console.
//
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <stdio.h>
#include "OrdenaExterno.h"
#include "FuncoesAux.h"
#include <time.h>

int main(int argc, char **argv)
{
	ramdomReg("t.bin", 5);
	OrdeneExterno("t.bin", "f.bin", 2, 10, sizeof(Reg), cmpFuncReg, NULL, NULL);
	printf("%i",checkReg("t.bin", "f.bin"));
	return 0;

	//FILE *csv = fopen("out.csv", "w");

	int f, e, m[3] = { 4, 16,256 }, mi, mAux, i;
	float tempo;
	clock_t start, end;

	for (f = 2; f <= 4; f++) {
		printf("f=%i\n", f);
		printf("n      m/4      m/16     m/256\n");

		//fprintf(csv, "f=%i\nn;m/4;m/16;m/256\n", f);

		for (e = 8; e <= 10; e++) {
			printf("2^%i   ", e);

			//fprintf(csv, "2^%i;", e);

			ramdomReg("teste1.bin", e);
			for (mi = 0; mi < 3; mi++) {
				mAux = pow(2, e) / m[mi];
				tempo = 0;
				for (i = 0; i < 3; i++) {
					start = clock();
					OrdeneExterno("teste1.bin", "final.bin", f, mAux, sizeof(Reg), cmpFuncReg, NULL, NULL);
					end = clock();
					tempo += (float)(end - start) / CLOCKS_PER_SEC;
					//	if(!checkReg("teste1.bin", "final.bin")){
					//		printf("\nOrdenação falhou\nSaindo da aplicacao");
					//		return 1;
					//	}
				}
				tempo /= 3;
				printf("%.5f    ", tempo);
				//fprintf(csv, "%.5f;",tempo);
			}
			printf("\n");
			//fprintf(csv,"\n");
		}
	}

	//fclose(csv);
	_CrtDumpMemoryLeaks();
	return 0;
}
