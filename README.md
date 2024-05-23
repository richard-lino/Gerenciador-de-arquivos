# Gerenciador de Arquivos .txt
Este projeto implementa um sistema gerenciador de arquivos em linguagem C que permita aos usuários criar, remover, buscar, listar e realizar buscas internas em arquivos no formato .txt. Além disso, o sistema deve apresentar informações sobre a utilização da memória disponível e oferecer recursos para escrever e procurar a ocorrência de palavras nos arquivos, bem como a capacidade de formatar o sistema de arquivos.

## Área Explicativa (Código)

### main.c 
O arquivo main.c atua como o ponto de entrada do nosso programa. Sua principal função é coordenar as operações do sistema gerenciador de arquivos. Aqui, você encontrará a função principal que inicia o programa e interage com o usuário, variáveis globais e
estruturas de dados usadas para gerenciar o fluxo do programa e inicialização de funcionalidades de alto nível, como interação com o usuário e controle das operações do sistema.

#### Menu:
- $ **criar <arquivo>**: cria um arquivo com tamanho zero.
- $ **buscar <arquivo>**: verifica se o arquivo existe e exibe a memória disponível.
- $ **remover <arquivo>**: remove um arquivo previamente criado.
- $ **escrever <arquivo>**: permite escrever conteúdo em um arquivo existente.
- $ **procurar <palavra>**: procura a ocorrência de uma palavra em todos os arquivos e exibe uma tabela de ocorrências.
- $ **listar**: gera uma tabela com o nome dos arquivos, memória e blocos ocupados.
- $ **formatar**: realiza a formatação do sistema de arquivos.
- $ **sair**: encerra o programa.
  
### gerenciador.c
O arquivo gerenciador.c desempenha um papel essencial no nosso sistema, sendo responsável por controlar as operações de criação, remoção, busca, escrita, procura e listagem de arquivos. Além disso, ele gerencia a alocação de memória e a integridade dos dados do sistema. Aqui, vamos detalhar os principais campos da estrutura de dados e as funções implementadas:

#### Campos da Estrutura de Dados:
- **char usado**: Este campo é utilizado para verificar se uma posição na memória está sendo
ocupada ou não.
- **char nome[25]**: Armazena o nome do arquivo, permitindo a identificação única de cada
arquivo no sistema.
- **char texto[1000]**: Responsável por armazenar o texto quando a opção de escrita em um
arquivo é selecionada.
- **int ocorrencia**: Utilizado para verificar a ocorrência de uma palavra ao realizar a busca em
arquivos.
- **unsigned short bloco**: Armazena informações sobre o bloco utilizado pelo arquivo.
- **int tamanho**: Registra o tamanho do arquivo em bytes.

#### Funções Implementadas

- **int gerenciador_iniciar()**: Inicializa a estrutura dir_entrada com 0; Realiza a leitura da FAT; Lê o diretório do disco; Verifica se o disco está formatado.
- **int gerenciador_formatar()**: Permite a formatação do sistema de arquivos sempre que a função é chamada.
- **int gerenciador_free()**: Realiza a leitura dos bytes livres após a formatação do sistema.
- **int gerenciador_listar()**: Imprime os arquivos criados, incluindo o tamanho de cada arquivo em bytes e o bloco utilizado.
- **int gerenciador_criar(char *nome_arq)**: Verifica se o disco está formatado; Realiza busca para verificar se o arquivo com o nome especificado já existe; Confirma a disponibilidade de memória para a criação do arquivo; Cria o arquivo com o nome passado como parâmetro.
- **int gerenciador_buscar(char *nome_arq)**: Verifica se o nome do arquivo existe e se o mesmo está sendo usado.
- **int gerenciador_remover(char *nome_arq)**: Verifica a existência do arquivo a ser removido; Remove o arquivo da lista.
- **int gerenciador_escrever(char *nome_arq)**: Verifica se o nome do arquivo existe e se o mesmo está sendo usado; Abre o arquivo para escrita; Recebe o texto a ser inserido pelo usuário e armazena-o no campo texto da estrutura dir[i]; Registra os bytes utilizados no campo tamanho.
- **int gerenciador_procurar(char *nome_arq)**: Recebe a palavra a ser buscada nos arquivos como parâmetro; Percorre todos os arquivos e abre aqueles que estão em uso (quando dir[i].usado == 1) até encontrar a ocorrência da palavra; Exibe uma tabela com o nome do arquivo e o número de ocorrências da palavra.

### disco.c
O arquivo disco.c é uma parte fundamental do nosso sistema gerenciador de arquivos em linguagem C. Ele desempenha um papel crucial na inicialização da imagem do sistema gerenciador de arquivos, bem como na verificação e correção de erros de leitura e escrita nos setores do disco. Abaixo, detalhamos as principais funções implementadas neste arquivo:

### Funções Implementadas
- **int bloco_iniciar(char *arquivo, int tamanho)**: Inicializa a imagem do sistema gerenciador de arquivos com um tamanho específico; cria uma nova imagem se ainda não existir; ajusta o tamanho da imagem conforme necessário.
- **int bloco_tam()**: Retorna o tamanho total dos setores na imagem do sistema gerenciador de arquivos.
- **int bloco_escrita(int setor, char*** **buffer)**: Realiza testes rigorosos de erro ao posicionar o setor de escrita; identifica e lida com erros de escrita do setor e erros de gravação no disco, garantindo a integridade dos dados.
- **int bloco_leitura(int setor, char*** **buffer)**: Realiza verificações detalhadas de erro ao posicionar o setor de leitura; lida com erros de leitura do setor, garantindo que os dados sejam lidos corretamente.

## header files (.h)
Durante o desenvolvimento do Gerenciador de Arquivos, foi adotada uma estratégia de organização que visa simplificar a compreensão do código. Nesse contexto, foram estabelecidos dois arquivos com extensão .h, nomeadamente: disco.h e gerenciador.h. Essa abordagem foi adotada com o objetivo de promover a clareza e a estruturação do código-fonte, tornando-o mais acessível e de fácil compreensão para os desenvolvedores e mantenedores do sistema.

## Compilação do Código Fonte
- Abra um terminal. Navegue até o diretório onde o código-fonte está localizado. Utilize o seguinte comando para compilar o código:
```bash
   gcc main.c gerenciador.c disco.c -o <nome_do_gerenciador>
```
###### <nome_do_gerenciador> : deve ser preenchido pelo usuário com o nome de sua preferência.

## Execução do Programa
- Após a conclusão da compilação, execute o programa usando o seguinte comando:
```bash
   ./<nome_do_gerenciador> <imagem> <tamanho>
```
###### <nome_do_gerenciador> : deve ser preenchido com o mesmo nome criado no passo anterior.

###### < imagem>: deve ser preenchido pelo usuário com o nome de sua preferência.

###### < tamanho>: deve ser preenchido pelo usuário com o tamanho da imagem em MB.
