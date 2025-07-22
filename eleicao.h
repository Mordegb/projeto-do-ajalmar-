
typedef struct eleicao{
     int codigo_uf;
     int ano;
     char descricao[50];
 } eleicao;

void carregar_eleiçoes();
FILE* parametro_eleicao(char str[]);
void salvar_eleicoes_no_arquivo();
void liberar_memoria_eleicao();
int verificar_chave_primaria_ano(int ano);
int verificar_se_estado_existe(int codigo_uf);
void adicionar_eleicao(eleicao* novo);
int verificar_eleicao_estado_ano(int codigo_uf, int ano);
void adicionar_eleicao_ui();
void editar_eleicao();
void listar_todas_eleiçoes();
void buscar_eleicao_por_ano(const int ano_a_buscar);
void excluir_eleicao();
void mostrar_eleiçoes();
void menu_principal_eleicao();
void func_main_eleicao();
