#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eleicao.h"
#include "estado.h"

/*typedef struct eleição{
     int codigo_uf;
     int ano;
     char descrição[50];
 } eleição;*/

eleição** vetor_eleição = NULL;          //vetor_eleição = vetor_pessoa
int total_de_eleção = 0;

FILE* parametro(char str[]) {

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
    FILE* f = parametro("eleicao.bin");

    fseek(f, 0, SEEK_END);
    long tamanho = ftell(f);
    fseek(f, 0, SEEK_SET);

    total_de_eleção = tamanho / sizeof(eleição);
    vetor_eleição = (eleição**)malloc(total_de_eleção * sizeof(eleição*));

    for (int i = 0; i < total_de_eleção; i++) {
        vetor_eleição[i] = (eleição*)malloc(sizeof(eleição));
        fread(vetor_eleição[i], sizeof(eleição), 1, f);
    }
    fclose(f);
}

void salvar_eleicoes_no_arquivo() {
    FILE* f = fopen("eleicao.bin", "wb"); // Modo escrita binária (sobrescreve)
    if (!f) {
        printf("Erro ao abrir arquivo para escrita!\n");
        return;
    }
    for (int i = 0; i < total_de_eleção; i++) {
        fwrite(vetor_eleição[i], sizeof(eleição), 1, f);
    }

    fclose(f);
}

void liberar_memoria_eleição() {
    for (int i = 0; i < total_de_eleção; i++) {
        free(vetor_eleição[i]);
    }
    free(vetor_eleição);
    vetor_eleição = NULL;
    total_de_eleção = 0;
}

int verificar_chave_primaria_ano(int ano) {
    for (int i = 0; i < total_de_eleção; i++) {
        if (vetor_eleição[i] -> ano == ano) {
            return 0;
        }
    }
    return 1;
}


int verificar_se_estado_existe(int codigo_uf) {
    if (testar_chave_primaria_code(codigo_uf)) {
        buscar_por_codigo_uf(codigo_uf);
        return 1;
    }
    else {
        printf("Estado não cadastrado em nossos registros!\n");
        return 0;
    }
}

void adicionar_eleição(eleição* novo) {
   total_de_eleção++;
   vetor_eleição = (eleição**)realloc(vetor_eleição,total_de_eleção * sizeof(eleição*));
   vetor_eleição[total_de_eleção - 1] = (eleição*)malloc(sizeof(eleição));
        vetor_eleição[total_de_eleção - 1] -> ano = novo -> ano;
        vetor_eleição[total_de_eleção - 1] -> codigo_uf = novo -> codigo_uf;
        strncpy(vetor_eleição[total_de_eleção - 1] -> descrição, novo -> descrição, sizeof(novo -> descrição) - 1);

    salvar_eleicoes_no_arquivo();
}


void adicionar_eleição_ui() {
    eleição* novo = (eleição*)malloc(sizeof(eleição));

    int codigo_valido = 0;

    while (!codigo_valido) {
        printf("\n");
        printf("ano da eleição:\t ");
       scanf("%d", &novo -> ano);
        if (novo -> ano < 1000) {
            printf("o ano precisa ter 4 digitos.");
        }
        if (verificar_chave_primaria_ano(novo->ano) == 0) {
            printf("ja teve uma eleição esse ano.\n");
            continue;
        } else {
            codigo_valido = 1;
        }
    }
    codigo_valido = 0;
    while (!codigo_valido) {
        printf("codigo do estado\t ");
        scanf("%d", &novo -> codigo_uf);
        if (verificar_se_estado_existe(novo -> codigo_uf) == 0){
            continue;
        }
        else {
            codigo_valido = 1;
        }
    }

        printf("Descrição da eleição:\t");
        fflush(stdin);
        fgets(novo -> descrição, 50, stdin);

        adicionar_eleição(novo);
        free(novo);
        printf("\n");
        printf("\n");
}

void excluir_eleição() {
    int ano;
    printf("Ano da eleição que deseja excluir: ");
    scanf("%d", &ano);

    for (int i = 0; i < total_de_eleção; i++) {
        if (vetor_eleição[i]->ano == ano) {
            free(vetor_eleição[i]);

            for (int j = i; j < total_de_eleção - 1; j++) {
                vetor_eleição[j] = vetor_eleição[j + 1];
            }

            total_de_eleção--;
            vetor_eleição = realloc(vetor_eleição, total_de_eleção * sizeof(eleição*));
            salvar_eleicoes_no_arquivo();
            printf("Eleição excluída!\n");
            return;
        }
    }
    printf("Eleição não encontrada!\n");
}

void editar_eleiçao() {
    int ano;
    printf("Ano da eleição que deseja editar: ");
    scanf("%d", &ano);

    for (int i = 0; i < total_de_eleção; i++) {
        if (vetor_eleição[i]->ano == ano) {
            int novo_ano;
            printf("Novo ano [atual: %d]: ", vetor_eleição[i]->ano);
            scanf("%d", &novo_ano);

            if (novo_ano != vetor_eleição[i]->ano && verificar_chave_primaria_ano(novo_ano)) {
                printf("Já existe eleição neste ano!\n");
                return;
            }
            vetor_eleição[i]->ano = novo_ano;

            printf("Novo código UF [atual: %d]: ", vetor_eleição[i]->codigo_uf);
            scanf("%d", &vetor_eleição[i]->codigo_uf);

            printf("Nova descrição [atual: %s]: ", vetor_eleição[i]->descrição);
            getchar();
            fgets(vetor_eleição[i]->descrição, 50, stdin);
            vetor_eleição[i]->descrição[strcspn(vetor_eleição[i]->descrição, "\n")] = '\0';

            salvar_eleicoes_no_arquivo();
            printf("Eleição atualizada!\n");
            return;

        }
    }
    printf("pessoa não encontrado!\n");
}

void listar_todas_eleiçoes() {
    for (int i = 0; i < total_de_eleção; i++) {
        printf("\n");
        printf("Ano: %s\n", vetor_eleição[i]->descrição);
        printf("estado que aconteceu: %d\n", vetor_eleição[i] -> codigo_uf);
        printf("descrição: %s\n", vetor_eleição[i] -> descrição);

        printf("\n");
    }
}

void buscar_eleição_por_ano(const int ano_a_buscar) {
    for (int i = 0; i < total_de_eleção; i++) {
        if (vetor_eleição[i] -> ano == ano_a_buscar) {
            printf("ano da eleição: %s\n", vetor_eleição[i] -> ano);
            printf("codigo do estado da eleição: %s\n", vetor_eleição[i] -> codigo_uf);
            printf("descrição: %s\n", vetor_eleição[i] -> descrição);

            printf("\n");
            return;
        }
    }
    printf("eleição não encotrda!\n");
}

void mostrar_eleiçoes() {
    printf("\n");
    int opcao;
    printf("1 - listar todas as eleições. 2 - procurar eleição por ano: ");
    scanf("%d", &opcao); // Leitura da opção

    switch (opcao) {
        case 1:
            listar_todas_eleiçoes();
            break;

        case 2: { // Bloco para declaração de variável
            int ano;
            printf("Digite o ano da eleição: ");
            scanf("%d", &ano);
            buscar_eleição_por_ano(ano);
            break;
        }
        default:
            printf("Opção inválida!");
    }
}

void menu_principal_eleição() {
    int opcao;
    do {

        printf("1 - adicionar eleiçao. 2 - listar todas.\n");
        printf("3 - editar eleiçao. 4 - excluir eleição.\n");
        printf("5 - buscar eleição por ano. 0 - sair.\n");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                adicionar_eleição_ui();
                break;

            case 2:

                listar_todas_eleiçoes();
                break;

            case 3:

                editar_eleiçao();
                break;

            case 4:

                excluir_eleição();
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

int main() {

    carregar_eleiçoes();

    printf("\n");
    printf("==================================\n");
    printf("  bem vindo as eleçoes IFCE 2025  \n");
    printf("==================================\n");
    printf("\n");
    printf("\n");

menu_principal_eleição();
    liberar_memoria_eleição();
    return 0;
}
