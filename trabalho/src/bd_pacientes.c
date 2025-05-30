#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bd_pacientes.h"

#define BUF_SIZE 150
#define QTD_PACIENTES 7
#define NOME '1'
#define CPF '2'

Paciente* bd_carregar_pacientes(char *csv_path) {
    FILE *csv = fopen(csv_path, "r");
    if (!csv) return NULL;

    char linha[BUF_SIZE];
    if (!fgets(linha, BUF_SIZE, csv)) {
        printf("Erro ao ler cabeçalho\nArquivo vazio");
        return NULL;
    }

    Paciente *pacientes = (Paciente*) malloc(sizeof(Paciente) * QTD_PACIENTES);

    int id_buf;
    char cpf_buf[15];
    char nome_buf[100];
    int idade_buf;
    char data_buf[11];
    int i = 0;
    char format[] = "%d,%14[^,],%100[^,],%d,%10s\n";
    while (i < QTD_PACIENTES && fscanf(csv, format, &id_buf, cpf_buf, nome_buf, &idade_buf, data_buf) == 5) {
        Paciente p;
        p.id = id_buf;
        strncpy(p.cpf, cpf_buf, sizeof p.cpf);
        strncpy(p.nome, nome_buf, sizeof p.nome);
        p.idade = idade_buf;
        strncpy(p.data, data_buf, sizeof p.data);
        pacientes[i] = p;
        i++;
    }

    return pacientes;
    fclose(csv);
}

void bd_imprimir_paciente(Paciente p) {
    printf("%d\t%s\t%s\t%d\t%s\n",
    p.id, p.cpf, p.nome, p.idade, p.data);
}

void bd_listar_pacientes(Paciente *pacientes) {
    printf("ID\tCPF\tNOME\tIDADE\tDATA_CADASTRO\n");
    for (int i = 0; i < QTD_PACIENTES; i++) {
        bd_imprimir_paciente(pacientes[i]);
    }
}

void bd_free(Paciente *pacientes) {
    free(pacientes);
}

void bd_pesquisar_paciente(const Paciente *pacientes, char *consulta, int chave){
    int qtd_impressos = 0;
    printf("ID\tCPF\tNOME\tIDADE\tDATA_CADASTRO\n");
    for (int i = 0; i < QTD_PACIENTES; i++) {
        Paciente p = pacientes[i];
        char *dado = (chave == CPF) ? p.cpf : p.nome;
        if (strncmp(consulta, dado, strlen(consulta)) == 0) {
            bd_imprimir_paciente(p);
            qtd_impressos++;
        }
    }
    if (qtd_impressos == 0) puts("Nenhum paciente encontrado");
}