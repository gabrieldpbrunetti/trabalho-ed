#ifndef PACIENTE_H
#define PACIENTE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE        150
#define QTD_PACIENTES   7
#define CPF             1
#define NOME            2

#pragma region TAD paciente
typedef struct {
    int id;
    char cpf[15];
    char nome[100];
    int idade;
    char data[11];
} Paciente;
#pragma endregion


/* Carrega até QTD_PACIENTES pacientes de um arquivo CSV */
Paciente* bd_carregar_pacientes(char *csv_path);

/* Imprime um único paciente */
void bd_imprimir_paciente(Paciente p);

/* Imprime todos os pacientes carregados */
void bd_listar_pacientes(Paciente *pacientes);

/* Pesquisa pacientes por CPF ou nome */
void bd_pesquisar_paciente(const Paciente *pacientes, char *consulta, int chave);

void bd_free(Paciente *paceintes);

#endif /* PACIENTE_H */
