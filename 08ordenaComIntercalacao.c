#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Endereco Endereco;

struct _Endereco
{
	char logradouro[72];
	char bairro[72];
	char cidade[72];
	char uf[72];
	char sigla[2];
	char cep[8];
	char lixo[2];
};

int compara(const void *e1, const void *e2)
{
	return strncmp(((Endereco*)e1)->cep,((Endereco*)e2)->cep,8);
}

int main(int argc, char** argv)
{
	FILE *a;
	Endereco *ea;
	long long int qtb, qtdReg, tamBytes;
	int n = 8;
	int sobra;
	char nomeArq[30];
	a = fopen("cep.dat","rb"); //arquivo de 200mb
	fseek(a,0,SEEK_END);
	tamBytes = ftell(a);
	qtdReg = tamBytes/sizeof(Endereco); // Quantidade de registros no arquivo.
	qtb = qtdReg/n; // Qtd de registros por bloco do arquivo.
	sobra = qtdReg%n; // Se houver sobra na divisão, ela será distribuida entre os blocos que serão alocados com qtb+1.
	
	ea = malloc(sizeof(Endereco)*(qtb+1));
	rewind(a);

	for(int i=0; i<n; i++) // de 0 à 7
	{
		int qt = qtb + (i<sobra? 1 : 0); // Se o bloco for menor que a sobra, ele recebe um item a mais.
		
		fread(ea,sizeof(Endereco)*qt,1,a); //Lê 1 bloco
		qsort(ea,qt,sizeof(Endereco),compara); // Ordena o bloco
		sprintf(nomeArq,"08bloco_%d.dat",i); // Cria um nome para o arquivo do bloco
		FILE *saida = fopen(nomeArq,"wb");
		fwrite(ea,sizeof(Endereco)*qt,1,saida); // Escreve o bloco ordenado no arquivo do bloco
		fclose(saida); // Fecha o arquivo do bloco, pra evitar problemas de acesso concorrente.
	}

	free(ea);
	fclose(a);
	
	int prox = 0, ultimo = n; // Índices para controle dos blocos a serem intercalados.
	while(prox<ultimo-1){
		char nomeArq1[30];
		char nomeArq2[30];
		char nomeArq3[30];
		sprintf(nomeArq1,"08bloco_%d.dat",prox);
		sprintf(nomeArq2,"08bloco_%d.dat",prox+1);
		sprintf(nomeArq3,"08bloco_%d.dat",ultimo);
		FILE *a = fopen(nomeArq1,"rb");
		FILE *b = fopen(nomeArq2,"rb");
		FILE *saida = fopen(nomeArq3,"wb");

		Endereco ea, eb;
		fread(&ea,sizeof(Endereco),1,a);
		fread(&eb,sizeof(Endereco),1,b);

		// eof -> End Of File
		while(!feof(a) && !feof(b))
		{
			if(compara(&ea,&eb)<0) // ea < eb
			{
				fwrite(&ea,sizeof(Endereco),1,saida);
				fread(&ea,sizeof(Endereco),1,a);
			}
			else if(compara(&ea,&eb)>0) // ea > eb
			{
				fwrite(&eb,sizeof(Endereco),1,saida);
				fread(&eb,sizeof(Endereco),1,b);
			} 
			else { // ea == eb
				fwrite(&ea,sizeof(Endereco),1,saida);
				fread(&ea,sizeof(Endereco),1,a);
				fread(&eb,sizeof(Endereco),1,b);
			}
		}

		while(!feof(a))
		{
			fwrite(&ea,sizeof(Endereco),1,saida);
			fread(&ea,sizeof(Endereco),1,a);		
		}
		
		while(!feof(b))
		{
			fwrite(&eb,sizeof(Endereco),1,saida);
			fread(&eb,sizeof(Endereco),1,b);		
		}

		fclose(a);
		fclose(b);
		fclose(saida);

		prox = prox + 2; // caminha aos pares de blocos
		ultimo++; // criado um novo bloco por vez, quando próx for maior, significa que não há mais pares
	}
	
}