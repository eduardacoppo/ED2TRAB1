#include <stdlib.h>
#include "OrdenaExterno.h"

void AbreArqEntrada(ArqEntradaTipo* ArrArqEnt, int Low, int Lim)
{
	char name[10];
	int j = 0, i;
	for (i = Low; i <= Lim; i++)
	{
		sprintf(name, "%i", i);
		ArrArqEnt[j] = fopen(name, "rb");
		j++;
	}
}

ArqEntradaTipo AbrirArquivoDeEntrada(char *arquivoNome, int arquivoTexto) {
	if (arquivoTexto) {
		return fopen(arquivoNome, "rb");
	}
	return fopen(arquivoNome, "rb");
}

ArqEntradaTipo AbreArqSaida(int NBlocos) {
	char bloco[10];
	sprintf(bloco, "%i", NBlocos);
	return fopen(bloco, "wb");
}

int EnchePaginas(int NBlocos, ArqEntradaTipo arquivo, void **Buffer, int *nRegsLidos, int numRegistros, int tamReg, void*(*leituraPersonalizada)(FILE*)) {
	int i;
	void *c;
	*nRegsLidos = 0;
	if (leituraPersonalizada) {
		for (i = 0; i < numRegistros; i++) {
			c = leituraPersonalizada(arquivo);
			if (feof(arquivo)) {
				Buffer[i] = NULL;
				return true;
			}
			Buffer[i] = c;
			(*nRegsLidos)++;
		}
	}
	else {
		for (i = 0; i < numRegistros; i++) {
			c = malloc(tamReg);
			fread(c, tamReg, 1, arquivo);
			if (feof(arquivo)) {
				Buffer[i] = NULL;
				free(c);
				return true;
			}
			Buffer[i] = c;
			(*nRegsLidos)++;
		}
	}
	if (feof(arquivo)) {
		return true;
	}
	return false;
}

void DescarregaPaginas(ArqEntradaTipo ArqSaida, void **Buffer, int numRegistros, int tamReg) {
	int i;
	for (i = 0; i < numRegistros; i++) {
		if (Buffer[i] == NULL) return;
		fwrite(Buffer[i], tamReg, 1, ArqSaida);
		free(Buffer[i]);
	}
}

void* getMenorElemento(void **c, int tam, int *posicao, int(*callback)(const void**, const void**)) {
	int i = 0;
	void *menor;
	*posicao = 0;
	for (i = 0; i <= tam; i++) {
		if (c[i] == NULL) continue;
		menor = c[i];
		*posicao = i;
	}
	for (i = 0; i <= tam; i++) {
		if (c[i] == NULL) continue;
		if (callback((const void**)&menor, (const void**)&c[i]) > 0) {
			menor = c[i];
			*posicao = i;
		}
	}
	c[*posicao] = NULL;
	return menor;
}

void Intercale(ArqEntradaTipo *ArrArqEnt, int Low, int Lim, ArqEntradaTipo ArqSaida, int OrdemIntercalacao, int tamReg, int(*callback)(const void**, const void**))
{
	const int tam = Lim - Low;
	int i, j, posicao;
	void *c1;
	void **c = malloc(sizeof(void*)*OrdemIntercalacao);
	int blocosFinalizados = 0;
	for (i = 0, j = 0; i <= tam; i++) {
		c1 = malloc(tamReg);
		fread(c1, tamReg, 1, ArrArqEnt[i]);
		c[j] = c1;
		j++;
	}
	do {
		c1 = getMenorElemento(c, tam, &posicao, callback);
		fwrite(c1, tamReg, 1, ArqSaida);
		fread(c1, tamReg, 1, ArrArqEnt[posicao]);
		if (feof(ArrArqEnt[posicao])) {
			blocosFinalizados++;
			free(c1);
		}
		else {
			c[posicao] = c1;
		}
	} while (blocosFinalizados <= tam);
	free(c);
}

int Minimo(int Low, int High)
{
	if (Low > High)
	{
		return High;
	}
	return Low;
}

void Apague_Arquivo(int i) {
	char name[10];
	sprintf(name, "%i", i);
	remove(name);
}

int verificaBufferVazio(void **buffer) {
	if (buffer[0] == NULL) {
		return true;
	}
	return false;
}

void MudarNomeArquivo(int arquivo, char* arquivoFinal, int tamReg, void(*escritaPersonalizada)(FILE*, void*)) {

	char *arquivoName;
	FILE *arqSaida, *arqEntrada;
	void *reg;

	reg = malloc(tamReg);
	arquivoName = (char*)malloc(sizeof(char) * 20);
	sprintf(arquivoName, "%i", arquivo);

	if (escritaPersonalizada) {
		arqEntrada = fopen(arquivoName, "rb");
		arqSaida = fopen(arquivoFinal, "w");
		do {
			fread(reg, tamReg, 1, arqEntrada);
			if (feof(arqEntrada)) break;
			escritaPersonalizada(arqSaida, reg);
		} while (1);
	}
	else {
		arqEntrada = fopen(arquivoName, "rb");
		arqSaida = fopen(arquivoFinal, "wb");
		do {
			fread(reg, tamReg, 1, arqEntrada);
			if (feof(arqEntrada)) break;
			fwrite(reg, tamReg, 1, arqSaida);
		} while (1);
	}

	fclose(arqEntrada);
	fclose(arqSaida);

	remove(arquivoName);

	free(arquivoName);
	free(reg);
}

void OrdeneExterno(char *arquivo, char* arquivoFinal, int OrdemIntercalacao, int numRegistros, int tamReg, int(*callback)(const void**, const void**), void*(*leituraPersonalizada)(FILE*), void(*escritaPersonalizada)(FILE*, void*)) {
	int NBlocos = 0;
	ArqEntradaTipo ArqEntrada, ArqSaida;
	ArqEntradaTipo *ArrArqEnt = (ArqEntradaTipo*)malloc(sizeof(ArqEntradaTipo)*OrdemIntercalacao);
	short Fim;
	int Low, High, Lim, nRegsLidos = 0, i, j;
	void **buffer = malloc(sizeof(void*)*numRegistros);
	ArqEntrada = AbrirArquivoDeEntrada(arquivo, leituraPersonalizada ? 1 : 0);
	do   /*Formacao inicial dos NBlocos ordenados */
	{
		Fim = EnchePaginas(NBlocos, ArqEntrada, buffer, &nRegsLidos, numRegistros, tamReg, leituraPersonalizada);
		if (verificaBufferVazio(buffer)) break;
		NBlocos++;
		qsort(buffer, nRegsLidos, sizeof(void*), callback);
		ArqSaida = AbreArqSaida(NBlocos - 1);
		DescarregaPaginas(ArqSaida, buffer, numRegistros, tamReg);
		fclose(ArqSaida);
	} while (!Fim);
	fclose(ArqEntrada);
	Low = 0;
	High = NBlocos - 1;
	while (Low < High) /* Intercalacao dos NBlocos ordenados */
	{
		Lim = Minimo(Low + OrdemIntercalacao - 1, High);
		AbreArqEntrada(ArrArqEnt, Low, Lim);
		High++;
		ArqSaida = AbreArqSaida(High);
		Intercale(ArrArqEnt, Low, Lim, ArqSaida, OrdemIntercalacao, tamReg, callback);
		fclose(ArqSaida);
		for (i = Low, j = 0; i <= Lim; i++, j++) {
			fclose(ArrArqEnt[j]);
			Apague_Arquivo(i);
		}
		Low += OrdemIntercalacao;
	}   //Mudar o nome do arquivo High para o nome fornecido pelo usuario;
	MudarNomeArquivo(High, arquivoFinal, tamReg, escritaPersonalizada);
	free(ArrArqEnt);
	free(buffer);

}
