#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estado.h"

// Ponteiro para array de ponteiros de estados
estd** estados = NULL;
int total_estados = 0;

// Função auxiliar para limpar buffer de entrada
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Carrega estados do arquivo binário
void carregar_estados() {
    FILE* f = fopen("estado.bin", "rb");
    if (!f) return;

    fseek(f, 0, SEEK_END);
    long tamanho = ftell(f);
    fseek(f, 0, SEEK_SET);

    total_estados = tamanho / sizeof(estd);
    estados = (estd**)malloc(total_estados * sizeof(estd*));

    for (int i = 0; i < total_estados; i++) {
        estados[i] = (estd*)malloc(sizeof(estd));
        fread(estados[i], sizeof(estd), 1, f);
    }
    fclose(f);
}

// Salva estados no arquivo binário
void salvar_estados() {
    FILE* f = fopen("estado.bin", "wb");
    if (!f) {
        perror("Erro ao salvar estados");
        return;
    }

    for (int i = 0; i < total_estados; i++) {
        fwrite(estados[i], sizeof(estd), 1, f);
    }
    fclose(f);
}

// Libera toda a memória alocada
void liberar_memoria() {
    for (int i = 0; i < total_estados; i++) {
        free(estados[i]);
    }
    free(estados);
    estados = NULL;
    total_estados = 0;
}

// Verifica se um código já existe
int codigo_existente(int codigo) {
    for (int i = 0; i < total_estados; i++) {
        if (estados[i]->codigo == codigo) {
            return 1;
        }
    }
    return 0;
}

// Adiciona novo estado usando ponteiro
void adicionar_estado(estd* novo) {
    total_estados++;
    estados = (estd**)realloc(estados, total_estados * sizeof(estd*));
    estados[total_estados - 1] = (estd*)malloc(sizeof(estd));

    // Copia dados usando ponteiros
    estados[total_estados - 1]->codigo = novo->codigo;
    strncpy(estados[total_estados - 1]->nome, novo->nome, sizeof(novo->nome) - 1);
    strncpy(estados[total_estados - 1]->sigla, novo->sigla, sizeof(novo->sigla) - 1);

    salvar_estados();
}

// Interface para adicionar estado usando ponteiro
void adicionar_estado_ui() {
    estd* novo = (estd*)malloc(sizeof(estd));
    if (!novo) {
        perror("Erro ao alocar memória");
        return;
    }

    int codigo_valido = 0;

    while (!codigo_valido) {
        printf("Código do estado: ");
        scanf("%d", &novo->codigo);
        limpar_buffer();
        if (codigo_existente(novo->codigo)) {
            printf("Código já existe!\n");
        } else {
            codigo_valido = 1;
        }
    }

    printf("Nome do estado: ");
    fgets(novo->nome, sizeof(novo->nome), stdin);
    novo->nome[strcspn(novo->nome, "\n")] = '\0';

    printf("Sigla do estado (2 caracteres): ");
    fgets(novo->sigla, sizeof(novo->sigla), stdin);
    novo->sigla[strcspn(novo->sigla, "\n")] = '\0';

    adicionar_estado(novo);
    free(novo); // Libera memória após cópia
    printf("Estado adicionado com sucesso!\n");
}

// Lista todos os estados usando ponteiros
void listar_estados() {
    if (total_estados == 0) {
        printf("Nenhum estado cadastrado\n");
        return;
    }

    for (int i = 0; i < total_estados; i++) {
        printf("Codigo: %d\n", estados[i]->codigo);
        printf("Nome: %s\n", estados[i]->nome);
        printf("Sigla: %s\n", estados[i]->sigla);
        printf("\n");
    }
}

// Busca estado por nome usando ponteiros
void buscar_por_nome(const char* nome) {
    for (int i = 0; i < total_estados; i++) {
        if (strcmp(estados[i]->nome, nome) == 0) {
            printf("Codigo: %d\n", estados[i]->codigo);
            printf("Nome: %s\n", estados[i]->nome);
            printf("Sigla: %s\n", estados[i]->sigla);
            printf("\n");
            return;
        }
    }
    printf("Estado não encontrado!\n");
}

// Busca estado por código usando ponteiros
void buscar_por_codigo(int codigo) {
    for (int i = 0; i < total_estados; i++) {
        if (estados[i]->codigo == codigo) {
            printf("Código: %d\n", estados[i]->codigo);
            printf("Nome: %s\n", estados[i]->nome);
            printf("Sigla: %s\n", estados[i]->sigla);
            printf("\n");
            return;
        }
    }
    printf("Estado não encontrado!\n");
}

// Busca estado por sigla usando ponteiros
void buscar_por_sigla(const char* sigla) {
    for (int i = 0; i < total_estados; i++) {
        if (strcmp(estados[i]->sigla, sigla) == 0) {
            printf("Codigo: %d\n", estados[i]->codigo);
            printf("Nome: %s\n", estados[i]->nome);
            printf("Sigla: %s\n", estados[i]->sigla);
            printf("\n");
            return;
        }
    }
    printf("Estado não encontrado!\n");
}

// Menu de buscas usando ponteiros
void menu_busca() {
    int opcao;
    printf("\nOpções de busca:\n");
    printf("1. Por nome\n");
    printf("2. Por código\n");
    printf("3. Por sigla\n");
    printf("Escolha: ");
    scanf("%d", &opcao);
    limpar_buffer();

    switch (opcao) {
        case 1: {
            char* nome = (char*)malloc(50 * sizeof(char));
            printf("Nome: ");
            fgets(nome, 50, stdin);
            nome[strcspn(nome, "\n")] = '\0';
            buscar_por_nome(nome);
            free(nome);
            break;
        }
        case 2: {
            int codigo;
            printf("Código: ");
            scanf("%d", &codigo);
            limpar_buffer();
            buscar_por_codigo(codigo);
            break;
        }
        case 3: {
            char* sigla = (char*)malloc(3 * sizeof(char));
            printf("Sigla (2 letras): ");
            fgets(sigla, 3, stdin);
            sigla[strcspn(sigla, "\n")] = '\0';
            buscar_por_sigla(sigla);
            free(sigla);
            break;
        }
        default:
            printf("Opção inválida!\n");
    }
}

// Edita um estado existente usando ponteiros
void editar_estado() {
    int codigo;
    printf("Código do estado a editar: ");
    scanf("%d", &codigo);
    limpar_buffer();

    for (int i = 0; i < total_estados; i++) {
        if (estados[i]->codigo == codigo) {
            printf("Novo nome [atual: %s]: ", estados[i]->nome);
            fgets(estados[i]->nome, sizeof(estados[i]->nome), stdin);
            estados[i]->nome[strcspn(estados[i]->nome, "\n")] = '\0';

            printf("Nova sigla [atual: %s]: ", estados[i]->sigla);
            fgets(estados[i]->sigla, sizeof(estados[i]->sigla), stdin);
            estados[i]->sigla[strcspn(estados[i]->sigla, "\n")] = '\0';

            salvar_estados();
            printf("Estado atualizado!\n");
            return;
        }
    }
    printf("Estado não encontrado!\n");
}

// Remove um estado usando ponteiros
void remover_estado() {
    int codigo;
    printf("Código do estado a remover: ");
    scanf("%d", &codigo);
    limpar_buffer();

    for (int i = 0; i < total_estados; i++) {
        if (estados[i]->codigo == codigo) {
            free(estados[i]);

            // Compacta o array
            for (int j = i; j < total_estados - 1; j++) {
                estados[j] = estados[j + 1];
            }

            total_estados--;
            estados = (estd**)realloc(estados, total_estados * sizeof(estd*));
            salvar_estados();
            printf("Estado removido!\n");
            return;
        }
    }
    printf("Estado não encontrado!\n");
}

// Menu principal
void menu_principal() {
    int opcao;
    do {
        printf("\n=== Menu Principal ===\n");
        printf("1. Inserir estado\n");
        printf("2. Procurar estado\n");
        printf("3. Listar estados\n");
        printf("4. Editar estado\n");
        printf("5. Excluir estado\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
            case 1: adicionar_estado_ui(); break;
            case 2: menu_busca(); break;
            case 3: listar_estados(); break;
            case 4: editar_estado(); break;
            case 5: remover_estado(); break;
            case 0: printf("Saindo...\n"); break;
            default: printf("Opção inválida!\n");
        }
    } while (opcao != 0);
}

int main() {
    carregar_estados();
    printf("\n=== Sistema de Estados ===\n");
    menu_principal();
    liberar_memoria();
    return 0;
}
