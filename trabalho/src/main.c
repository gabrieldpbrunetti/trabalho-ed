#include "bd_pacientes.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>  // Para EXIT_FAILURE e EXIT_SUCCESS

/**
 * @brief Descarta caracteres restantes no buffer de stdin até o próximo '\n' ou EOF.
 * 
 * Quando se utiliza scanf() para ler um único caractere ou número, o caractere de nova linha
 * muitas vezes permanece no buffer de entrada. Esta função consome todos os caracteres até
 * encontrar '\n' ou EOF, garantindo que a próxima leitura de linha funcione corretamente.
 */
void limpar_buffer() {
    int c;
    // Lê e descarta caracteres até encontrar o fim de linha ou EOF
    while ((c = getchar()) != '\n' && c != EOF) { }
}

/**
 * @brief Solicita ao usuário um critério de pesquisa (nome ou CPF) e executa a busca.
 * 
 * Exibe um pequeno menu com as opções:
 *   1 - Pesquisar por nome
 *   2 - Pesquisar por CPF
 *   3 - Retornar ao menu principal
 * 
 * Após ler a opção escolhida, solicita ao usuário o texto de consulta (nome ou CPF) via fgets(),
 * remove o '\n' final e chama bd_pesquisar_paciente() para exibir os resultados.
 * 
 * @param pacientes Array de Paciente previamente carregado pela função bd_carregar_pacientes().
 */
void pesquisar(Paciente *pacientes) {
    char consulta[101];  // Buffer para armazenar a string de consulta (até 100 caracteres + '\0')
    char op;             // Opção escolhida pelo usuário: '1', '2' ou '3'

    puts("\nEscolha o modo de consulta");
    puts("1 - Por nome");
    puts("2 - Por cpf");
    puts("3 - Retornar ao menu principal");

    // Lê um único caractere para definir o modo de pesquisa
    if (scanf(" %c", &op) != 1) {
        // Caso scanf falhe ao ler a opção, exibe mensagem e retorna sem realizar busca
        puts("Falha ao ler opção de menu");
        return;
    }
    // Remove o resto da linha (incluindo '\n') do buffer
    limpar_buffer();

    // Define a mensagem de prompt com base na opção escolhida
    char *mensagem = (op == '1') ? "Digite o nome: " :
                    (op == '2') ? "Digite o cpf: " : NULL;

    // Se a opção não for '1' nem '2', retorna ao menu principal
    if (!mensagem) {
        puts("Opção não reconhecida");
        return;
    }

    // Solicita ao usuário que digite o texto de consulta
    printf("%s", mensagem);

    // Lê a linha completa (incluindo espaços) via fgets
    if (!fgets(consulta, sizeof(consulta), stdin)) {
        // Em caso de erro na leitura, exibe mensagem e retorna
        puts("Erro na leitura da consulta");
        return;
    }

    // Remove o caractere de nova linha '\n' que fgets() inclui
    consulta[strcspn(consulta, "\n")] = '\0';

    // Verifica se a consulta ficou vazia após remoção do '\n'
    const size_t len = strlen(consulta);
    if (len == 0) {
        puts("Nenhum caracter lido na consulta");
        return;
    }

    // Chama a função de pesquisa do módulo bd_pacientes, passando a chave '1' ou '2'
    bd_pesquisar_paciente(pacientes, consulta, op);
}

/**
 * @brief Exibe as opções disponíveis no menu principal do programa.
 * 
 * Opções:
 *   1 - Procurar paciente
 *   2 - Listar pacientes
 *   3 - Encerrar programa
 */
void imprime_menu() {
    puts("\nQual operação deseja executar?");
    puts("1 - Procurar paciente");
    puts("2 - Listar pacientes");
    puts("3 - Encerrar programa");
}

/**
 * @brief Função principal (main) do programa.
 * 
 * - Carrega o arquivo CSV de pacientes chamando bd_carregar_pacientes().
 * - Exibe repetidamente o menu principal, lê a opção do usuário e executa:
 *      '1' = Chama pesquisar()
 *      '2' = Chama bd_listar_pacientes()
 *      '3' = Encerra o laço e finaliza o programa
 *   Para qualquer outra opção, exibe mensagem de "Opção não reconhecida".
 * - Ao encerrar, libera a memória alocada para os pacientes e retorna EXIT_SUCCESS.
 * 
 * @return EXIT_SUCCESS (0) se tudo ocorrer bem, ou EXIT_FAILURE (1) caso não consiga carregar pacientes.
 */
int main() {
    // Tenta carregar os pacientes do arquivo "./dados.csv"
    Paciente *pacientes = bd_carregar_pacientes("./dados.csv");
    if (!pacientes) {
        // Se não conseguiu carregar (arquivo não encontrado ou vazio), encerra com código de erro
        return EXIT_FAILURE;
    }

    char opcao;  // Opção do usuário no menu principal

    // Loop principal: exibe o menu e executa até o usuário escolher '3'
    do {
        imprime_menu();

        // Lê a opção escolhida como caractere
        if (scanf(" %c", &opcao) != 1) {
            puts("Erro ao ler opção");
            // Se falhar a leitura, continua para próxima iteração do loop
            continue;
        }
        // Limpa o buffer para descartar '\n' remanescente
        limpar_buffer();

        // Verifica qual opção foi selecionada
        switch (opcao) {
            case '1':
                // Caso 1: pesquisa de paciente
                pesquisar(pacientes);
                break;
            case '2':
                // Caso 2: listar todos os pacientes
                bd_listar_pacientes(pacientes);
                break;
            case '3':
                // Caso 3: exibe mensagem de encerramento; o loop terminará em seguida
                puts("Encerrando programa");
                break;
            default:
                // Qualquer outra opção inválida
                puts("Opção não reconhecida");
                break;
        }
    } while (opcao != '3');

    // Libera a memória alocada para o array de pacientes
    free(pacientes);

    return EXIT_SUCCESS;
}
