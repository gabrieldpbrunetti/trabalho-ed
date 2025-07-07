#include "bd_pacientes.h"    // Declarações de LinkedList e operações em Paciente
#include <stdio.h>            // Funções de entrada/saída padrão (printf, scanf, getchar)
#include <string.h>           // Funções de manipulação de strings (strlen, strncmp, strcpy, strcspn)
#include <stdlib.h>           // EXIT_FAILURE, EXIT_SUCCESS, malloc, free, atoi

/**
 * @brief Limpa o buffer de entrada até o próximo '\n' ou EOF.
 *
 * Quando se usa scanf(), o '\n' da entrada permanece no stdin,
 * o que pode atrapalhar leituras subsequentes via fgets() ou scanf().
 * Esta função descarta todo o resto da linha atual.
 */
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // descarta caracteres até encontrar fim de linha ou EOF
    }
}

/**
 * @brief Exibe menu de pesquisa e executa busca por nome ou CPF.
 *
 * Exibe três opções: pesquisar por nome, pesquisar por CPF ou voltar.
 * Lê a opção escolhida, descarta o '\n', solicita a string de consulta,
 * retira o '\n' final e chama bd_pesquisar_paciente().
 *
 * @param pacientes Ponteiro para LinkedList já carregada de Paciente.
 */
void pesquisar(LinkedList *pacientes) {
    char consulta[101];  // até 100 caracteres + '\0'
    char op;

    // Menu de opções de pesquisa
    puts("\nEscolha o modo de consulta");
    puts("1 - Por nome");
    puts("2 - Por cpf");
    puts("3 - Retornar ao menu principal");

    // Lê opção como caractere (ignora espaços em branco antes)
    if (scanf(" %c", &op) != 1) {
        puts("Falha ao ler opção de menu");
        return;
    }
    limpar_buffer();  // remove resto da linha

    // Determina a mensagem de prompt com base na opção
    char *mensagem = (op == '1') ? "Digite o nome: " :
                     (op == '2') ? "Digite o cpf: " : NULL;

    if (!mensagem) {
        // opção inválida: retorna sem pesquisar
        puts("Opção não reconhecida");
        return;
    }

    // Solicita a string de consulta
    printf("\n%s", mensagem);
    if (!fgets(consulta, sizeof(consulta), stdin)) {
        puts("Erro na leitura da consulta");
        return;
    }
    // Remove o '\n' que fgets inclui
    consulta[strcspn(consulta, "\n")] = '\0';

    // Verifica se ao menos um caractere foi digitado
    if (strlen(consulta) == 0) {
        puts("Nenhum caracter lido na consulta");
        return;
    }

    // Executa a pesquisa usando a chave '1' (nome) ou '2' (cpf)
    bd_pesquisar_paciente(pacientes, consulta, op);
}

/**
 * @brief Atualiza campos de um paciente existente.
 *
 * Primeiro chama pesquisar() para o usuário escolher um filtro,
 * depois lê o ID do registro a alterar, encontra o nó, exibe o nome,
 * e lê novos valores para cada campo (ou '-' para manter).
 *
 * @param pacientes Lista de pacientes onde buscar e alterar.
 */
void atualizar_paciente(LinkedList *pacientes) {
    pesquisar(pacientes);

    int id_buf;
    puts("\nDigite o id do registro a ser alterado (-1 para sair)");
    if (scanf("%d", &id_buf) != 1) {
        puts("Falha ao ler do terminal");
        limpar_buffer();
        return;
    }
    limpar_buffer();

    if (id_buf == -1) return;  // sai sem alterações

    // Procura nó pelo ID
    Node *node = linked_list_find(pacientes, id_buf);
    if (!node) {
        puts("Nenhum registro encontrado com esse id");
        return;
    }

    // Exibe nome do paciente que será atualizado
    printf("\nAtualizando %s\n", node->data->nome);

    // Buffers para novos valores (ou '-')
    char nome_buf[100], cpf_buf[15], idade_buf[4], data_buf[11];
    puts("Digite os novos valores ou '-' para manter o atual.");

    printf("Nome: ");
    scanf(" %[^\n]", nome_buf);
    limpar_buffer();

    printf("Cpf: ");
    scanf(" %s", cpf_buf);
    limpar_buffer();

    printf("Idade: ");
    scanf(" %s", idade_buf);
    limpar_buffer();

    printf("Data de cadastro: ");
    scanf(" %s", data_buf);
    limpar_buffer();

    // Atualiza o paciente chamando a função do módulo bd_pacientes
    bd_atualizar_paciente(node->data, nome_buf, cpf_buf, idade_buf, data_buf);
}

/**
 * @brief Remove um paciente existente da lista.
 *
 * Solicita filtro via pesquisar(), lê o ID a remover,
 * busca o nó correspondente e chama bd_remover_paciente().
 *
 * @param pacientes Lista contendo o paciente a remover.
 */
void remover_paciente(LinkedList *pacientes) {
    pesquisar(pacientes);

    int id_buf;
    puts("\nDigite o id do registro a ser apagado (-1 para sair)");
    if (scanf("%d", &id_buf) != 1) {
        puts("Falha ao ler do terminal");
        limpar_buffer();
        return;
    }
    limpar_buffer();

    if (id_buf == -1) return;

    Node *node = linked_list_find(pacientes, id_buf);
    if (!node) {
        puts("Nenhum registro encontrado com esse id");
        return;
    }

    // Informa remoção e executa
    printf("\nRemovendo %s\n", node->data->nome);
    bd_remover_paciente(pacientes, node);
}

/**
 * @brief Insere um novo paciente na lista.
 *
 * Solicita ao usuário nome, CPF, idade e data de cadastro,
 * lê cada valor, limpa o buffer e chama bd_inserir_paciente().
 *
 * @param pacientes Lista para inserir o novo paciente.
 */
void inserir_paciente(LinkedList *pacientes) {
    char nome_buf[100], cpf_buf[15], idade_buf[4], data_buf[11];
    puts("Insira os dados do novo paciente:");

    printf("Nome: ");
    scanf(" %[^\n]", nome_buf);
    limpar_buffer();

    printf("Cpf: ");
    scanf(" %s", cpf_buf);
    limpar_buffer();

    printf("Idade: ");
    scanf(" %s", idade_buf);
    limpar_buffer();

    printf("Data de cadastro: ");
    scanf(" %s", data_buf);
    limpar_buffer();

    // Chama inserção e verifica sucesso
    if (!bd_inserir_paciente(pacientes, cpf_buf, nome_buf, atoi(idade_buf), data_buf)) {
        puts("Falha ao inserir paciente");
    }
}

/**
 * @brief Exibe o menu principal de operações.
 *
 * Opções disponíveis:
 *  - Consultar paciente
 *  - Atualizar paciente
 *  - Remover paciente
 *  - Inserir paciente
 *  - Listar todos os pacientes
 *  - Sair
 */
void imprime_menu() {
    puts("\nQual operação deseja executar?");
    puts("1 - Consultar paciente");
    puts("2 - Atualizar paciente");
    puts("3 - Remover paciente");
    puts("4 - Inserir paciente");
    puts("5 - Imprimir lista de pacientes");
    puts("Q - Sair do programa");
}

/**
 * @brief Ponto de entrada do programa.
 *
 * Carrega pacientes de CSV, entra em loop de menu,
 * executa operações até o usuário escolher 'Q',
 * atualiza o CSV e libera toda a memória.
 *
 * @return EXIT_SUCCESS se tudo ocorrer bem, EXIT_FAILURE caso falhe carregar CSV.
 */
int main() {
    // Caminho do arquivo de dados
    char *filename = "/mnt/c/Users/gabri/trabalho 2 ed/trabalho-ed/trabalho/bd_pacientes.csv";

    // Carrega pacientes para memória
    LinkedList *pacientes = bd_carregar_pacientes(filename);
    if (!pacientes) {
        puts("Falha ao abrir arquivo");
        return EXIT_FAILURE;
    }

    char opcao;
    // Loop principal: exibe menu e processa opção até 'Q'
    do {
        imprime_menu();

        if (scanf(" %c", &opcao) != 1) {
            puts("Erro ao ler opção");
            limpar_buffer();
            continue;
        }
        limpar_buffer();

        switch (opcao) {
            case '1': pesquisar(pacientes); break;
            case '2': atualizar_paciente(pacientes); break;
            case '3': remover_paciente(pacientes); break;
            case '4': inserir_paciente(pacientes); break;
            case '5': bd_listar_pacientes(pacientes); break;
            case 'Q': puts("Encerrando programa"); break;
            default:  puts("Opção não reconhecida");    break;
        }
    } while (opcao != 'Q');

    // Salva alterações no CSV e libera memória
    bd_atualiza_csv(filename, pacientes);
    bd_free(pacientes);

    return EXIT_SUCCESS;
}
