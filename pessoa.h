typedef struct  pessoa{
    char CPF[13];
    char titulo[14];
    char nome[50];
    char fone[20];
    char endereço[45];
    char Dnasci[12];
} persona;

FILE* parametro(char str[]);
void carregar_pessoas();
void salvar_pessoas_no_arquivo();
void liberar_memoria();
int verificar_chave_primaria_CPF(const char* CPF);
int verificar_chave_primaria_titulo(const char* titulo);
void adicionar_pessoa(persona* novo);
void adicionar_pessoa_ui();
void excluir_pessoa();
void editar_pessoa();
void listar_todas_pessoas();
void bucar_pessoas_por_titulo(const char* titulo_a_buscar);
void listar_procurar_pessoa();
void menu_principal();
