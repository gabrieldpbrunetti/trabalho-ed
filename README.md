# Sistema de Gerenciamento de Pacientes

Este projeto implementa um sistema básico em C para gerenciamento de pacientes utilizando dados provenientes de um arquivo CSV. O sistema permite carregar, listar, pesquisar, inserir, removere e atualizar pacientes.

## Estrutura do Projeto

* `bd_pacientes.h` - Cabeçalho contendo definição da estrutura `Paciente` e `LinkedList` e protótipos das funções.
* `bd_pacientes.c` - Implementação das funções relacionadas à manipulação dos dados dos pacientes e da lista encadeada.
* `main.c` - Arquivo principal que interage com o usuário através de um menu.
* `bs_pacientes.csv` - Arquivo contendo os dados dos pacientes a serem carregados.

## Funcionalidades

* **Carregamento de Dados**: lê pacientes de um arquivo CSV e os carrega em memória.
* **Listagem de Pacientes**: exibe uma tabela com todos os pacientes carregados.
* **Pesquisa por Nome/CPF**: permite buscar pacientes através de parte ou totalidade do nome ou CPF.
* **Inserir novos Pacientes**: é possível cadastrar novos pacientes
* **Remover Pacientes**: agora há a função de remover registros de pacientes.
* **Atualizar Pacientes**: registros já existentes podem ser modificados

## Como Executar o Projeto

### Compilação

Para compilar o projeto, utilize o seguinte comando no terminal:

```bash
make
```

### Execução

Execute o programa compilado com:

```bash
./programa
```

Certifique-se de que o arquivo `bd_pacientes.csv` esteja no mesmo diretório onde o programa será executado.

## Formato do arquivo CSV

O arquivo CSV deve seguir o seguinte formato:

```csv
id,cpf,nome,idade,data_cadastro
1,123.456.789-01,Ana Silva,30,2023-05-01
2,987.654.321-00,João Souza,45,2023-05-10
```

Cada linha representa um paciente e seus respectivos dados.

## Dependências

* Compilador C padrão (ex.: gcc)

## Estrutura dos Dados

```c
typedef struct {
    int id;
    char cpf[15];
    char nome[100];
    int idade;
    char data[11];
} Paciente;
```
- id: escolhido como int, é representação padrão
- cpf: definido com o tamanho de 15 caracteres, 11 digitos, pontos e traço mais caracter de fim de string
- nome: valor alto reservado, para lidar com possíveis nomes grandes
- idade: unsigned char seria o suficiente pois conseguiria armazenar até 255 anos, int foi escolhido por conveniência e similiradide da natureza dos dados
- data: armazena datas no formato yyyy-mm-dd. O tamanho de vetor escolhido é o suficiente para armazenar todos os caracteres mais o \0

```c
typedef struct node {
    struct node *next, prev;
    Paciente *data;
} Node;
```
- next: ponteiro para o próximo nó da lista
- prev: ponteiro para o nó anterior
- data: ponteiro para uma estrutura Paciente

```c
typedef struct {
    Node *head, *tail
    unsigned long long size;
} LinkedList;
```
- head: ponteiro para o primeiro nó da lista
- tail: ponteiro para o último nó da lista
- size: tamanho da lista

## Configurações

- O caminho do arquivo csv passado como parâmetro para a função bd_carregar_pacientes pode ser alterado caso desejado.

## Modularização

A ideia é fazer a implementação mais simples que resolva o problema. O projeto contém dois módulos principais. bd_pacientes, responsável pela lógica de processamento dos dados do arquico csv, e main, sendo o módulo principal do projeto e tendo como função interagir por meio do terminal com o usuário. A motivação por trás de apartar esses dois módulos foi tornar invisível para main a lógica que lida com os dados csv. Dessa forma, main possui uma única responsabilidade, ser a interface com o usuário. Ademais, qualquer alteração na lógica de processamento do csv pode ser feita sem que necessariamente o código main seja refatorado.

## Autor

Gabriel de Paula Brunetti

