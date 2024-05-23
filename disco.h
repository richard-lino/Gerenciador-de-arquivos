#define TAM_SETOR 512

int bloco_iniciar(char *arquivo, int tamanho);
int bloco_tam();
int bloco_escrita(int setor, char* buffer);
int bloco_leitura(int setor, char* buffer);

