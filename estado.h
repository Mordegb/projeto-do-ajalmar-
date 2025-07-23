
typedef struct estado {
    int codigo;
    char nome[20];
    char sigla[4];
} estd;

void limpar_getchar();
FILE* parametro_uf(char str[]);
void carregar_estados();
void salvar_estados_arq();
void liberar_memoria_uf();
int testar_chave_primaria_code(int codigo);
void adicionar_estado(estd* novo);
void adicionar_estado_ui();
void listar_estados();
void buscar_por_nome(const char* nome_a_buscar);
void buscar_por_codigo_uf(int codigo_a_buscar);
void buscar_por_sigla(const char* sigla_a_buscar);
void buscar_estado();
void editar_estado();
void remover_estado();
void menu_principal_uf();
void func_main_uf();
