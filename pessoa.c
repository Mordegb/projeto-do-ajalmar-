#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pessoa.h"



persona** vetor_pessoa = NULL;          //estados = vetor_pessoa
int total_pessoas = 0;


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

void carregar_pessoas() {
    FILE* f = parametro("pessoa.bin");

    fseek(f, 0, SEEK_END);
    long tamanho = ftell(f);
    fseek(f, 0, SEEK_SET);

    total_pessoas = tamanho / sizeof(persona);
    vetor_pessoa = (persona**)malloc(total_pessoas * sizeof(persona*));

    for (int i = 0; i < total_pessoas; i++) {
        vetor_pessoa[i] = (persona*)malloc(sizeof(persona));
        fread(vetor_pessoa[i], sizeof(persona), 1, f);
    }
    fclose(f);
}



void salvar_pessoas_no_arquivo() {
    FILE* f = fopen("pessoa.bin", "wb"); // Modo escrita binária (sobrescreve)
    if (!f) {
        printf("Erro ao abrir arquivo para escrita!\n");
        return;
    }

    for (int i = 0; i < total_pessoas; i++) {
        fwrite(vetor_pessoa[i], sizeof(persona), 1, f);
    }

    fclose(f);
}

void liberar_memoria() {
    for (int i = 0; i < total_pessoas; i++) {
        free(vetor_pessoa[i]);
    }
    free(vetor_pessoa);
    vetor_pessoa = NULL;
    total_pessoas = 0;
}


                                                // Verifica se um cpf já existe (terminar dps)
int verificar_chave_primaria_CPF(const char* CPF) {
    for (int i = 0; i < total_pessoas; i++) {
        if (strcmp(vetor_pessoa[i]->CPF, CPF) == 0) {  // Use strcmp()
            return 1;
        }
    }
    return 0;
}

int verificar_chave_primaria_titulo(const char* titulo) {
    for (int i = 0; i < total_pessoas; i++) {
        if (strcmp(vetor_pessoa[i]->titulo, titulo) == 0) {
            return 1;
        }
    }
    return 0;
}

void adicionar_pessoa(persona* novo) {
   total_pessoas++;
    vetor_pessoa = (persona**)realloc(vetor_pessoa, total_pessoas * sizeof(persona*));
    vetor_pessoa[total_pessoas - 1] = (persona*)malloc(sizeof(persona));

    strncpy(vetor_pessoa[total_pessoas - 1]->CPF, novo->CPF, sizeof(novo->CPF) - 1);
    strncpy(vetor_pessoa[total_pessoas - 1]->titulo, novo->titulo, sizeof(novo->titulo) - 1);
    strncpy(vetor_pessoa[total_pessoas - 1]->nome, novo->nome, sizeof(novo->nome) - 1);
    strncpy(vetor_pessoa[total_pessoas - 1]->fone, novo->fone, sizeof(novo->fone) - 1);
    strncpy(vetor_pessoa[total_pessoas - 1]->endereço, novo->endereço, sizeof(novo->endereço) - 1);
    strncpy(vetor_pessoa[total_pessoas - 1]->Dnasci, novo->Dnasci, sizeof(novo->Dnasci) - 1);

    salvar_pessoas_no_arquivo();
}

void adicionar_pessoa_ui() {
    persona* novo = (persona*)malloc(sizeof(persona));

    int codigo_valido = 0;

    while (!codigo_valido) {
        printf("\n");
        printf("CPF da pessoa:\t ");
        fflush(stdin);
        gets(novo->CPF);
        if (verificar_chave_primaria_CPF(novo->CPF)) {
            printf("CPF ja cadastrado.\n");
            continue;
        } else {
            codigo_valido = 1;
        }
    }
    codigo_valido = 0;
    while (!codigo_valido) {

        printf("titulo de eleitor da pessoa: \t");
        fflush(stdin);
        gets(novo->titulo);
        if (verificar_chave_primaria_titulo(novo->titulo)) {
            printf("titulo ja cadastrado.\n");
            continue;
        } else {
            codigo_valido = 1;
        }
    }

        printf("Nome da pessoa:\t");
        fflush(stdin);
        gets(novo -> nome);

        printf("Fone da pessoa:\t");
        fflush(stdin);
        gets(novo -> fone);

        printf("endereço da pessoa:\t");
        fflush(stdin);
        gets(novo -> endereço);

        printf("Data de nascimento da pessoa:\t");
        fflush(stdin);
        gets(novo -> Dnasci);

        adicionar_pessoa(novo);
        free(novo);
        printf("\n");
        printf("\n");

}


void excluir_pessoa() {
    char CPF[13];
    printf("CPF da pessoa que deseja excluir: ");
    fflush(stdin);
    gets(CPF);

    for (int i = 0; i < total_pessoas; i++) {
        if (strcmp(vetor_pessoa[i]->CPF, CPF) == 0) {
            free(vetor_pessoa[i]);

            for (int j = i; j < total_pessoas - 1; j++) {
                vetor_pessoa[j] = vetor_pessoa[j + 1];
            }

            total_pessoas--;
            vetor_pessoa = (persona**)realloc(vetor_pessoa, total_pessoas * sizeof(persona*));
            salvar_pessoas_no_arquivo();
            printf("voce matou %s  \n" , vetor_pessoa[i]->nome);
            return;
        }
    }
    printf("pessoa não encontrada.\n");
}


void editar_pessoa() {
    char CPF[13];
    printf("CPF da pessoa que deseja excluir: ");
    fflush(stdin);
    gets(CPF);

    for (int i = 0; i < total_pessoas; i++) {
        if (strcmp(vetor_pessoa[i]->CPF, CPF) == 0)  {
            printf("Novo nome [atual: %s]: ", vetor_pessoa[i] -> nome);
            fflush(stdin);
            gets(vetor_pessoa[i] -> nome);
            vetor_pessoa[i] -> nome[strcspn(vetor_pessoa[i] -> nome, "\n")] = '\0';


            printf("Novo numero [atual: %s]: ", vetor_pessoa[i] -> fone);
            fflush(stdin);
            gets(vetor_pessoa[i] -> fone);
            vetor_pessoa[i] -> fone[strcspn(vetor_pessoa[i] -> fone, "\n")] = '\0';

            printf("Novo endereço [atual: %s]: ", vetor_pessoa[i] -> endereço);
            fflush(stdin);
            gets(vetor_pessoa[i] -> endereço);
            vetor_pessoa[i] -> endereço[strcspn(vetor_pessoa[i] -> endereço, "\n")] = '\0';

            printf("Novo data de nascimento [atual: %s]: ", vetor_pessoa[i] -> Dnasci);
            fflush(stdin);
            gets(vetor_pessoa[i] -> Dnasci);
            vetor_pessoa[i] -> Dnasci[strcspn(vetor_pessoa[i] -> Dnasci, "\n")] = '\0';

            salvar_pessoas_no_arquivo();
            printf("pessoa atualizado!\n");
            return;
        }
    }
    printf("pessoa não encontrado!\n");
}


void listar_todas_pessoas() {
    for (int i = 0; i < total_pessoas; i++) {
        printf("\n");
        printf("CPF: %s\n", vetor_pessoa[i]->CPF);
        printf("titulo: %s\n", vetor_pessoa[i] -> titulo);
        printf("nome: %s\n", vetor_pessoa[i] -> nome);
        printf("fone: %s\n", vetor_pessoa[i] -> fone);
        printf("endereço: %s\n", vetor_pessoa[i] -> endereço);
        printf("data de nascimento: %s\n", vetor_pessoa[i] -> Dnasci);
        printf("\n");
    }
}

void bucar_pessoas_por_titulo(const char* titulo_a_buscar) {
    for (int i = 0; i < total_pessoas; i++) {
        if (strcmp(vetor_pessoa[i] -> titulo, titulo_a_buscar) == 0) {
            printf("CPF: %s\n", vetor_pessoa[i]->CPF);
            printf("titulo: %s\n", vetor_pessoa[i] -> titulo);
            printf("nome: %s\n", vetor_pessoa[i] -> nome);
            printf("fone: %s\n", vetor_pessoa[i] -> fone);
            printf("endereço: %s\n", vetor_pessoa[i] -> endereço);
            printf("data de nascimento: %s\n", vetor_pessoa[i] -> Dnasci);
            printf("\n");
            return;
        }
    }
    printf("pessoa não encontrado!\n");
}

void listar_procurar_pessoa() {
    int opcao;
    printf("1 - listar todas as pessoas. 2 - buscar pessoa pelo titulo \n");
    scanf("%d", &opcao);

    switch (opcao) {
        case 1:
            listar_todas_pessoas();
            break;

        case 2:
            printf("Digite o titulo da pessoa: ");
            fflush(stdin);
            char titulo[14];
            gets(titulo);
            bucar_pessoas_por_titulo(titulo);
    }
}


void menu_principal() {
    int opcao;
    do {

        printf("1 - adicionar pessoa. 2 - listar / procurar pessoa.\n");
        printf("3 - editar pessoa. 4 - excluir pessoa.\n");
        printf("0 - sair\n");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                adicionar_pessoa_ui();
                break;

            case 2:

                listar_procurar_pessoa();
                break;

            case 3:

                editar_pessoa();
                break;

            case 4:

                excluir_pessoa();
                break;

            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);
}

void func_main_pessoa()
  {
    carregar_pessoas();
    menu_principal();
    liberar_memoria();
}
int main() {
    carregar_pessoas();

    printf("\n");
    printf("==================================\n");
    printf("  bem vindo as eleçoes IFCE 2025  \n");
    printf("==================================\n");
    printf("\n");
    printf("\n");

    menu_principal();
    liberar_memoria();
    return 0;
}
