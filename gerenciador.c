
#include <stdio.h>
#include <string.h>
#include "disco.h"
#include "gerenciador.h"

#define TAM_FAT 65536

unsigned short fat[TAM_FAT];

typedef struct {
    char usado;  	   /*!<campo utilizado para verificar se a posião está sendo ocupada */
    char nome[25];         /*!<campo usado para armazenar o nome do arquivo */
    char texto[1000];  	   /*!<campo usado para armazenar o texto na opção escrita no arquivo*/
    int ocorrencia;        /*!<campo usado para verificar a ocorrência de uma palavra na opção procurar no arquivo  */
    unsigned short bloco;  /*!<campo usado para verificar o bloco utilizado */
    int tamanho;	   /*!<campo usado para armazenar o tamanho do arquivo */
} dir_entrada;

dir_entrada dir[128];

int formatado = 1;


///__________Função Iniciar______________________

int gerenciador_iniciar() {

		int i =0,j=0;
		char *buffer;
		
	//__________inicializar a struct diretorio_______
	for(i=0; i < 128;i++){
		dir[i].usado = 0;
		dir[i].bloco = 0;
		dir[i].nome[0] = '\0';
		dir[i].tamanho = 0;
	}buffer = (char *) fat;

	//_______fazer leitura da FAT__________
	for(i=0; i < 256;i++){
		if(!bloco_leitura(i,&buffer[i*TAM_SETOR]))
		 return 0;
	}
		
	//_______ler o diretorio do disco_______
	buffer = (char *) dir;
	for(i=256;i < 264;i++){
		if(!bloco_leitura(i,&buffer[j*TAM_SETOR]))
		  return 0;
		j++;
	} 

	//____verificar se o disco está formatado_____
	for(i=0;i < 32;i++){
		if(fat[i] != 3){
		  formatado = 0;
		  break;
		}
	} 

	gerenciador_formatar(); // Formata sempre ao iniciar
		return 1;
}

//________________Gerenciador Formatar______________
int gerenciador_formatar() {
  
	int i=0,j =0;
	char *buffer;

	for(i = 0; i < 32;i++){
		fat[i] = 3;
	}

	fat[32] = 4;

	for(i = 0; i < 128;i++){
		dir[i].usado = 0;
	}

	for(i = 33;i < TAM_FAT; i++){
		fat[i] = 1;
	}

	buffer = (char *) fat;
	for(i=0; i < 256;i++){
		if(!bloco_escrita(i,&buffer[i*TAM_SETOR]))
		 return 0;
	}

	buffer = (char *) dir;
	for(i=256;i < 264;i++){
		if(!bloco_escrita(i,&buffer[j*TAM_SETOR]))
		  return 0;
		j++;
		}

	formatado = 1;

	return 1;
}


//____________Gerenciador Free_________________

int gerenciador_free() {

  int i, cont = 0;

  for(i = 0 ; i < bloco_tam()/8; i++ ){
    if(fat[i] == 1){
      cont++;
    }
  }
  return cont*TAM_SETOR;
}

//_____________Gerenciador Listar_______________

int gerenciador_listar() {
 
  int flag = 0;
 
	printf("Nome\tTam(bytes)\tBlocos\n___________________________________\n");
  for(int i=0; i < 128; i++){
		
    if(dir[i].usado == 1){
			printf("%s\t",dir[i].nome);
			printf("%d\t", dir[i].tamanho);
			printf("%d\t\n\n", dir[i].bloco);
      flag = 1;
    } 
  }

  return flag;
}


//_____________Gerenciador Criar_______________

int gerenciador_criar(char* nome_arq) {
  
  int i = 0, j = 0,livre = -1, fBloco;
  char *buffer;

  if (!formatado) {
    printf("Disco não formatado\n");
    return 0;
  } 

  /*Verificar se o arquivo existe*/
  for(i=0; i < 128;i++){
    if(strcmp(dir[i].nome,nome_arq) == 0 && dir[i].usado == 1){
      printf("Arquivo com o mesmo nome já existe\n");
      return 0;
    }
    if(dir[i].usado == 0 && livre == -1){
      livre = i;
    }
  }

  if(livre == -1){
    printf("Não há espaço no diretorio\n");
    return 0;
  }

  if(strlen(nome_arq) > 25){
    printf("Nome de arquivo muito grande\n");
    return 0;
  }

  strcpy(dir[livre].nome, nome_arq);
  dir[livre].usado = 1; 
  dir[livre].tamanho = 0;
  dir[livre].bloco = 0;

  for (j = 256; j < TAM_FAT && !dir[i].bloco; j++) {
    if (fat[j] == 1) {
      dir[i].bloco = j;
      fat[j] = 2;
    }
  }  

  for (fBloco = 0, i = 256; i < TAM_FAT; i++) {
    if (fat[i] == 1 && !fBloco) {
      dir[livre].bloco = i;
      fat[i] = 2;
      fBloco = 1;
    }
  }

  buffer = (char*) fat;
  for(i=0; i < 256; i++) {
    if(!bloco_escrita(i,&buffer[i*TAM_SETOR])) {
      printf("Erro ao escrever a FAT no disco\n");
      return 0;
    }
  }

  buffer = (char*) dir;
  for(i = 256, j = 0;i < 264;i++){
    if(!bloco_escrita(i,&buffer[j*TAM_SETOR])) {
      printf("Erro ao escrever o diretório no disco\n");
      return 0;
    }
    j++;
  }
  
  return 1;
}

//_____________Gerenciador Buscar_______________

int gerenciador_buscar(char *nome_arq){
	int i;
	int cont=0;
	for(i=0; i < 128;i++){
    if(strcmp(dir[i].nome,nome_arq) == 0 && dir[i].usado == 1 ){ //verifica se o nome do arquivo existe e se o mesmo está sendo usado
      printf("Arquivo Encontrado!\n");
			printf("Nome:%s\n",dir[i].nome);
			printf("Tamanho:%d\n",dir[i].tamanho);
			printf("Bloco:%d\n\n",dir[i].bloco);
			cont=1;
      return 0;

		}
	}
	if(cont==0){
			printf("Arquivo não encontrado\n");
			return 0;
	}
}


//_____________Gerenciador Procurar_______________
int gerenciador_procurar(char *palavra){
	
	char *aux,frase[7];
	int i, cont;
	printf("Arquivo\t\tOcorrencia\n_________________________");
	for(i=0; i < 128;i++){  //varre todos os arquivos 
		cont=0;
		if(dir[i].usado == 1){   //abre somente os arquivos usados 
			FILE *arq_aux;
	
			aux=dir[i].nome;
			arq_aux= fopen(aux, "r");
			if (!( arq_aux = fopen(aux,"r"))){ 
  				 printf("Erro, nao foi possivel abrir o arquivo\n");
			}else{

				while (!feof(arq_aux)) {  
					fscanf (arq_aux,"%s",frase);          
					if (!strcmp(frase, palavra)){
						  cont++; 
					}               
				}
				
				dir[i].ocorrencia=cont;
				printf("\n\n%s\t\t%d",dir[i].nome,dir[i].ocorrencia);
				fclose(arq_aux); 
			}
		}
	}
	return 0;
}

//_____________Gerenciador Escrever_______________

int gerenciador_escrever(char *nome_arq){
	int i=0, cont=0;
	for(i=0; i < 128;i++){
    if(strcmp(dir[i].nome,nome_arq) == 0 && dir[i].usado == 1){ //verifica se o nome do arquivo existe e se o mesmo está sendo usado
			FILE *arq_aux;
			arq_aux= fopen(nome_arq, "w");
			if(arq_aux == NULL){
  				 printf("Erro, nao foi possivel abrir o arquivo\n");
			}else{
				printf("Texto:\n");
				fgets(dir[i].texto,1000,stdin);
				fprintf(arq_aux,"%s",dir[i].texto);

				//______Tamanho em bytes do arquivo___________
				fseek(arq_aux, 0, SEEK_END);
				dir[i].tamanho = ftell(arq_aux);
				printf("O arquivo possui %d bytes", dir[i].tamanho);
				//___________________________________________

				fclose(arq_aux);
				cont=1;
				return 0;
			}
    }
	}
		if(cont==0){
				printf("Arquivo não existe!\n");
				return 0;
		}	

}

//_____________Gerenciador Remover_______________

int gerenciador_remover (char *nome_arq) {

  int i,j;
  int remover = -1;
  char *buffer;

  /*Verificar se o arquivo existe*/
  for(remover = -1, i=0; i < 128;i++){
    if(strcmp(dir[i].nome,nome_arq) == 0 && dir[i].usado == 1){
      remover = i;
    }
  }

  if(remover == -1){
    printf("Arquivo não existe\n");
    return 0;
  }

  strcpy(dir[remover].nome, "");
  dir[remover].tamanho = 0;
  dir[remover].usado = 0;
  j = dir[remover].bloco;

  while(fat[j] != 2){ // remove tipo lista encadeada
    remover = j;
    j = fat[j];
    fat[remover] = 1;
  }

  fat[j] = 1;

  buffer = (char *) fat;
  for(i = 0; i < 256; i++){
    if(!bloco_escrita(i, &buffer[i*TAM_SETOR]))
     return 0;
  }

  buffer = (char *) dir;
  for(i = 256, j = 0; i < 264; i++, j++){
    if(!bloco_escrita(i, &buffer[j*TAM_SETOR]))
      return 0;
  }


  return 0;
}





