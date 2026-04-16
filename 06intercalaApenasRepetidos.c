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
	saida = fopen("06saida.dat","wb");

	fread(&ea,sizeof(Endereco),1,a);
	fread(&eb,sizeof(Endereco),1,b);

	// eof -> End Of File
	while(!feof(a) && !feof(b))
	{
		if(compara(&ea,&eb)<0) // ea < eb
		{
			fread(&ea,sizeof(Endereco),1,a);
		}
		else if(compara(&ea,&eb)>0) // ea > eb
		{
			fread(&eb,sizeof(Endereco),1,b);
		} 
		// LINHA NOVA
		else {
			// ea == eb, ambos os arquivos tem o mesmo cep, 
			// se as informacoes sao iguais, entao basta escrever uma vez no arquivo de saida
			// se forem diferentes, o código poderia ser alterado para escrever os dois. 
			fwrite(&ea,sizeof(Endereco),1,saida);
			fread(&ea,sizeof(Endereco),1,a);
			fread(&eb,sizeof(Endereco),1,b);
		}
	}

	// Se um dos arquivos chegar ao fim, o processo de intercalação deve 
	// ser encerrado, pois nao ha mais ceps para comparar e escrever no arquivo de saida.

	/*
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
	*/
	fclose(a);
	fclose(b);
	fclose(saida);
}

