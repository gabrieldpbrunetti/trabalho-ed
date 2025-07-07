#ifndef PACIENTE_H
#define PACIENTE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BUF_SIZE        150
#define QTD_PACIENTES   7
#define CPF             1
#define NOME            2

#pragma region TAD paciente
typedef struct {
    int id;
    char cpf[15];
    char nome[100];
    unsigned char idade;
    char data[11];
} Paciente;
#pragma endregion

#pragma region linked list

typedef struct node {
    Paciente *data;
    struct node *next, *prev;
} Node;

typedef struct {
    Node *head, *tail;
    unsigned long long int size;
} LinkedList;
#pragma endregion

LinkedList* linked_list_init();

void linked_list_free(LinkedList *l);

bool linked_list_append(LinkedList *l, Paciente *data);

Node* linked_list_find(LinkedList *l, int id);

/* Carrega até QTD_PACIENTES pacientes de um arquivo CSV */
LinkedList* bd_carregar_pacientes(char *csv_path);

/* Imprime um único paciente */
void bd_imprimir_paciente(Paciente *p);

/* Imprime todos os pacientes carregados */
void bd_listar_pacientes(LinkedList *pacientes);

/* Pesquisa pacientes por CPF ou nome */
void bd_pesquisar_paciente(const LinkedList *pacientes, char *consulta, int chave);

void bd_free(LinkedList *pacientes);

void bd_atualizar_paciente(Paciente *p, char *nome, char *cpf, char *idade, char *data);

void bd_remover_paciente(LinkedList *l, Node *p);

bool bd_inserir_paciente(LinkedList *l, char *cpf, char *nome, int idade, char* data);

bool bd_atualiza_csv(char *filename, LinkedList *l);

#endif /* PACIENTE_H */
