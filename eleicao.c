#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eleicao.h"
#include "estado.c"

/*typedef struct eleição{
     int codigo_uf;
     int ano;
     char descrição[50];
 } eleicao;*/

eleicao** vetor_eleicao = NULL;          //vetor_eleicao = vetor_pessoa
int total_de_elecao = 0;

FILE* parametro_eleicao(char str[]) {

    FILE* arq = fopen(str, "rb+");
    if (arq == NULL) {
        arq = fopen(str, "wb+");
        if (arq == NULL) {
            printf("Erro ao criar o arquivo %s\n", str);
            return NULL;
        }
    }
    return arq;
}

void carregar_eleiçoes() {
    FILE* f = parametro_eleicao("eleicao.bin");

    fseek(f, 0, SEEK_END);
    long tamanho = ftell(f);
    fseek(f, 0, SEEK_SET);

    total_de_elecao = tamanho / sizeof(eleicao);
    vetor_eleicao = (eleicao**)malloc(total_de_elecao * sizeof(eleicao*));

    for (int i = 0; i < total_de_elecao; i++) {
        vetor_eleicao[i] = (eleicao*)malloc(sizeof(eleicao));
        fread(vetor_eleicao[i], sizeof(eleicao), 1, f);
    }
    fclose(f);
}

void salvar_eleicoes_no_arquivo() {
    FILE* f = fopen("eleicao.bin", "wb"); // Modo escrita binária (sobrescreve)
    if (!f) {
        printf("Erro ao abrir arquivo para escrita!\n");
        return;
    }
    for (int i = 0; i < total_de_elecao; i++) {
        fwrite(vetor_eleicao[i], sizeof(eleicao), 1, f);
    }

    fclose(f);
}

void liberar_memoria_eleicao() {
    for (int i = 0; i < total_de_elecao; i++) {
        free(vetor_eleicao[i]);
    }
    free(vetor_eleicao);
    vetor_eleicao = NULL;
    total_de_elecao = 0;
}

int verificar_chave_primaria_ano(int ano) {
    for (int i = 0; i < total_de_elecao; i++) {
        if (vetor_eleicao[i] -> ano == ano) {
            return 0;
        }
    }
    return 1;
}


int verificar_se_estado_existe(int codigo_uf) {
    carregar_estados();

    for (int i = 0; i < total_estados; i++) {
        if (estados[i]->codigo == codigo_uf) {
            buscar_por_codigo_uf(codigo_uf);
            return 1;
        }
    }

    return 0;
}

void adicionar_eleicao(eleicao* novo) {
  total_de_elecao++;
   vetor_eleicao = (eleicao**)realloc(vetor_eleicao,total_de_elecao * sizeof(eleicao*));
   vetor_eleicao[total_de_elecao - 1] = (eleicao*)malloc(sizeof(eleicao));
        vetor_eleicao[total_de_elecao - 1] -> ano = novo -> ano;
        vetor_eleicao[total_de_elecao - 1] -> codigo_uf = novo -> codigo_uf;
        strncpy(vetor_eleicao[total_de_elecao - 1] -> descricao, novo -> descricao, sizeof(novo -> descricao) - 1);

    salvar_eleicoes_no_arquivo();
}

int verificar_eleicao_estado_ano(int codigo_uf, int ano) {
    // Verifica se o estado existe
    if (!verificar_se_estado_existe(codigo_uf)) {
        return 0; // Estado não existe
    }

    // Verifica se já existe eleição no mesmo estado e ano
    for (int i = 0; i < total_de_elecao; i++) {
        if (vetor_eleicao[i]->codigo_uf == codigo_uf && vetor_eleicao[i]->ano == ano) {
            return 1; // Eleição já existe
        }
    }
    return 0; // Eleição não existe
}

void adicionar_eleicao_ui() {
    eleicao* novo = (eleicao*)malloc(sizeof(eleicao));
    int codigo_valido = 0;

        printf("Ano da eleicao:\t ");
        scanf("%d", &novo->ano);
    while (!codigo_valido) {
        if (novo -> ano < 1000) {
            printf("ano invalido, precisa ter 4 digitos");
            continue;
        }

        printf("Código do estado:\t ");
        scanf("%d", &novo->codigo_uf);
        if (verificar_se_estado_existe(novo->codigo_uf)) {
            // Estado existe, aceita
        }
        else {
            printf("Estado inválido! Tente novamente.\n");
            continue;
        }

        if (verificar_eleicao_estado_ano(novo->codigo_uf, novo->ano)) {
            printf("Erro: Já existe uma eleição neste estado e ano!\n");
        } else {
            codigo_valido = 1;
        }
    }


    printf("Descrição da eleicao:\t");
    fflush(stdin);
    fgets(novo->descricao, 50, stdin);
    novo->descricao[strcspn(novo->descricao, "\n")] = '\0'; // Remove a quebra de linha

    adicionar_eleicao(novo);
    free(novo);

        printf("\n");
        printf("\n");
}


void editar_eleicao() {
    int ano;
    printf("Ano da eleicao que deseja editar: ");
    scanf("%d", &ano);

    for (int i = 0; i < total_de_elecao; i++) {
        if (vetor_eleicao[i]->ano == ano) {
            int novo_ano, novo_codigo_uf;
            printf("Novo ano [atual: %d]: ", vetor_eleicao[i]->ano);
            scanf("%d", &novo_ano);
            if (novo_ano < 1000) {
                printf("ano deve ter 4 digitos");
                continue;
            }
            printf("Novo código UF [atual: %d]: ", vetor_eleicao[i]->codigo_uf);
            scanf("%d", &novo_codigo_uf);
            if (verificar_se_estado_existe(novo_codigo_uf)) {
            }
            else {
                printf("Estado inválido! Tente novamente.\n");
                continue;
            }


            // Verifica se a nova combinação já existe
            if (verificar_eleicao_estado_ano(novo_codigo_uf, novo_ano)) {
                printf("Erro: Já existe uma eleição neste estado e ano!\n");
                return;
            }

            vetor_eleicao[i]->ano = novo_ano;
            vetor_eleicao[i]->codigo_uf = novo_codigo_uf;

            printf("Nova descrição [atual: %s]: ", vetor_eleicao[i]->descricao);
            getchar();
            fgets(vetor_eleicao[i]->descricao, 50, stdin);
            vetor_eleicao[i]->descricao[strcspn(vetor_eleicao[i]->descricao, "\n")] = '\0';

            salvar_eleicoes_no_arquivo();
            printf("Eleição atualizada!\n");
            return;
        }
    }
    printf("Eleição não encontrada!\n");
}

void listar_todas_eleiçoes() {
    for (int i = 0; i <total_de_elecao; i++) {
        printf("Ano: %d\n", vetor_eleicao[i]->ano);
        printf("estado que aconteceu: %d\n", vetor_eleicao[i] -> codigo_uf);
        printf("descrição: %s\n", vetor_eleicao[i] ->descricao);

        printf("\n");
    }
}

void buscar_eleicao_por_ano(const int ano_a_buscar) {
    for (int i = 0; i <total_de_elecao; i++) {
        if (vetor_eleicao[i] -> ano == ano_a_buscar) {
            printf("ano da eleicao: %d\n", vetor_eleicao[i] -> ano);
            printf("codigo do estado da eleicao: %s\n", vetor_eleicao[i] -> codigo_uf);
            printf("descrição: %s\n", vetor_eleicao[i] -> descricao);

            printf("\n");
            return;
        }
    }
    printf("eleicao não encotrda!\n");
}

void excluir_eleicao() {
    int ano;
    printf("Ano da eleicao que deseja excluir: ");
    scanf("%d", &ano);

    for (int i = 0; i <total_de_elecao; i++) {
        if (vetor_eleicao[i]->ano == ano) {
            free(vetor_eleicao[i]);

            for (int j = i; j <total_de_elecao - 1; j++) {
                vetor_eleicao[j] = vetor_eleicao[j + 1];
            }

           total_de_elecao--;
            vetor_eleicao = realloc(vetor_eleicao,total_de_elecao * sizeof(eleicao*));
            salvar_eleicoes_no_arquivo();
            printf("Eleição excluída!\n");
            return;
        }
    }
    printf("Eleição não encontrada!\n");
}

void mostrar_eleiçoes() {
    printf("\n");
    int opcao;
    printf("1 - listar todas as eleições. 2 - procurar eleicao por ano: ");
    scanf("%d", &opcao); // Leitura da opção

    switch (opcao) {
        case 1:
            listar_todas_eleiçoes();
            break;

        case 2: { // Bloco para declaração de variável
            int ano;
            printf("Digite o ano da eleicao: ");
            scanf("%d", &ano);
            buscar_eleicao_por_ano(ano);
            break;
        }
        default:
            printf("Opção inválida!");
    }
}

void menu_principal_eleicao(){
    int opcao;
    do {


        printf("1 - adicionar eleiçao. 2 - listar todas.\n");
        printf("3 - editar eleiçao. 4 - excluir eleicao.\n");
        printf("5 - buscar eleicao por ano. 0 - sair.\n");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                adicionar_eleicao_ui();
                break;

            case 2:

                listar_todas_eleiçoes();
                break;

            case 3:

                editar_eleicao();
                break;

            case 4:

                excluir_eleicao();
                break;

            case 5:
                mostrar_eleiçoes();
                break;

            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);
}

void func_main_eleicao() {
    carregar_eleiçoes();
    menu_principal_eleicao();
    liberar_memoria_eleicao();
}


int main() {

    carregar_eleiçoes();

    printf("\n");
    printf("==================================\n");
    printf("  bem vindo as eleçoes IFCE 2025  \n");
    printf("==================================\n");
    printf("\n");
    printf("\n");

    menu_principal_eleicao();
    liberar_memoria_eleicao();
    return 0;
}
