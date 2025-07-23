#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estado.c"
#include "pessoa.c"
#include "eleicao.c"
#include "candidatoEleicao.h"

cdte** vetor_de_candidatos = NULL;          //vetor_eleicao = vetor_pessoa
int total_de_candidatos = 0;

FILE* parametro_candidato(char str[]) {

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

void carregar_candidatos_eleicao() {
    FILE* f = parametro_candidato("candidatos.bin");

    fseek(f, 0, SEEK_END);
    long tamanho = ftell(f);
    fseek(f, 0, SEEK_SET);

    total_de_candidatos = tamanho / sizeof(cdte);
    vetor_de_candidatos = (cdte**)malloc(total_de_candidatos * sizeof(cdte*));

    for (int i = 0; i < total_de_candidatos; i++) {
        vetor_de_candidatos[i] = (cdte*)malloc(sizeof(cdte));
        fread(vetor_de_candidatos[i], sizeof(cdte), 1, f);
    }
    fclose(f);
}

void salvar_candidatos_no_arquivo() {
    FILE* f = fopen("candidatos.bin", "wb"); // Modo escrita binária (sobrescreve)
    if (!f) {
        printf("Erro ao abrir arquivo para escrita!\n");
        return;
    }
    for (int i = 0; i < total_de_candidatos; i++) {
        fwrite(vetor_de_candidatos[i], sizeof(cdte), 1, f);
    }

    fclose(f);
}

void liberar_memoria_candidatos() {
    for (int i = 0; i < total_de_candidatos; i++) {
        free(vetor_de_candidatos[i]);
    }
    free(vetor_de_candidatos);
    vetor_de_candidatos = NULL;
    total_de_candidatos = 0;
}

int verificar_chave_primaria_ano_para_eleicao(int ano) {
    for (int i = 0; i < total_de_elecao; i++) {
        if (vetor_eleicao[i] -> ano == ano) {
            return 1;
        }
    }
    return 0;
}
int verificar_se_pessoa_existe(char CPF[]) {
    carregar_pessoas();

    for (int i = 0; i < total_pessoas; i++) {
        if (vetor_pessoa[i]->CPF == CPF) {
            buscar_pessoas_por_CPF(CPF);
            return 1;
        }
    }

    return 0;
}


void adicionar_candidato(cdte* novo) {
    total_de_candidatos++;
    vetor_de_candidatos = (cdte**)realloc(vetor_de_candidatos,total_de_candidatos * sizeof(cdte*));
    vetor_de_candidatos[total_de_candidatos - 1] = (cdte*)malloc(sizeof(cdte));
    vetor_de_candidatos[total_de_candidatos - 1] -> ano = novo -> ano;
    vetor_de_candidatos[total_de_candidatos - 1] -> codigo_uf = novo -> codigo_uf;
    strncpy(vetor_de_candidatos[total_de_candidatos - 1] -> CPF, novo -> CPF, sizeof(novo -> CPF) - 1);
    vetor_de_candidatos[total_de_candidatos - 1] -> numero_urna = novo -> numero_urna;
    salvar_eleicoes_no_arquivo();
}


void adicionar_candidato_ui() {
    cdte* novo = (cdte*)malloc(sizeof(cdte));
    int codigo_valido = 0;

    printf("Ano da eleicao:\t ");
    scanf("%d", &novo->ano);
    while (!codigo_valido) {
        if (novo -> ano < 1000) {
            printf("ano invalido, precisa ter 4 digitos");
            continue;
        }
        if (verificar_chave_primaria_ano_para_eleicao(novo->ano)) {
            // teve eleição nesse ano
        }
        else {
            printf("Estado inválido! Tente novamente.\n");
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

        printf("CPF do candidato:\t");
        fflush(stdin);
        fgets(novo->CPF, 11, stdin);
        novo->CPF[strcspn(novo->CPF, "\n")] = '\0';// Remove a quebra de linha
        if (verificar_se_pessoa_existe(novo->CPF)) {
            // Estado existe, aceita
        }
        else {
            printf("pessoa não existe.\n");
            continue;
        }
        codigo_valido = 1;
        }

        printf("numero de urna do candidato:");
        scanf("%d", &novo->numero_urna);

        adicionar_candidato(novo);
    }



void excluir_candidato(int codigo_uf, int ano, int numero_urna) {
    int encontrado = 0;

    for (int i = 0; i < total_de_candidatos; i++) {
        if (vetor_de_candidatos[i]->codigo_uf == codigo_uf && vetor_de_candidatos[i]->ano == ano && vetor_de_candidatos[i]->numero_urna == numero_urna) {

            // Remove o candidato encontrado
            free(vetor_de_candidatos[i]);

            // Move os elementos restantes
            for (int j = i; j < total_de_candidatos - 1; j++) {
                vetor_de_candidatos[j] = vetor_de_candidatos[j + 1];
            }

            total_de_candidatos--;
            encontrado = 1;

            // Realoca o vetor
            cdte** temp = realloc(vetor_de_candidatos, total_de_candidatos * sizeof(cdte*));
            if (temp != NULL || total_de_candidatos == 0) {
                vetor_de_candidatos = temp;
            } else {
                printf("Erro ao realocar memória!\n");
            }

            salvar_candidatos_no_arquivo();
            printf("Candidato excluído com sucesso!\n");
            break;
            }
    }

    if (!encontrado) {
        printf("Candidato não encontrado com os dados fornecidos.\n");
    }
}

void excluir_candidato_ui() {
    int codigo_uf, ano, numero_urna;

    printf("Digite o código do estado (UF): ");
    scanf("%d", &codigo_uf);

    printf("Digite o ano da eleição: ");
    scanf("%d", &ano);

    printf("Digite o número da urna do candidato: ");
    scanf("%d", &numero_urna);

    excluir_candidato(codigo_uf, ano, numero_urna);
}


void listar_candidatos_por_eleicao(int ano, int codigo_uf) {
    int encontrados = 0;

    printf("\nCandidatos da eleição %d no estado %d:\n", ano, codigo_uf);
    printf("=====================================\n");

    for (int i = 0; i < total_de_candidatos; i++) {
        if (vetor_de_candidatos[i]->ano == ano &&
            vetor_de_candidatos[i]->codigo_uf == codigo_uf) {

            encontrados = 1;
            printf("CPF: %s\n", vetor_de_candidatos[i]->CPF);
            printf("Número na urna: %d\n", vetor_de_candidatos[i]->numero_urna);

            // Buscar e mostrar informações da pessoa pelo CPF
            carregar_pessoas();
            for (int j = 0; j < total_pessoas; j++) {
                if (strcmp(vetor_pessoa[j]->CPF, vetor_de_candidatos[i]->CPF) == 0) {
                    printf("Nome: %s\n", vetor_pessoa[j]->nome);
                    break;
                }
            }
    printf("=====================================\n");
            }
    }

    if (!encontrados) {
        printf("Nenhum candidato encontrado para esta eleição e estado.\n");
    }
}


void listar_candidatos_ui() {
    int ano;
    int codigo_uf;

    printf("Digite o ano da eleição: ");
    scanf("%d", &ano);

    printf("Digite o código do estado (UF): ");
    scanf("%d", &codigo_uf);

    listar_candidatos_por_eleicao(ano, codigo_uf);
}



void listar_candidatos_por_ano_organizado_por_uf(int ano) {
    // Primeiro, verificar se há candidatos no ano especificado
    int encontrados = 0;

    // Carregar estados para obter nomes
    carregar_estados();

    // Percorrer todos os estados
    for (int uf_index = 0; uf_index < total_estados; uf_index++) {
        int estado_com_candidatos = 0;

        // Verificar se há candidatos neste estado/ano
        for (int i = 0; i < total_de_candidatos; i++) {
            if (vetor_de_candidatos[i]->ano == ano &&
                vetor_de_candidatos[i]->codigo_uf == estados[i]->codigo) {

                // Se é o primeiro candidato deste estado, imprimir cabeçalho
                if (!estado_com_candidatos) {
                    printf("\n=========================================\n");
                    printf("Estado: %s (Código: %d)\n", estados[i]->nome, estados[i]->codigo);
                    printf("=========================================\n");
                    estado_com_candidatos = 1;
                    encontrados = 1;
                }

                // Imprimir detalhes do candidato
                printf("CPF: %s\n", vetor_de_candidatos[i]->CPF);
                printf("Número na urna: %d\n", vetor_de_candidatos[i]->numero_urna);

                // Buscar e mostrar nome da pessoa
                carregar_pessoas();
                for (int j = 0; j < total_pessoas; j++) {
                    if (strcmp(vetor_pessoa[j]->CPF, vetor_de_candidatos[i]->CPF) == 0) {
                        printf("Nome: %s\n", vetor_pessoa[j]->nome);
                        break;
                    }
                }
                printf("-----------------------------------------\n");
                }
        }
    }

    if (!encontrados) {
        printf("\nNenhum candidato encontrado para o ano %d.\n", ano);
    }
}

void listar_candidatos_por_ano_ui() {
    int ano;

    printf("Digite o ano da eleição: ");
    scanf("%d", &ano);

    listar_candidatos_por_ano_organizado_por_uf(ano);
}

