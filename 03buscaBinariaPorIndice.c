#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct _Endereco Endereco;

struct _Endereco {
    char logradouro[72];
    char bairro[72];
    char cidade[72];
    char uf[72];
    char sigla[2];
    char cep[8];
    char lixo[2]; 
};

typedef struct _indiceEndereco indiceEndereco;
struct _indiceEndereco
{
    char cep[8];
    unsigned int posicao;
};


int main(int argc, char**argv)
{
  if(argc != 2) {
    fprintf(stderr, "USO: %s [CEP]\n", argv[0]);
    return 1;
  }

	FILE *cep, *ind;
	Endereco e;
	indiceEndereco *ie;
	int qt;
	int c =0;

	printf("Tamanho da Estrutura: %ld\n\n", sizeof(Endereco));
	cep = fopen("cep.dat","rb");
	ind = fopen("indice.dat", "rb");
  if(!cep || !ind) {
    perror("Erro ao abrir arquivo cep.dat ou indice.dat");
    fclose(cep);
    fclose(ind);
    return 1;
  }


	int inicio = 0;
  fseek(ind, 0, SEEK_END);
  int final = (ftell(ind) / sizeof(indiceEndereco)) - 1;

	ie = (indiceEndereco*) malloc((final + 1)*sizeof(indiceEndereco)); // Tamanho em bytes do vetor de índices (quantidade de registros * tamanho da estrutura indiceEndereco)
  
	rewind(cep);

	qt = fread(&e,sizeof(Endereco),1,cep); 
	// Lê o primeiro registro do arquivo e armazena na variável e. 
	// O valor de qt será 1 se a leitura for bem-sucedida, ou 0 se atingir 
	// o final do arquivo.
  while(inicio <= final) {
    c++;
    int meio = (inicio + final) / 2;
    
    fseek(ind, meio * sizeof(indiceEndereco), SEEK_SET);

    if (fread(&ie[meio], sizeof(indiceEndereco), 1, ind) != 1) break; 
    if(strncmp(argv[1], ie[meio].cep, 8) == 0) {

      // Posiciona o ponteiro do arquivo cep.dat no byte correto usando o valor de posicao do índice
      fseek(cep, ie[meio].posicao, SEEK_SET);
      
      if (fread(&e, sizeof(Endereco), 1, cep) != 1) {
        fprintf(stderr, "Erro ao ler o registro do arquivo cep.dat\n");
        fclose(cep);
        fclose(ind);
        free(ie);
        return 1;
      }
      printf("%.72s\n%.72s\n%.72s\n%.72s\n%.2s\n%.8s\n", 
             e.logradouro, e.bairro, e.cidade, e.uf, e.sigla, e.cep);
      break;
    }
    else if(strncmp(argv[1], ie[meio].cep, 8) > 0) { // O que eu quero é MAIOR que o atual, olha pra direita
      inicio = meio + 1;
    }
    else { // O que eu quero é MENOR que o atual, olha pra esquerda
      final = meio - 1;
    }

  }

	printf("Total Lido: %d\n", c);
  fclose(ind);
	fclose(cep);
	free(ie);
  return 0;
}

