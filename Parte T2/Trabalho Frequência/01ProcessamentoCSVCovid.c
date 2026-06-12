#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "CSVParser.h"

// emcc Teste2.c CSVParser.c -lnoderawfs.js -lnodefs.js
struct Dados{
  int numCasos;
  int numMortes;
};
typedef struct Dados Tdados;

#define READ_BUF_SIZE 8192

void callback(char** cols, int ncols, void* userData) {
    Tdados* dados = (Tdados*) userData;
        if(strcmp(cols[1],"South America") == 0){
          dados->numCasos += atoi(cols[5]);
          dados->numMortes += atoi(cols[8]);
        }
}

int main() {    
  Tdados dados;
  dados.numCasos=0;
  dados.numMortes=0;
  int qt=0;
  unsigned int total = 0;
  char* buf = (char*) malloc(READ_BUF_SIZE);
  CSVParser csv;
  CSVParser_init(&csv);
  FILE *f = fopen("./owid-covid-data.csv","rb");
  if(f) {
    qt = fread(buf,1,READ_BUF_SIZE,f);
    while(qt > 0) {
      total += qt;
      CSVParser_processLines(&csv,buf,qt,callback,&dados);
      qt = fread(buf,1,READ_BUF_SIZE,f);
    }
    fclose(f);
    CSVParser_processLines(&csv,"\n",1,callback,&dados);
  }
   printf("Numero total de casos: %d\n", dados.numCasos);
   printf("Numero total de mortes: %d\n", dados.numMortes);
   printf("A cada %.2f casos, um leva a morte\n", (float)dados.numCasos/dados.numMortes);
  free(buf);
  return 0;
}
