#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bd_pacientes.h"

#define BUF_SIZE 150         // Tamanho máximo de uma linha lida do CSV
#define QTD_PACIENTES 7     // Quantidade fixa de pacientes a serem carregados
#define NOME '1'             // Constante para indicar pesquisa pelo nome
#define CPF '2'              // Constante para indicar pesquisa pelo CPF

/**
 * @brief Carrega até QTD_PACIENTES registros de pacientes de um arquivo CSV.
 * 
 * Lê um arquivo CSV cujo caminho é passado em 'csv_path', ignora o cabeçalho
 * e tenta ler até QTD_PACIENTES linhas com o formato:
 *    id,cpf,nome,idade,data_cadastro
 * Cada linha válida é armazenada em um vetor de Paciente alocado dinamicamente.
 * 
 * @param csv_path Caminho para o arquivo CSV contendo os pacientes.
 * @return Ponteiro para um array de Paciente preenchido (tamanho QTD_PACIENTES),
 *         ou NULL em caso de erro (arquivo não encontrado ou arquivo vazio).
 * 
 * @note É responsabilidade do chamador liberar a memória retornada com bd_free().
 * @note O arquivo CSV é aberto em modo leitura ("r").
 * @note Caso o arquivo exista, mas não contenha conteúdo após o cabeçalho, retorna NULL.
 * @warning O fclose(csv) está posicionado após o return, ficando inacessível. 
 *          Em uma versão corrigida, seria necessário fechar o arquivo antes de retornar.
 */
Paciente* bd_carregar_pacientes(char *csv_path) {
    // Abre o arquivo CSV para leitura
    FILE *csv = fopen(csv_path, "r");
    if (!csv) {
        // Se não conseguir abrir, retorna NULL para indicar falha
        return NULL;
    }

    // Buffer para armazenar temporariamente cada linha lida
    char linha[BUF_SIZE];

    // Lê a primeira linha (cabeçalho) e descarta
    if (!fgets(linha, BUF_SIZE, csv)) {
        // Se não conseguiu ler nada (arquivo vazio), imprime mensagem de erro e retorna NULL
        printf("Erro ao ler cabeçalho\nArquivo vazio");
        return NULL;
    }

    // Aloca um array de QTD_PACIENTES estruturas Paciente
    Paciente *pacientes = (Paciente*) malloc(sizeof(Paciente) * QTD_PACIENTES);
    if (!pacientes) {
        // Se a alocação falhar, fecha o arquivo e retorna NULL
        fclose(csv);
        return NULL;
    }

    // Variáveis temporárias para receber cada campo do CSV
    int id_buf;
    char cpf_buf[15];
    char nome_buf[100];
    unsigned char idade_buf;
    char data_buf[11];

    // Índice de quantos pacientes foram efetivamente lidos
    int i = 0;

    // String de formato para fscanf: 
    //   %d          -> lê id (número inteiro)
    //   ,           -> consome a vírgula
    //   %14[^,]     -> lê até 14 caracteres que não sejam vírgula (cpf)
    //   ,           -> consome a vírgula
    //   %100[^,]    -> lê até 100 caracteres que não sejam vírgula (nome)
    //   ,           -> consome a vírgula
    //   %d          -> lê idade (número inteiro)
    //   ,           -> consome a vírgula
    //   %10s\n      -> lê até 10 caracteres (data), consumindo também o '\n'
    char format[] = "%d,%14[^,],%100[^,],%d,%10s\n";

    // Loop de leitura: enquanto não atingir o máximo de pacientes e fscanf retorne 5 (campos lidos)
    while (i < QTD_PACIENTES && fscanf(csv, format, &id_buf, cpf_buf, nome_buf, &idade_buf, data_buf) == 5) {
        // Cria uma estrutura Paciente temporária
        Paciente p;

        // Preenche os campos da estrutura com os valores lidos
        p.id = id_buf;

        // Usa strncpy para copiar strings, garantindo não ultrapassar o tamanho do campo
        strncpy(p.cpf, cpf_buf, sizeof(p.cpf));
        p.cpf[sizeof(p.cpf)-1] = '\0';  // Garante terminação nula

        strncpy(p.nome, nome_buf, sizeof(p.nome));
        p.nome[sizeof(p.nome)-1] = '\0';

        p.idade = idade_buf;

        strncpy(p.data, data_buf, sizeof(p.data));
        p.data[sizeof(p.data)-1] = '\0';

        // Armazena o paciente no array
        pacientes[i] = p;
        i++;
    }

    // Fecha os o arquivo após os dados já terem sido carregados para a memória
    fclose(csv);

    // Retorna o ponteiro para o array de pacientes
    return pacientes;
}

/**
 * @brief Imprime na saída padrão (console) os dados de um único paciente.
 * 
 * Formata e exibe os campos:
 *    id    CPF    Nome    Idade    Data de Cadastro
 * 
 * @param p Estrutura Paciente cujos dados serão impressos.
 */
void bd_imprimir_paciente(Paciente p) {
    printf("%d\t%s\t%s\t%d\t%s\n",
        p.id,
        p.cpf,
        p.nome,
        p.idade,
        p.data
    );
}

/**
 * @brief Lista todos os pacientes armazenados, imprimindo um cabeçalho e cada registro.
 * 
 * Recebe um array de Paciente com exatamente QTD_PACIENTES posições preenchidas.
 * Imprime um cabeçalho com os nomes das colunas e, em seguida, chama bd_imprimir_paciente()
 * para cada elemento do array.
 * 
 * @param pacientes Ponteiro para o array de Paciente (tamanho QTD_PACIENTES).
 */
void bd_listar_pacientes(Paciente *pacientes) {
    // Imprime cabeçalho das colunas
    printf("ID\tCPF\tNOME\tIDADE\tDATA_CADASTRO\n");
    // Loop através de todos os índices fixos (0 a QTD_PACIENTES-1)
    for (int i = 0; i < QTD_PACIENTES; i++) {
        bd_imprimir_paciente(pacientes[i]);
    }
}

/**
 * @brief Libera a memória alocada para o array de pacientes.
 * 
 * Deve ser chamado quando não for mais necessário acessar os dados de pacientes.
 * 
 * @param pacientes Ponteiro para o array de Paciente anteriormente retornado por bd_carregar_pacientes().
 */
void bd_free(Paciente *pacientes) {
    free(pacientes);
}

/**
 * @brief Pesquisa pacientes por CPF ou por nome, exibindo os que coincidem com o critério.
 * 
 * Percorre o array fixo de QTD_PACIENTES e compara, para cada paciente, os primeiros
 * caracteres (strlen(consulta)) do campo selecionado (CPF ou Nome) com a string de consulta.
 * Se houver coincidência exata no prefixo, imprime o paciente. Caso nenhum seja encontrado,
 * exibe mensagem informando isso.
 * 
 * @param pacientes Ponteiro para o array de Paciente (tamanho QTD_PACIENTES).
 * @param consulta  String a ser comparada (prefixo) contra CPF ou Nome.
 * @param chave     Indica qual campo usar na pesquisa: CPF ('2') ou NOME ('1').
 */
void bd_pesquisar_paciente(const Paciente *pacientes, char *consulta, int chave) {
    int qtd_impressos = 0;

    // Imprime cabeçalho das colunas
    printf("ID\tCPF\tNOME\tIDADE\tDATA_CADASTRO\n");

    // Percorre todos os pacientes
    for (int i = 0; i < QTD_PACIENTES; i++) {
        Paciente p = pacientes[i];
        // Escolhe o dado a ser comparado: se chave == CPF, usa p.cpf; caso contrário, usa p.nome
        char *dado = (chave == CPF) ? p.cpf : p.nome;

        // Compara apenas o prefixo (strlen(consulta)) de 'dado' com 'consulta'
        if (strncmp(consulta, dado, strlen(consulta)) == 0) {
            // Se coincidir, imprime o paciente
            bd_imprimir_paciente(p);
            qtd_impressos++;
        }
    }

    // Se não imprimiu nenhum paciente, avisa que não encontrou resultados
    if (qtd_impressos == 0) {
        puts("Nenhum paciente encontrado");
    }
}
