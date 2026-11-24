#ifndef SEMANTICO_H
#define SEMANTICO_H

typedef enum {
    TIPO_ERRO = 0,
    TIPO_VOID,
    TIPO_INT,
    TIPO_REAL,
    TIPO_STRING,
    TIPO_STRUCT
} TipoBase;

typedef struct {
    TipoBase base;
    int dimensao;           /* 0 = escalar, 1 = array [], 2 = matriz [][], etc. */
    char nome_struct[64];   /* Preenchido quando base == TIPO_STRUCT */
} Tipo;

/* Estrutura para trafegar texto + tipo nas regras de expressão do Bison */
typedef struct {
    char *texto;
    Tipo tipo;
} ExprInfo;

/* Representação de um campo dentro de uma struct */
typedef struct {
    char nome[64];
    Tipo tipo;
} CampoStruct;

/* Definição de um tipo struct na tabela global de structs */
typedef struct StructDef {
    char nome[64];
    CampoStruct campos[32];
    int num_campos;
    struct StructDef *prox;
} StructDef;

/* Representação de um símbolo (variável ou subrotina) */
typedef struct Simbolo {
    char *nome;
    Tipo tipo;
    int is_func;
    int num_params;
    Tipo *params;          /* Vetor com os tipos dos parâmetros */
    int nivel_escopo;
    struct Simbolo *prox;
} Simbolo;

/* Controle de Escopos e Tabela */
void iniciar_semantico(void);
void entrar_escopo(void);
void sair_escopo(void);

/* Inserção e Busca de Símbolos */
Simbolo* inserir_variavel(const char *nome, Tipo tipo, int linha);
Simbolo* inserir_funcao(const char *nome, Tipo retorno, int num_params, Tipo *params, int linha);
Simbolo* buscar_simbolo(const char *nome);

/* Inserção e Busca de Structs e Campos */
StructDef* inserir_struct(const char *nome, int linha);
StructDef* buscar_struct(const char *nome);
int adicionar_campo_struct(StructDef *st, const char *nome_campo, Tipo tipo_campo, int linha);
Tipo buscar_campo_struct(const char *nome_struct, const char *nome_campo);

/* Utilitários de Tipo e Verificações */
Tipo criar_tipo(TipoBase base, int dimensao);
Tipo criar_tipo_struct(const char *nome_struct, int dimensao);
Tipo parse_tipo_string(const char *str);
const char* tipo_para_string(Tipo t);
int tipos_compativeis(Tipo destino, Tipo origem);
Tipo verificar_binario_aritmetico(Tipo t1, Tipo t2, const char *op, int linha);
Tipo verificar_relacional(Tipo t1, Tipo t2, const char *op, int linha);
void erro_semantico(int linha, const char *fmt, ...);
int obter_erros_semanticos(void);

#endif