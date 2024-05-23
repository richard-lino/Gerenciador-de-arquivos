/**
*implementação de um protótipo de um sistema gerenciador de arquivos (.txt). O sistema permite a criação, remoção, busca de arquivos, apresenta a situação da memória disponível, permite escrever e procurar a ocorrência de uma palavra nos arquivos criados e de listar os arquivos com a quantidade de blocos utilizados e memória, além de formatar o sistema de arquivos.
*
*
*	Compilação:  gcc main.c gerenciador.c disco.c -o <nome_do_gerenciador>
*	Executar:    ./<nome_do_gerenciador> imagem <tamanho>
*
*							Imagem: é o arquivo contendo a imagem do disco.
*							Tamanho: é o tamanho da imagem em MB.
*
*
*	
* Opções da Main.c:
*
*	criar <arquivo> 	: cria um arquivo com tamanho 0
*	buscar <arquivo>	: busca se o arquivo existe e mostra a memória livre
*	remover <arquivo> 	: remove um arquivo criado
*	escrever <arquivo>	: escreve no arquivo criado
*	procurar <palavra>	: procura a ocorrência de uma palavra nos arquivos e mostra a tabela de ocorrências
*	listar			: imprime uma tabela com o nome dos arquivos, memória e blocos ocipados por cada arquivo.
*	formatar 		: formata o sistema de arquivos
*	sair			: encerra o programa
*/

//--------------------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "disco.h"
#include "gerenciador.h"

#define MAX_STR 256
#define MAX_ARG 32

void formatar();
void listar();
void criar(char *arquivo);
void buscar(char *arquivo);
void remover(char *arquivo);
void escrever(char *arquivo);
void procurar(char *arquivo);


int main(int argc, char **argv) {
  char *imagem;
  int tamanho;
  char linha[MAX_STR];
  char *argumento[MAX_ARG + 1];
  char *token;
  int i, tam;

  tamanho = -1;
  if (argc >= 2 && argc <= 3) {
    imagem = argv[1];
    if (argc > 2) {
      tamanho = atoi(argv[2]) * 2048; /// Cada MB tem 2048 setores.
    }
	}else{
    printf("Uso: %s imagem [tamanho]\n", argv[0]);
    printf("Onde: imagem é o arquivo contendo a imagem do disco.\n");
    printf("      tamanho (opcional) é o tamanho da imagem em MB.\n");
    exit(0);
  }

  if (!bloco_iniciar(imagem, tamanho)) {
    exit(0);
  }

  printf("\n\nArquivo de imagem %s aberto.\n", imagem);
  printf("Tamanho %d setores (%d bytes).\n", bloco_tam(), bloco_tam() * TAM_SETOR);
  
  if (!gerenciador_iniciar()) {
    exit(0);
  }

  while (1) {

		printf("\n\n---------------------MENU-------------------\n-criar <arquivo>\n-buscar <arquivo>\n-remover <arquivo>\n-escrever <arquivo>\n-procurar <palavra>\n-Listar\n-formatar\n-sair\n---------------------------------------------\n\n");

    printf(">> ");
    linha[0] = '\0';
    fgets(linha, MAX_STR, stdin);
    tam = strlen(linha);
    if (tam > 0 && linha[tam - 1] == '\n') {
      linha[tam - 1] = '\0';
    }

    i = 0;
    token = strtok(linha, " ");
    while (token != NULL && i < MAX_ARG) {
      argumento[i] = token;
      i++;
      token = strtok(NULL, " ");
    }
    argumento[i] = NULL;
    
    if (argumento[0] == NULL) {
      continue;
    }
///______________Opção Sair_________________
    if (!strcmp(argumento[0], "sair")) {
			printf("ENCERRADO\n");
      exit(EXIT_SUCCESS);


///______________Opção Formatar_________________
    }else if (!strcmp(argumento[0], "formatar")) {
      formatar();


///______________Opção Listar___________________
    }else if (!strcmp(argumento[0], "listar")) {
      listar();

///______________Opção Criar___________________
    }else if (!strcmp(argumento[0], "criar")) {
      if(i == 2) {
				criar(argumento[1]);
      }else {
				printf("Uso: criar <arquivo>\n");
      }
			
///______________Opção Bsucar___________________
		 }else if (!strcmp(argumento[0], "buscar")) {
      if (i == 2) {
				buscar(argumento[1]);
      }else {
				printf("Uso: buscar <arquivo>\n");
      }


///______________Opção Remover___________________
			}else if (!strcmp(argumento[0], "remover")) {
     		if (i == 2) {
	 				remover(argumento[1]);
      	}else {
					printf("Uso: remover <arquivo>\n");
      	}


///______________Opção Escrever___________________
			}else if (!strcmp(argumento[0], "escrever")) {
     		if (i == 2) {
	 				escrever(argumento[1]);
      	}else {
					printf("Uso: escrever <arquivo>\n");
      	}
///______________Opção Procurar___________________

    }else if (!strcmp(argumento[0], "procurar")) {
      if(i == 2) {
				procurar(argumento[1]);
      }else {
				printf("Uso: procurar <palavra>\n");
      }


///______________Erro_________________
    }else {
      printf("Comando inválido\n");
    }
	
  }//fim while
}//fim main


//-----------------------------------------------------------------------------------------------------------------
///-------------------------------------------------FUNÇÔES---------------------------------------------------------


///____________Função Formatar_________________

void formatar() {
  if (gerenciador_formatar()) {
    printf("Formatação concluída. %d bytes livres.\n", gerenciador_free());
  }
}


///____________Função Listar___________________

void listar() {
  char buffer[4096];
  if (gerenciador_listar(buffer, 4096)) {
    printf("%s", buffer);
    printf("%d bytes livres.\n", gerenciador_free());
  }
}

///____________Função Criar_____________________

void criar(char *arquivo) {
  gerenciador_criar(arquivo);
}

///____________Função Buscar___________________

void buscar(char *arquivo) {
  gerenciador_buscar(arquivo);
  printf("%d bytes livres.\n", gerenciador_free());
	
}

///____________Função escrever _________________
void escrever(char *arquivo) {
  gerenciador_escrever(arquivo);
}


///____________Função Procurar _________________
void procurar(char *arquivo) {
  gerenciador_procurar(arquivo);
}


///____________Função Remover_________________

void remover(char *arquivo) {
  gerenciador_remover(arquivo);
}


//-----------------------------------------------------

