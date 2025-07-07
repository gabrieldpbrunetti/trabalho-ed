#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bd_pacientes.h"

#define BUF_SIZE 150         // Tamanho máximo de uma linha lida do CSV
#define NOME '1'             // Chave para indicar pesquisa pelo nome
#define CPF '2'              // Chave para indicar pesquisa pelo CPF

/**
 * @brief Inicializa uma lista duplamente encadeada vazia.
 * 
 * Aloca e configura uma estrutura LinkedList, definindo
 * size = 0 e ponteiros head e tail para NULL.
 *
 * @return Ponteiro para a lista inicializada, ou NULL em caso de falha.
 */
LinkedList* linked_list_init() {
    LinkedList *l = malloc(sizeof *l);
    if (!l) return NULL;

    l->size = 0;     // Nenhum nó ainda
    l->head = NULL;  // Início aponta para vazio
    l->tail = NULL;  // Fim aponta para vazio
    return l;
}

/**
 * @brief Libera todos os nós e dados de uma lista duplamente encadeada.
 *
 * Percorre os nós a partir de head, liberando primeiro o campo data
 * (Paciente*) e depois o próprio nó. Ao final, libera a estrutura LinkedList.
 *
 * @param l Ponteiro para a lista a ser liberada.
 */
void linked_list_free(LinkedList *l) {
    Node *n = l->head;
    while (n) {
        Node *tmp = n->next;
        free(n->data);  // libera memória do paciente
        free(n);        // libera o nó
        n = tmp;
    }
    free(l);            // libera estrutura da lista
}

/**
 * @brief Adiciona um novo nó ao final da lista.
 *
 * Cria um nó com o dado indicado e o insere após o tail.
 * Ajusta prev/next e atualiza head/tail conforme necessário.
 *
 * @param l    Lista onde o nó será inserido.
 * @param data Ponteiro para Paciente que será armazenado.
 * @return true em sucesso, false em caso de falha de alocação.
 */
bool linked_list_append(LinkedList *l, Paciente *data) {
    Node *n = malloc(sizeof *n);
    if (!n) return false;

    n->data = data;  // associa dado
    n->next = NULL;  // novo nó será o último

    if (l->size == 0) {
        // lista vazia: head e tail apontam para o novo nó
        n->prev = NULL;
        l->head = n;
        l->tail = n;
        l->size++;
        return true;
    }

    // lista não vazia: insere após tail
    n->prev = l->tail;
    l->tail->next = n;
    l->tail = n;
    l->size++;
    return true;
}

/**
 * @brief Procura um nó pelo campo id dentro da lista.
 *
 * Percorre a lista a partir de head comparando p->id com id buscado.
 *
 * @param l  Lista onde será feita a busca.
 * @param id Identificador do paciente a ser encontrado.
 * @return Ponteiro para o nó encontrado, ou NULL se não existir.
 */
Node* linked_list_find(LinkedList *l, int id) {
    Node *itr = l->head;
    while (itr) {
        if (itr->data->id == id)
            return itr;  // encontrou o nó
        itr = itr->next;
    }
    return NULL;  // não encontrado
}

/**
 * @brief Carrega pacientes de um arquivo CSV para uma lista.
 *
 * Lê todos os registros, ignorando cabeçalho,
 * aloca cada Paciente e anexa à lista.
 *
 * @param csv_path Caminho para o arquivo CSV.
 * @return Ponteiro para LinkedList de pacientes ou NULL em erro.
 * @note Chamador deve liberar memória com bd_free().
 */
LinkedList* bd_carregar_pacientes(char *csv_path) {
    FILE *csv = fopen(csv_path, "r");
    if (!csv) return NULL;  // falha ao abrir

    char linha[BUF_SIZE];
    // descarta cabeçalho; se falhar, arquivo está vazio
    if (!fgets(linha, BUF_SIZE, csv)) {
        fclose(csv);
        printf("Erro ao ler cabeçalho\nArquivo vazio");
        return NULL;
    }

    // cria lista vazia de pacientes
    LinkedList *pacientes = linked_list_init();
    if (!pacientes) {
        puts("Falha ao alocar memória");
        fclose(csv);
        return NULL;
    }

    int id_buf;
    char cpf_buf[15];
    char nome_buf[100];
    unsigned char idade_buf;
    char data_buf[11];
    char format[] = "%d,%14[^,],%99[^,],%hhu,%10s";

    // lê linhas enquanto conseguir
    while (fscanf(csv, format,
                  &id_buf,
                  cpf_buf,
                  nome_buf,
                  &idade_buf,
                  data_buf) == 5) {
        Paciente *p = malloc(sizeof *p);
        if (!p) {
            puts("Falha ao alocar memória para o paciente");
            linked_list_free(pacientes);
            fclose(csv);
            return NULL;
        }
        // preenche campos da struct Paciente
        p->id = id_buf;
        strncpy(p->cpf, cpf_buf, sizeof(p->cpf));
        p->cpf[sizeof(p->cpf)-1] = '\0';
        strncpy(p->nome, nome_buf, sizeof(p->nome));
        p->nome[sizeof(p->nome)-1] = '\0';
        p->idade = idade_buf;
        strncpy(p->data, data_buf, sizeof(p->data));
        p->data[sizeof(p->data)-1] = '\0';

        linked_list_append(pacientes, p);
    }

    fclose(csv);  // fecha arquivo após leitura
    return pacientes;
}

/**
 * @brief Imprime um paciente formatado: id, cpf, nome, idade, data.
 *
 * @param p Ponteiro para o Paciente a ser exibido.
 */
void bd_imprimir_paciente(Paciente *p) {
    printf("%d\t%s\t%s\t%d\t%s\n",
        p->id,
        p->cpf,
        p->nome,
        p->idade,
        p->data
    );
}

/**
 * @brief Lista todos os pacientes de uma LinkedList.
 *
 * Percorre cada nó e chama bd_imprimir_paciente. Exibe cabeçalho antes.
 *
 * @param pacientes Lista de pacientes a ser exibida.
 */
void bd_listar_pacientes(LinkedList *pacientes) {
    printf("ID\tCPF\tNOME\tIDADE\tDATA_CADASTRO\n");
    Node *itr = pacientes->head;
    while (itr) {
        bd_imprimir_paciente(itr->data);
        itr = itr->next;
    }
}

/**
 * @brief Libera toda a memória alocada para pacientes.
 *
 * Chama linked_list_free para desalocar nós e dados.
 *
 * @param pacientes Lista a ser liberada.
 */
void bd_free(LinkedList *pacientes) {
    linked_list_free(pacientes);
}

/**
 * @brief Pesquisa e imprime pacientes cujo CPF ou nome inicia com consulta.
 *
 * Compara prefixo de cada dado com a string consulta.
 * Exibe mensagem se nenhum resultado.
 *
 * @param pacientes Lista onde realizar a busca.
 * @param consulta  Prefixo a ser pesquisado.
 * @param chave     NOME ('1') ou CPF ('2') para campo de comparação.
 */
void bd_pesquisar_paciente(const LinkedList *pacientes, char *consulta, int chave) {
    int qtd_impressos = 0;
    printf("ID\tCPF\tNOME\tIDADE\tDATA_CADASTRO\n");

    Node *itr = pacientes->head;
    while (itr) {
        Paciente *p = itr->data;
        // seleciona campo para comparação
        char *dado = (chave == CPF) ? p->cpf : p->nome;

        if (strncmp(consulta, dado, strlen(consulta)) == 0) {
            bd_imprimir_paciente(p);
            qtd_impressos++;
        }
        itr = itr->next;
    }

    if (qtd_impressos == 0) {
        puts("Nenhum paciente encontrado");
    }
}

/**
 * @brief Atualiza campos de um paciente se parâmetros não forem "-".
 *
 * Para cada campo recebido como string, substitui o valor atual
 * se o parâmetro for diferente de "-".
 *
 * @param p     Paciente a ser atualizado.
 * @param nome  Novo nome ou "-" para manter.
 * @param cpf   Novo CPF ou "-" para manter.
 * @param idade Nova idade (string) ou "-" para manter.
 * @param data  Nova data ou "-" para manter.
 */
void bd_atualizar_paciente(Paciente *p, char *nome, char *cpf, char *idade, char *data) {
    if (strcmp(nome, "-") != 0) strcpy(p->nome, nome);
    if (strcmp(cpf, "-") != 0) strcpy(p->cpf, cpf);
    if (strcmp(idade, "-") != 0) p->idade = atoi(idade);
    if (strcmp(data, "-") != 0) strcpy(p->data, data);
}

/**
 * @brief Remove um paciente da lista e libera sua memória.
 *
 * Ajusta ponteiros prev/next dos nós vizinhos e atualiza head e tail.
 * Decrementa size e libera o dado e o nó.
 *
 * @param pacientes Lista de onde o nó será removido.
 * @param paciente  Ponteiro para o nó a ser removido.
 */
void bd_remover_paciente(LinkedList *pacientes, Node *paciente) {
    if (pacientes->size == 1) {
        // único nó na lista
        pacientes->head = NULL;
        pacientes->tail = NULL;
    }
    else if (pacientes->head == paciente) {
        // remove do início
        paciente->next->prev = NULL;
        pacientes->head = paciente->next;
    }
    else if (pacientes->tail == paciente) {
        // remove do fim
        paciente->prev->next = NULL;
        pacientes->tail = paciente->prev;
    }
    else {
        // remove do meio
        paciente->prev->next = paciente->next;
        paciente->next->prev = paciente->prev;
    }
    pacientes->size--;
    free(paciente->data);
    free(paciente);
}

/**
 * @brief Insere um novo paciente com próximo ID automático.
 *
 * Atribui ID = tail->id + 1 e faz append na lista.
 *
 * @param pacientes Lista onde inserir.
 * @param cpf        CPF do novo paciente.
 * @param nome       Nome do novo paciente.
 * @param idade      Idade do novo paciente.
 * @param data       Data de cadastro do novo paciente.
 * @return true em sucesso, false em falha de alocação ou append.
 */
bool bd_inserir_paciente(LinkedList *pacientes, char *cpf, char *nome, int idade, char *data) {
    Paciente *p = malloc(sizeof *p);
    if (!p) {
        puts("Falha ao alocar memória para paciente");
        return false;
    }

    strncpy(p->cpf, cpf, sizeof(p->cpf));
    strncpy(p->nome, nome, sizeof(p->nome));
    strncpy(p->data, data, sizeof(p->data));
    p->idade = idade;
    p->id = pacientes->tail->data->id + 1;  // novo ID sequencial

    if(!linked_list_append(pacientes, p)) {
        return false;
    }

    return true;
}

/**
 * @brief Reescreve o arquivo CSV com os dados atuais da lista.
 *
 * Abre arquivo em modo escrita (sobrescreve), escreve cabeçalho e
 * cada registro no formato CSV. Fecha arquivo ao final.
 *
 * @param filename Caminho do CSV a ser atualizado.
 * @param l        Lista de pacientes com dados atualizados.
 * @return true em sucesso, false em caso de falha ao abrir.
 */
bool bd_atualiza_csv(char *filename, LinkedList *l) {
    FILE *csv = fopen(filename, "w");
    if (!csv) {
        puts("Erro ao abrir csv para escrita");
        return false;
    }

    // escreve cabeçalho
    fprintf(csv, "ID,CPF,Nome,Idade,Data_Cadastro\n");

    Node *itr = l->head;
    while(itr) {
        Paciente *p = itr->data;
        fprintf(
            csv,
            "%d,%s,%s,%hhu,%s\n",
            p->id, p->cpf, p->nome, p->idade, p->data
        );
        itr = itr->next;
    }
    puts("Arquivo atualizado");
    fclose(csv);
    return true;
}
