#ifndef SEMANTICO_H
#define SEMANTICO_H

typedef enum {
    TIPO_ERRO = 0,
    TIPO_VOID,
    TIPO_INT,
    TIPO_REAL,
    TIPO_STRING
} TipoBase;

typedef struct {
    TipoBase base;
    int dimensao; /* 0 = escalar, 1 = array [], 2 = matriz [][], etc. */
} Tipo;

/* Estrutura para trafegar texto + tipo nas regras de expressão do Bison */
typedef struct {
    char *texto;
    Tipo tipo;
} ExprInfo;

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

/* Inserção e Busca */
Simbolo* inserir_variavel(const char *nome, Tipo tipo, int linha);
Simbolo* inserir_funcao(const char *nome, Tipo retorno, int num_params, Tipo *params, int linha);
Simbolo* buscar_simbolo(const char *nome);

/* Utilitários de Tipo e Verificações */
Tipo criar_tipo(TipoBase base, int dimensao);
Tipo parse_tipo_string(const char *str);
const char* tipo_para_string(Tipo t);
int tipos_compativeis(Tipo destino, Tipo origem);
Tipo verificar_binario_aritmetico(Tipo t1, Tipo t2, const char *op, int linha);
Tipo verificar_relacional(Tipo t1, Tipo t2, const char *op, int linha);
void erro_semantico(int linha, const char *fmt, ...);
int obter_erros_semanticos(void);

#endif