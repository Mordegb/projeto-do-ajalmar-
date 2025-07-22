#include <stdio.h>
#include <stdlib.h>
#include "pessoa.c"
#include "estado.c"
//#include "eleicao.c"
enum {estado = 1, pessoa = 2, eleicaao = 3 ,};




int main() {

    printf("==================================\n");
    printf("  trabalho eduardo almeida 2025  \n");
    printf("==================================\n");

    int opcao;
    do {
        printf("oque deseja realizar em seu banco de dados?.\n");
        printf("1 - configurar estado. 2 - configurar pessoa.\n");
        printf("3 - configurar eleicao.\n");
        scanf("%d", &opcao);
        limpar_getchar();

        switch (opcao) {
            case estado:
                func_main_uf();
                break;

            case pessoa:
               func_main_pessoa();
                break;
            case eleicaao:
              //  func_main_eleicao();
                break;
            case 4:
                editar_estado();
                break;
            case 5:
                remover_estado();
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);


    return 0;
}#include <stdio.h>
#include <stdlib.h>
#include "pessoa.h"
#include "estado.h"

enum {estado = 1, pessoa = 2, };




int main() {

    printf("==================================\n");
    printf("  trabalho eduardo almeida 2025  \n");
    printf("==================================\n");

    int opcao;
    do {
        printf("oque deseja realizar em seu banco de dados?.\n");
        printf("1 - configurar estado. 2 - configurar pessoa.\n");
        scanf("%d", &opcao);
        limpar_getchar();

        switch (opcao) {
            case estado:
                func_main_uf();
                break;

            case pessoa:
                buscar_estado();
                break;
            case 3:
               func_main_pessoa();
                break;
            case 4:
                editar_estado();
                break;
            case 5:
                remover_estado();
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);


    return 0;
}
