#include <stdio.h>
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
