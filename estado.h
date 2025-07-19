
typedef struct estado {
    int codigo;
    char nome[20];
    char sigla[3];
} estd;

void limpar_getchar();
void carregar_estados();
void salvar_estados_arq();
void liberar_memoria();
int testar_chave_primaria(int codigo);
void adicionar_estado(estd* novo);
void adicionar_estado_ui();
void listar_estados();
void buscar_por_nome(const char* nome_a_buscar);
void buscar_por_codigo(int codigo_a_buscar);
void buscar_por_sigla(const char* sigla_a_buscar);
void buscar_estado();
void editar_estado();
void remover_estado();
void menu_principal();


