#include "bd_pacientes.h"
#include <stdio.h>

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

void pesquisar(Paciente *pacientes) {
    char consulta[101];
    char op;

    puts("\nEscolha o modo de consulta");
    puts("1 - Por nome");
    puts("2 - Por cpf");
    puts("3 - Retornar ao menu principal");

    if(scanf(" %c", &op) != 1) {
        puts("Falha ao ler opção de menu");
        return;
    }
    limpar_buffer();

    char *mensagem = (op == '1') ? "Digite o nome: " :
                    (op == '2') ? "Digite o cpf: " : NULL;

    if (!mensagem) {
        puts("Opção não reconhecida");
        return;
    }

    printf("%s", mensagem);

    if (!fgets(consulta, sizeof(consulta), stdin)) {
        puts("Erro na leitura da consulta");
        return;
    }

    consulta[strcspn(consulta, "\n")] = '\0';
    const size_t len = strlen(consulta);

    if (len == 0) {
        printf("Nenhum caracter lido na consulta\n");
        return;
    }

    bd_pesquisar_paciente(pacientes, consulta, op);
}

#pragma endregion

void imprime_menu() {
    puts("\nQual operação deseja executar?");
    puts("1 - Procurar paciente");
    puts("2 - Listar pacientes");
    puts("3 - Encerrar programa");
}


int main() {
    Paciente *pacientes = bd_carregar_pacientes("./dados.csv");
    if (!pacientes) return EXIT_FAILURE;
    char opcao;

    do {
        imprime_menu(); 

        if (scanf(" %c", &opcao) != 1) {
            printf("Erro ao ler opção");
            continue;
        }
        limpar_buffer();

        switch (opcao)
        {
        case '1':
            pesquisar(pacientes);
            break;
        case '2':
            bd_listar_pacientes(pacientes);
            break;
        case '3':
            puts("Encerrando programa");
            break;
        default:
            puts("Opção não reconhecida");
            break;
        }
    } while (opcao != '3');

    free(pacientes);
    return EXIT_SUCCESS;
}