# Organização de Arquivos
### Parte T1

* **Arquivo 01** : Busca Binária em arquivo de CEP Ordenado
* **Arquivo 02 e 03** : Criação e uso de índice
* **Arquivo 08** : Ordenação Externa com k = 8 blocos.

**Outros arquivos** : Testes extras.

### Parte T2
* **Frequência - Casos e mortes causadas pela Covid na américa do sul**:

```
gcc -o 01ProcessamentoCSVCovid  01ProcessamentoCSVCovid.c CSVParser.c
./01ProcessamentoCSVCovid
```

* **Árvore B**:
    * **Arquivo 01** : Cria dois arquivos baseados no "cep.dat", disponível em aula, com 80% dos registros de CEPs, organizados aleatoriamente.
    * **Arquivo 02** : Cria um arquivo com a estrutura de uma Árvore B, baseando-se no arquivo "cep-1.dat".
    * **Arquivo 03** : Busca registros coincidentes tanto no "cep-2.dat" quanto na Árvore B feita com o "cep-1.dat".

```
//Cria os arquivos cep-1.dat e cep-2.dat
gcc -o 01AleatorizaCep 01AleatorizaCep.c 
./01AleatorizaCep

//Cria o hash da Arvore B consumindo o cep-1.dat
gcc -o 02CriaArvoreBcomCep1 02CriaArvoreBcomCep1.c ArvoreB.c
./02CriaArvoreBcomCep1 

//Procura se os registros de cep-2 existem na Árvore B 
gcc -o 03JoinArvoreBcomCep 03JoinArvoreBcomCep.c ArvoreB.c
./03JoinArvoreBcomCep```
