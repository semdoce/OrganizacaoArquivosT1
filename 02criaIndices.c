#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct _Endereco Endereco;
struct _Endereco
{
	char logradouro[72];
	char bairro[72];
	char cidade[72];
	char uf[72];
	char sigla[2];
	char cep[8];
	char lixo[2]; // Ao Espaço no final da linha + quebra de linha
};

typedef struct _indiceEndereco indiceEndereco;
struct _indiceEndereco
{
    char cep[8];
    unsigned int posicao;
};


int compara(const void *a, const void *b)
{
		return strncmp(((indiceEndereco *)a)->cep, ((indiceEndereco *)b)->cep, 8);
}
int main(int argc, char**argv)
{
	FILE *f, *s;
	Endereco e;
	indiceEndereco *ind;
	int qt;
	int c =0;

	printf("Tamanho da Estrutura: %ld\n\n", sizeof(Endereco));
	f = fopen("cep.dat","rb");
	s = fopen("indice.dat", "wb");

	fseek(f,0,SEEK_END);

	long tamanhoBytes = ftell(f); // Tamanho do arquivo em bytes
	long tamanhoRegistros = tamanhoBytes/sizeof(Endereco); // Quantidade de registros (linhas) do arquivo

	ind = (indiceEndereco*) malloc(tamanhoRegistros*sizeof(indiceEndereco)); // Tamanho em bytes do vetor de índices (quantidade de registros * tamanho da estrutura indiceEndereco)

	rewind(f);

	qt = fread(&e,sizeof(Endereco),1,f); 
	// Lê o primeiro registro do arquivo e armazena na variável e. 
	// O valor de qt será 1 se a leitura for bem-sucedida, ou 0 se atingir 
	// o final do arquivo.

	while(qt > 0)
	{
		strncpy(ind[c].cep, e.cep, 8);
		ind[c].posicao = c*sizeof(Endereco); //300 em 300 bytes, isso é importante pois o arquivo que contem os dados completos tem registros de 300 bytes.
    	c++;
		qt = fread(&e,sizeof(Endereco),1,f); 	
	}

	qsort(ind, c, sizeof(indiceEndereco), compara);
	fwrite(ind, sizeof(indiceEndereco), c, s);
	printf("Total Lido: %d\n", c);
  	fclose(s);
	fclose(f);
	free(ind);
}

