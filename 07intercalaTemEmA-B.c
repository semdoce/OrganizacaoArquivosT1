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
	FILE *a, *b, *saida;
	Endereco ea, eb;

	a = fopen("cep_a.dat","rb");
	b = fopen("cep_b.dat","rb");
	saida = fopen("07saida.dat","wb");

	fread(&ea,sizeof(Endereco),1,a);
	fread(&eb,sizeof(Endereco),1,b);

	// eof -> End Of File
	while(!feof(a) && !feof(b))
	{
		if(compara(&ea,&eb)<0) // ea < eb
		{
			// guarda apenas os ceps que estão presentes no arquivo a, mas não estão presentes no arquivo b.
			fwrite(&ea,sizeof(Endereco),1,saida);
			fread(&ea,sizeof(Endereco),1,a);
		}
		else if(compara(&ea,&eb)>0) // ea > eb
		{
			fread(&eb,sizeof(Endereco),1,b);
		} 
		else { // ea == eb
			fread(&ea,sizeof(Endereco),1,a);
			fread(&eb,sizeof(Endereco),1,b);
		}
	}

	// Se o arquivo b chegou ao fim, todos os itens seguintes do arquivo a serão diferentes,
	// portanto é feita uma leitura sequencial do arquivo a até chegar ao fim.
	
	while(!feof(a))
	 {
	 	fwrite(&ea,sizeof(Endereco),1,saida);
	 	fread(&ea,sizeof(Endereco),1,a);
	 }
	/*
	 while(!feof(b))
	 {
	 	fwrite(&eb,sizeof(Endereco),1,saida);
	 	fread(&eb,sizeof(Endereco),1,b);		
	 }
	*/
	fclose(a);
	fclose(b);
	fclose(saida);
}

