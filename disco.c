
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include "disco.h"

#define PAGESIZE 4096

int device_size;
FILE *stream;


//________________Inicialização da imagem com o tamanho________________

int bloco_iniciar(char *arquivo, int tamanho) {
  struct stat sb;

  stream = NULL;
  if (stat(arquivo, &sb) == 0) {
    if (S_ISREG(sb.st_mode)) {
      device_size = sb.st_size;
      stream = fopen(arquivo, "r+");
    }
    if (stream == NULL) {
      perror("Abrindo imagem pré-existente");
      return 0;
    }
  } else {
    device_size = tamanho * TAM_SETOR;
    if (device_size < 1) {
      printf("Imagem não pode ter tamanho zero\n");
      return 0;
    }
    stream = fopen(arquivo, "w+");
    if (stream == NULL) {
      perror("Criando nova imagem");
      return 0;
    }
    if (truncate(arquivo, device_size) == -1) {
      perror("Ajustando tamanho da imagem");
      return 0;
    }
  }
  return 1; 
}
//_____________Retorna o tamanho total dos setores_________

int bloco_tam() {
  return device_size / TAM_SETOR;
}

//___________ Escrita Setor / teste de erro_________________

int bloco_escrita(int setor, char *buffer) {
  if (fseek(stream, setor * TAM_SETOR, SEEK_SET) == -1) {
    perror("Erro posicionando setor para escrita");
    return 0;
  }
  if (fwrite(buffer, sizeof(char), TAM_SETOR, stream) != TAM_SETOR) {
    perror("Erro escrevendo setor");
    return 0;
  }
  if (fflush(stream) != 0) {
    perror("Erro gravando setor no disco");
    return 0;
  }
  return 1;
}

//___________ Leitura Setor / teste de erro_________________

int bloco_leitura(int setor, char *buffer){
  if (fseek(stream, setor * TAM_SETOR, SEEK_SET) == -1) {
    perror("Erro posicionando setor para leitura");
    return 0;
  }
  if (fread(buffer, sizeof(char), TAM_SETOR, stream) != TAM_SETOR) {
    perror("Erro lendo setor");
    return 0;
  }
  return 1;
}
