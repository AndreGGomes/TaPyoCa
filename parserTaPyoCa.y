%code requires {
    #include "semantico.h"

    extern int yylineno;
    extern char *yytext;
    extern FILE *yyin;

    typedef struct {
        char *nomes[32];
        Tipo tipos[32];
        int is_ref[32];
        int count;
        char *texto;
        char *c_texto;
    } ListaParams;

    typedef struct {
        Tipo tipos[32];
        int is_ref[32];
        char *nomes[32];
        char *c_itens[32];
        int count;
        char *texto;
        char *c_texto;
    } ListaArgs;
}
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "semantico.h"

#define MAX_IFS 100
#define MAX_LOOPS 100

typedef struct {
    char *label_else;
    char *label_fim;
} IfLabels;

typedef struct {
    char *inicio;
    char *fim;
} LoopLabels;

int yylex(void);
int yyerror(char *s);
extern int yylineno;
extern char *yytext;

/* ---------- utilitarios de impressao ---------- */

static int depth = 0;       /* nivel de aninhamento atual (4 espacos por tab) */
static int loop_depth = 0;  /* controla quantos lacos estao ativos */
static IfLabels if_stack[MAX_IFS];
static int if_top = -1;
static LoopLabels loop_stack[MAX_LOOPS];
static int loop_top = -1;

/* Monta uma string nova (malloc) estilo printf. Quem recebe deve dar free. */
static char *fmt(const char *f, ...) {
    va_list ap, ap2;
    va_start(ap, f);
    va_copy(ap2, ap);
    int n = vsnprintf(NULL, 0, f, ap);
    va_end(ap);
    char *s = malloc(n + 1);
    vsnprintf(s, n + 1, f, ap2);
    va_end(ap2);
    return s;
}

/* Imprime uma linha indentada conforme o nivel atual */
static void pr(const char *f, ...) {
    va_list ap;
    for (int i = 0; i < depth; i++) printf("    ");
    va_start(ap, f);
    vprintf(f, ap);
    va_end(ap);
    printf("\n");
}

static int novo_label(void) {
    static int label_count = 0;
    return label_count++;
}

static char *new_label(void) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "L%d", novo_label());
    return strdup(buffer);
}

static void push_if(char *label_else, char *label_fim) {
    if (if_top >= MAX_IFS - 1) {
        fprintf(stderr, "Erro: limite de ifs aninhados excedido.\n");
        exit(1);
    }
    if_top++;
    if_stack[if_top].label_else = label_else;
    if_stack[if_top].label_fim  = label_fim;
}

static void pop_if(void) {
    if (if_top >= 0) {
        free(if_stack[if_top].label_else);
        free(if_stack[if_top].label_fim);
        if_top--;
    }
}

static void push_loop(char *inicio, char *fim) {
    if (loop_top >= MAX_LOOPS - 1) {
        fprintf(stderr, "Erro: limite de lacos aninhados excedido.\n");
        exit(1);
    }
    loop_top++;
    loop_stack[loop_top].inicio = inicio;
    loop_stack[loop_top].fim = fim;
}

static void pop_loop(void) {
    if (loop_top >= 0) {
        free(loop_stack[loop_top].inicio);
        free(loop_stack[loop_top].fim);
        loop_top--;
    }
}

static FILE *out_c = NULL;

/* Emite uma linha indentada no arquivo C */
static void emit_c(const char *f, ...) {
    if (!out_c) return;
    for (int i = 0; i < depth; i++) fprintf(out_c, "    ");
    va_list ap;
    va_start(ap, f);
    vfprintf(out_c, f, ap);
    va_end(ap);
    fprintf(out_c, "\n");
}

/* Converte Tipo (TaPyoCa) para sintaxe C
   (ex: int[] -> int*, string -> char*, bool -> bool, Aluno -> struct Aluno) */
static char *tipo_para_c(Tipo t) {
    char base[96];
    switch (t.base) {
        case TIPO_VOID:   strcpy(base, "void"); break;
        case TIPO_REAL:   strcpy(base, "double"); break;
        case TIPO_STRING: strcpy(base, "char*"); break;
        case TIPO_BOOL:   strcpy(base, "bool"); break;
        case TIPO_STRUCT: snprintf(base, sizeof(base), "struct %s", t.nome_struct); break;
        default:          strcpy(base, "int"); break;
    }
    char buf[128];
    strcpy(buf, base);
    for (int i = 0; i < t.dimensao && i < 10; i++) {
        strcat(buf, "*");
    }
    return strdup(buf);
}

/* ---------- estruturas auxiliares para o semantico ---------- */

static Tipo retorno_esperado;
static int encontrou_return = 0;
static StructDef *struct_atual = NULL;   /* struct sendo declarada no momento */

%}

%code {
static void free_lista_args(ListaArgs *la) {
    if (!la) return;
    for (int i = 0; i < la->count; i++) {
        free(la->nomes[i]);
        free(la->c_itens[i]);
    }
    free(la->texto);
    free(la->c_texto);
    free(la);
}
}

%union {
    int ival;
    double fval;
    char *sval;
    ExprInfo expr;
    ListaParams *plist;
    ListaArgs *alist;
}

%token MAIN LPAREN RPAREN NEWLINE INDENT DEDENT ENDMAIN SUB ENDSUB COMMA
%token ASSIGN RETURN IF ELSE ENDIF WHILE ENDWHILE INT LBRACKET RBRACKET STRING REAL BOOL
%token TRUE FALSE NULL_VAL
%token LT LEQ GT GEQ EQ NEQ AND OR NOT PLUS MINUS MUL DIV POW PRINT READ REF
%token STRUCT ENDSTRUCT DOT
%token BREAK

%token <ival> INT_NUM
%token <fval> REAL_NUM
%token <sval> ID
%token <sval> STRUCT_ID
%token <sval> STRING_LITERAL

/* Nao-terminais tipados */
%type <sval> tipo tipo_opt op_relacional
%type <plist> parametros_opt lista_parametros
%type <alist> argumentos_opt lista_argumentos array_elements_opt
%type <expr> expressao explogica exp_and exp_not exprelacional exparitimetica termo unario
%type <expr> l_value fator potencia arrayLiteral chamadafuncao

%start programa

%%

programa
    : declaracoes_globais
        {
            if (obter_erros_semanticos() == 0) {
                printf("Analise sintatica e semantica concluida com sucesso.\n");
            } else {
                fprintf(stderr, "Falha na compilacao: %d erro(s) semantico(s).\n", obter_erros_semanticos());
                YYABORT;
            }
        }
    ;

declaracoes_globais
    : declaracoes_globais declaracao {}
    | declaracao                     {}
    ;

declaracao
    : subrotina         {}
    | main_block        {}
    | declaracao_struct {}
    ;

/* ---------- structs ---------- */

declaracao_struct
    : STRUCT ID
        {
            pr("[struct] %s", $2);
            struct_atual = inserir_struct($2, yylineno);
            emit_c("struct %s {", $2);
            depth++;
        }
      NEWLINE corpo_struct ENDSTRUCT NEWLINE
        {
            if (struct_atual && struct_atual->num_campos == 0) {
                erro_semantico(yylineno, "Struct '%s' deve possuir ao menos um campo.", $2);
            }
            depth--;
            emit_c("};\n");
            pr("[endstruct]");
            printf("\n");
            struct_atual = NULL;
            free($2);
        }
    ;

corpo_struct
    : INDENT campos DEDENT {}
    | /* vazio */          {}
    ;

campos
    : campos campo NEWLINE {}
    | /* vazio */          {}
    ;

campo
    : REF tipo ID
        {
            pr("[campo] ref %s %s", $2, $3);
            Tipo t = parse_tipo_string($2);
            adicionar_campo_struct(struct_atual, $3, t, 1, yylineno);
            char *t_c = tipo_para_c(t);
            emit_c("%s *%s;", t_c, $3);
            free(t_c); free($2); free($3);
        }
    | tipo ID
        {
            pr("[campo] %s %s", $1, $2);
            Tipo t = parse_tipo_string($1);
            adicionar_campo_struct(struct_atual, $2, t, 0, yylineno);
            char *t_c = tipo_para_c(t);
            emit_c("%s %s;", t_c, $2);
            free(t_c); free($1); free($2);
        }
    ;

/* ---------- main e subrotinas ---------- */

main_block
    : MAIN LPAREN RPAREN
        {
            pr("[main]");
            emit_c("int main(void) %c", 123);
            depth++;
            retorno_esperado = criar_tipo(TIPO_VOID, 0);
            encontrou_return = 0;
            entrar_escopo();
        }
      NEWLINE corpo_bloco ENDMAIN NEWLINE
        {
            sair_escopo();
            emit_c("return 0;");
            depth--;
            emit_c("%c\n", 125);
            pr("[endmain]");
            printf("\n");
        }
    ;

subrotina
    : SUB tipo_opt ID LPAREN parametros_opt RPAREN
        {
            if ($2[0]) pr("[sub] %s %s(%s)", $2, $3, $5->texto);
            else       pr("[sub] %s(%s)", $3, $5->texto);

            Tipo ret = parse_tipo_string($2);
            char *ret_c = tipo_para_c(ret);
            emit_c("%s %s(%s) {", ret_c, $3, $5->c_texto);
            free(ret_c);
            depth++;

            /* Registra a funcao no escopo global com os tipos e flags is_ref */
            inserir_funcao_ref($3, ret, $5->count, $5->tipos, $5->is_ref, yylineno);

            /* Abre o escopo local e insere os parametros marcando quais sao 'ref' */
            entrar_escopo();
            for (int i = 0; i < $5->count; i++) {
                inserir_variavel_ref($5->nomes[i], $5->tipos[i], $5->is_ref[i], yylineno);
                free($5->nomes[i]);
            }

            retorno_esperado = ret;
            encontrou_return = 0;

            free($2); free($3); free($5->texto); free($5->c_texto); free($5);
        }
      NEWLINE corpo_bloco ENDSUB NEWLINE
        {
            if (retorno_esperado.base != TIPO_VOID && !encontrou_return) {
                erro_semantico(yylineno, "Subrotina com retorno '%s' deve possuir comando 'return'.",
                               tipo_para_string(retorno_esperado));
            }
            sair_escopo();
            depth--;
            emit_c("}\n");
            pr("[endsub]");
            printf("\n");
        }
    ;

tipo_opt
    : tipo         { $$ = $1; }
    | /* vazio */  { $$ = strdup(""); }
    ;

parametros_opt
    : lista_parametros { $$ = $1; }
    | /* vazio */
        {
            ListaParams *lp = calloc(1, sizeof(ListaParams));
            lp->count = 0;
            lp->texto = strdup("");
            lp->c_texto = strdup("");
            $$ = lp;
        }
    ;

lista_parametros
    : lista_parametros COMMA REF tipo ID
        {
            $$ = $1;
            Tipo t = parse_tipo_string($4);
            char *t_c = tipo_para_c(t);

            if ($$->count < 32) {
                int idx = $$->count++;
                $$->nomes[idx] = $5;
                $$->tipos[idx] = t;
                $$->is_ref[idx] = 1; /* Marca como parametro por referencia */

                char *novo = fmt("%s, ref %s %s", $$->texto, $4, $$->nomes[idx]);
                char *novo_c = (t.dimensao > 0)
                    ? fmt("%s, %s *%s, int *_len_%s", $$->c_texto, t_c, $$->nomes[idx], $$->nomes[idx])
                    : fmt("%s, %s *%s", $$->c_texto, t_c, $$->nomes[idx]);
                free($$->texto); free($$->c_texto);
                $$->texto = novo;
                $$->c_texto = novo_c;
            } else {
                erro_semantico(yylineno, "Limite maximo de 32 parametros excedido.");
                free($5);
            }
            free($4); free(t_c);
        }
    | REF tipo ID
        {
            ListaParams *lp = calloc(1, sizeof(ListaParams));
            Tipo t = parse_tipo_string($2);
            char *t_c = tipo_para_c(t);

            lp->nomes[0] = $3;
            lp->tipos[0] = t;
            lp->is_ref[0] = 1; /* Marca como parametro por referencia */
            lp->count = 1;
            lp->texto = fmt("ref %s %s", $2, $3);
            lp->c_texto = (t.dimensao > 0)
                ? fmt("%s *%s, int *_len_%s", t_c, $3, $3)
                : fmt("%s *%s", t_c, $3);
            free($2); free(t_c);
            $$ = lp;
        }
    | lista_parametros COMMA tipo ID
        {
            $$ = $1;
            Tipo t = parse_tipo_string($3);
            char *t_c = tipo_para_c(t);

            if ($$->count < 32) {
                int idx = $$->count++;
                $$->nomes[idx] = $4;
                $$->tipos[idx] = t;
                $$->is_ref[idx] = 0; /* Parametro normal (por valor) */

                char *novo = fmt("%s, %s %s", $$->texto, $3, $$->nomes[idx]);
                char *novo_c = (t.dimensao > 0)
                    ? fmt("%s, %s %s, int _len_%s", $$->c_texto, t_c, $$->nomes[idx], $$->nomes[idx])
                    : fmt("%s, %s %s", $$->c_texto, t_c, $$->nomes[idx]);
                free($$->texto); free($$->c_texto);
                $$->texto = novo;
                $$->c_texto = novo_c;
            } else {
                erro_semantico(yylineno, "Limite maximo de 32 parametros excedido.");
                free($4);
            }
            free($3); free(t_c);
        }
    | tipo ID
        {
            ListaParams *lp = calloc(1, sizeof(ListaParams));
            Tipo t = parse_tipo_string($1);
            char *t_c = tipo_para_c(t);

            lp->nomes[0] = $2;
            lp->tipos[0] = t;
            lp->is_ref[0] = 0; /* Parametro normal (por valor) */
            lp->count = 1;
            lp->texto = fmt("%s %s", $1, $2);
            lp->c_texto = (t.dimensao > 0)
                ? fmt("%s %s, int _len_%s", t_c, $2, $2)
                : fmt("%s %s", t_c, $2);
            free($1); free(t_c);
            $$ = lp;
        }
    ;

/* ---------- comandos ---------- */

comandos
    : comandos comando NEWLINE {}
    | /* vazio */              {}
    ;

corpo_bloco
    : INDENT comandos DEDENT {}
    | /* vazio */            {}
    ;

comando
    : declaracaovar {}
    | atribuicao    {}
    | comandoif     {}
    | comandowhile  {}
    | comandobreak  {}
    | chamadafuncao { pr("[chamada] %s", $1.texto); emit_c("%s;", $1.texto); free($1.texto); }
    | retorno       {}
    | comandoprint  {}
    ;

comandoprint
    : PRINT LPAREN argumentos_opt RPAREN
        {
            pr("[print] print %s", $3->texto);
            if ($3->count == 0) {
                emit_c("printf(\"\\n\");");
            } else {
                char formato[512] = "";
                char args_c[2048] = "";
                int tem_erro = 0;

                for (int i = 0; i < $3->count; i++) {
                    Tipo t = $3->tipos[i];

                    if ($3->is_ref[i]) {
                        erro_semantico(yylineno, "'print' nao aceita argumento por referencia ('ref') no argumento %d.", i + 1);
                        tem_erro = 1;
                        break;
                    }
                    if (t.base == TIPO_ERRO) {
                        tem_erro = 1;
                        break;
                    }
                    if (t.dimensao > 0) {
                        erro_semantico(yylineno, "Nao e possivel imprimir um array inteiro diretamente no argumento %d de 'print'.", i + 1);
                        tem_erro = 1;
                        break;
                    }
                    if (t.base == TIPO_VOID) {
                        erro_semantico(yylineno, "Argumento %d de 'print' nao possui valor (tipo 'void').", i + 1);
                        tem_erro = 1;
                        break;
                    }
                    if (t.base == TIPO_STRUCT) {
                        erro_semantico(yylineno, "Nao e possivel imprimir uma struct ('%s') diretamente no argumento %d de 'print'.",
                                       t.nome_struct, i + 1);
                        tem_erro = 1;
                        break;
                    }

                    if (i > 0) {
                        strcat(formato, " ");
                        strcat(args_c, ", ");
                    }

                    if (t.base == TIPO_REAL) {
                        strcat(formato, "%g");
                        strcat(args_c, $3->c_itens[i]);
                    } else if (t.base == TIPO_STRING) {
                        strcat(formato, "%s");
                        strcat(args_c, $3->c_itens[i]);
                    } else if (t.base == TIPO_BOOL) {
                        strcat(formato, "%s");
                        char *b_expr = fmt("(%s ? \"true\" : \"false\")", $3->c_itens[i]);
                        strcat(args_c, b_expr);
                        free(b_expr);
                    } else {
                        strcat(formato, "%d");
                        strcat(args_c, $3->c_itens[i]);
                    }
                }

                if (!tem_erro) {
                    emit_c("printf(\"%s\\n\", %s);", formato, args_c);
                }
            }
            free_lista_args($3);
        }
    ;

declaracaovar
    : tipo ID ASSIGN READ LPAREN RPAREN
        {
            pr("[decl] %s %s <- read()", $1, $2);
            Tipo t_var = parse_tipo_string($1);
            if (inserir_variavel($2, t_var, yylineno) != NULL) {
                if (t_var.dimensao > 0 || t_var.base == TIPO_STRUCT) {
                    erro_semantico(yylineno, "Nao e possivel usar 'read()' diretamente em '%s'.", $2);
                } else {
                    char *t_c = tipo_para_c(t_var);
                    int l_ok = novo_label();
                    if (t_var.base == TIPO_INT) {
                        emit_c("%s %s;", t_c, $2);
                        emit_c("if( scanf(\"%%d\", &%s) == 1 ) goto L%d;", $2, l_ok);
                        emit_c("fprintf(stderr, \"Erro em tempo de execucao: esperado 'int'.\\n\");");
                        emit_c("exit(1);");
                        emit_c("L%d:;", l_ok);
                    } else if (t_var.base == TIPO_REAL) {
                        emit_c("%s %s;", t_c, $2);
                        emit_c("if( scanf(\"%%lf\", &%s) == 1 ) goto L%d;", $2, l_ok);
                        emit_c("fprintf(stderr, \"Erro em tempo de execucao: esperado 'real'.\\n\");");
                        emit_c("exit(1);");
                        emit_c("L%d:;", l_ok);
                    } else if (t_var.base == TIPO_STRING) {
                        emit_c("char *%s = malloc(256);", $2);
                        emit_c("if( scanf(\"%%255s\", %s) == 1 ) goto L%d;", $2, l_ok);
                        emit_c("fprintf(stderr, \"Erro em tempo de execucao: esperado 'string'.\\n\");");
                        emit_c("exit(1);");
                        emit_c("L%d:;", l_ok);
                    } else if (t_var.base == TIPO_BOOL) {
                        emit_c("%s %s;", t_c, $2);
                        emit_c("{ char _b[16]; if (scanf(\"%%15s\", _b) == 1) {");
                        emit_c("    if (strcmp(_b, \"true\") == 0 || strcmp(_b, \"1\") == 0) { %s = true; goto L%d; }", $2, l_ok);
                        emit_c("    if (strcmp(_b, \"false\") == 0 || strcmp(_b, \"0\") == 0) { %s = false; goto L%d; }", $2, l_ok);
                        emit_c("} }");
                        emit_c("fprintf(stderr, \"Erro em tempo de execucao: esperado 'bool'.\\n\");");
                        emit_c("exit(1);");
                        emit_c("L%d:;", l_ok);
                    }
                    free(t_c);
                }
            }
            free($1); free($2);
        }
    | tipo ID ASSIGN expressao
        {
            pr("[decl] %s %s <- %s", $1, $2, $4.texto);
            Tipo t_var = parse_tipo_string($1);
            if (inserir_variavel($2, t_var, yylineno) != NULL) {
                if (!tipos_compativeis(t_var, $4.tipo)) {
                    erro_semantico(yylineno, "Nao e possivel inicializar '%s' (%s) com tipo '%s'.",
                                   $2, tipo_para_string(t_var), tipo_para_string($4.tipo));
                } else {
                    char *t_c = tipo_para_c(t_var);
                    if (t_var.dimensao > 0 && $4.texto[0] == '[') {
                        Tipo t_elem = criar_tipo(t_var.base, t_var.dimensao - 1);
                        if (t_var.base == TIPO_STRUCT)
                            strcpy(t_elem.nome_struct, t_var.nome_struct);
                        char *elem_c = tipo_para_c(t_elem);
                        char *init = strdup($4.texto);
                        init[0] = '{';
                        init[strlen(init) - 1] = '}';
                        /* Aloca no heap para permitir realloc futuro com append() */
                        emit_c("%s _tmp_%s[] = %s;", elem_c, $2, init);
                        emit_c("int _len_%s = sizeof(_tmp_%s) / sizeof(%s);", $2, $2, elem_c);
                        emit_c("%s %s = malloc(sizeof(_tmp_%s));", t_c, $2, $2);
                        emit_c("memcpy(%s, _tmp_%s, sizeof(_tmp_%s));", $2, $2, $2);
                        free(elem_c); free(init);
                    } else if (t_var.dimensao > 0) {
                        emit_c("%s %s = %s;", t_c, $2, $4.texto);
                        if (strchr($4.texto, '(') == NULL && strchr($4.texto, '[') == NULL) {
                            emit_c("int _len_%s = _len_%s;", $2, $4.texto);
                        } else {
                            emit_c("int _len_%s = 0;", $2);
                        }
                    } else if ($4.is_ref) {
                        emit_c("%s %s = *(%s);", t_c, $2, $4.texto);
                    } else {
                        emit_c("%s %s = %s;", t_c, $2, $4.texto);
                    }
                    free(t_c);
                }
            }
            free($1); free($2); free($4.texto);
        }
    | tipo ID
        {
            pr("[decl] %s %s", $1, $2);
            Tipo t_var = parse_tipo_string($1);
            if (inserir_variavel($2, t_var, yylineno) != NULL) {
                char *t_c = tipo_para_c(t_var);
                if (t_var.dimensao > 0) {
                    emit_c("%s %s = NULL;", t_c, $2);
                    emit_c("int _len_%s = 0;", $2);
                } else {
                    emit_c("%s %s;", t_c, $2);
                }
                free(t_c);
            }
            free($1); free($2);
        }
    | REF tipo ID ASSIGN REF l_value
        {
            pr("[decl] ref %s %s <- ref %s", $2, $3, $6.texto);
            Tipo t_var = parse_tipo_string($2);
            if (inserir_variavel_ref($3, t_var, 2, yylineno) != NULL) {
                if (!tipos_identicos(t_var, $6.tipo)) {
                    erro_semantico(yylineno, "Referencia '%s' (%s) incompativel com '%s'.",
                                   $3, tipo_para_string(t_var), tipo_para_string($6.tipo));
                } else {
                    char *t_c = tipo_para_c(t_var);
                    if ($6.is_ref) emit_c("%s *%s = %s;", t_c, $3, $6.texto);
                    else           emit_c("%s *%s = &(%s);", t_c, $3, $6.texto);
                    free(t_c);
                }
            }
            free($2); free($3); free($6.texto);
        }
    | REF tipo ID ASSIGN expressao
        {
            pr("[decl] ref %s %s <- %s", $2, $3, $5.texto);
            Tipo t_var = parse_tipo_string($2);
            if (inserir_variavel_ref($3, t_var, 2, yylineno) != NULL) {
                if ($5.tipo.base == TIPO_NULL) {
                    char *t_c = tipo_para_c(t_var);
                    emit_c("%s *%s = NULL;", t_c, $3);
                    free(t_c);
                } else if (!$5.is_ref || !tipos_identicos(t_var, $5.tipo)) {
                    erro_semantico(yylineno, "Referencia '%s' (%s) exige 'ref' ou 'null'.",
                                   $3, tipo_para_string(t_var));
                } else {
                    char *t_c = tipo_para_c(t_var);
                    emit_c("%s *%s = %s;", t_c, $3, $5.texto);
                    free(t_c);
                }
            }
            free($2); free($3); free($5.texto);
        }
    | REF tipo ID
        {
            pr("[decl] ref %s %s", $2, $3);
            Tipo t_var = parse_tipo_string($2);
            if (inserir_variavel_ref($3, t_var, 2, yylineno) != NULL) {
                char *t_c = tipo_para_c(t_var);
                emit_c("%s *%s = NULL;", t_c, $3);
                free(t_c);
            }
            free($2); free($3);
        }
    ;

atribuicao
    : l_value ASSIGN READ LPAREN RPAREN
        {
            pr("[atrib] %s <- read()", $1.texto);
            if ($1.tipo.base != TIPO_ERRO) {
                if ($1.tipo.dimensao > 0 || $1.tipo.base == TIPO_STRUCT) {
                    erro_semantico(yylineno, "Nao e possivel usar 'read()' diretamente em '%s'.", $1.texto);
                } else if ($1.tipo.base == TIPO_INT) {
                    int l_ok = novo_label();
                    emit_c("if( scanf(\"%%d\", &%s) == 1 ) goto L%d;", $1.texto, l_ok);
                    emit_c("fprintf(stderr, \"Erro em tempo de execucao: esperado 'int'.\\n\");");
                    emit_c("exit(1);");
                    emit_c("L%d:;", l_ok);
                } else if ($1.tipo.base == TIPO_REAL) {
                    int l_ok = novo_label();
                    emit_c("if( scanf(\"%%lf\", &%s) == 1 ) goto L%d;", $1.texto, l_ok);
                    emit_c("fprintf(stderr, \"Erro em tempo de execucao: esperado 'real'.\\n\");");
                    emit_c("exit(1);");
                    emit_c("L%d:;", l_ok);
                } else if ($1.tipo.base == TIPO_STRING) {
                    int l_ok = novo_label();
                    emit_c("%s = malloc(256);", $1.texto);
                    emit_c("if( scanf(\"%%255s\", %s) == 1 ) goto L%d;", $1.texto, l_ok);
                    emit_c("fprintf(stderr, \"Erro em tempo de execucao: esperado 'string'.\\n\");");
                    emit_c("exit(1);");
                    emit_c("L%d:;", l_ok);
                } else if ($1.tipo.base == TIPO_BOOL) {
                    int l_ok = novo_label();
                    emit_c("{ char _b[16]; if (scanf(\"%%15s\", _b) == 1) {");
                    emit_c("    if (strcmp(_b, \"true\") == 0 || strcmp(_b, \"1\") == 0) { %s = true; goto L%d; }", $1.texto, l_ok);
                    emit_c("    if (strcmp(_b, \"false\") == 0 || strcmp(_b, \"0\") == 0) { %s = false; goto L%d; }", $1.texto, l_ok);
                    emit_c("} }");
                    emit_c("fprintf(stderr, \"Erro em tempo de execucao: esperado 'bool'.\\n\");");
                    emit_c("exit(1);");
                    emit_c("L%d:;", l_ok);
                }
            }
            free($1.texto);
        }
    | l_value ASSIGN REF l_value
        {
            pr("[atrib] %s <- ref %s", $1.texto, $4.texto);
            if ($1.tipo.base != TIPO_ERRO && $4.tipo.base != TIPO_ERRO) {
                if (!$1.is_ref) {
                    erro_semantico(yylineno, "'%s' nao e uma referencia ('ref') e nao pode receber 'ref %s'.",
                                   $1.texto, $4.texto);
                } else if (!tipos_identicos($1.tipo, $4.tipo)) {
                    erro_semantico(yylineno, "Referencia incompativel: '%s' exige '%s', recebeu '%s'.",
                                   $1.texto, tipo_para_string($1.tipo), tipo_para_string($4.tipo));
                } else if ($4.is_ref) {
                    emit_c("%s = %s;", $1.texto, $4.texto);
                } else {
                    emit_c("%s = &(%s);", $1.texto, $4.texto);
                }
            }
            free($1.texto); free($4.texto);
        }
    | l_value ASSIGN expressao
        {
            pr("[atrib] %s <- %s", $1.texto, $3.texto);
            if ($1.tipo.base != TIPO_ERRO && $3.tipo.base != TIPO_ERRO) {
                if ($1.is_ref) {
                    if ($3.tipo.base == TIPO_NULL) {
                        emit_c("%s = NULL;", $1.texto);
                    } else if ($3.is_ref && tipos_identicos($1.tipo, $3.tipo)) {
                        emit_c("%s = %s;", $1.texto, $3.texto);
                    } else {
                        erro_semantico(yylineno, "Atribuicao invalida para referencia '%s': use 'ref' ou 'null'.",
                                       $1.texto);
                    }
                } else if (!tipos_compativeis($1.tipo, $3.tipo)) {
                    erro_semantico(yylineno, "Atribuicao incompativel: '%s' espera '%s', mas recebeu '%s'.",
                                   $1.texto, tipo_para_string($1.tipo), tipo_para_string($3.tipo));
                } else if ($3.is_ref) {
                    emit_c("%s = *(%s);", $1.texto, $3.texto);
                } else {
                    emit_c("%s = %s;", $1.texto, $3.texto);
                }
            }
            free($1.texto); free($3.texto);
        }
    ;

comandowhile
    : WHILE LPAREN expressao RPAREN
        {
            char *label_inicio = new_label();
            char *label_fim = new_label();

            pr("[while] while (%s)", $3.texto);

            /* Valida a condicao do while */
            if ($3.tipo.base != TIPO_ERRO &&
                ($3.tipo.base != TIPO_BOOL || $3.tipo.dimensao != 0)) {
                erro_semantico(yylineno,
                    "Condicao do 'while' deve ser booleana ('bool'), recebeu '%s'.",
                    tipo_para_string($3.tipo));
            }

            /* Armazena as labels na pilha */
            push_loop(label_inicio, label_fim);

            /* Gera a label de inicio */
            emit_c("%s:", label_inicio);

            /* Verifica a condicao e permite sair do laco */
            emit_c("if( !(%s) ) goto %s;", $3.texto, label_fim);

            /* Abre um bloco para preservar o escopo */
            emit_c("{");

            depth++;
            loop_depth++;

            entrar_escopo();

            free($3.texto);
        }
      NEWLINE corpo_bloco ENDWHILE
        {
            sair_escopo();

            depth--;
            loop_depth--;

            /* Fecha o bloco do laco */
            emit_c("}");

            /* Retorna ao inicio do laco */
            emit_c("goto %s;", loop_stack[loop_top].inicio);

            /* Gera a label de saida */
            emit_c("%s: ;", loop_stack[loop_top].fim);

            /* Remove as labels da pilha */
            pop_loop();

            pr("[endwhile]");
        }
    ;

comandoif
    : IF LPAREN expressao RPAREN
        {
            char *label_else = new_label();
            char *label_fim  = new_label();

            pr("[if] if (%s)", $3.texto);

            /* Valida a condicao */
            if ($3.tipo.base != TIPO_ERRO &&
                ($3.tipo.base != TIPO_BOOL || $3.tipo.dimensao != 0)) {
                erro_semantico(yylineno,
                    "Condicao do 'if' deve ser booleana ('bool'), recebeu '%s'.",
                    tipo_para_string($3.tipo));
            }

            /* Armazena as labels na pilha */
            push_if(label_else, label_fim);

            /* Desvia para o else (ou para o fim, se nao houver else) quando falso */
            emit_c("if( !(%s) ) goto %s;", $3.texto, label_else);

            /* Abre um bloco para preservar o escopo do ramo 'if' */
            emit_c("{");

            depth++;
            entrar_escopo();

            free($3.texto);
        }
      NEWLINE corpo_bloco cauda_if
    ;

cauda_if
    : ENDIF
        {
            sair_escopo();
            depth--;

            /* Fecha o bloco do ramo 'if' */
            emit_c("}");

            /* Sem 'else', label_else atua como ponto de saida */
            emit_c("%s: ;", if_stack[if_top].label_else);

            pop_if();
            pr("[endif]");
        }
    | ELSE
        {
            sair_escopo();
            depth--;

            /* Fecha o bloco do ramo 'if' e salta sobre o 'else' */
            emit_c("}");
            emit_c("goto %s;", if_stack[if_top].label_fim);
            emit_c("%s: ;", if_stack[if_top].label_else);

            pr("[else]");

            /* Abre um novo bloco e escopo para o ramo 'else' */
            emit_c("{");
            depth++;
            entrar_escopo();
        }
      NEWLINE corpo_bloco ENDIF
        {
            sair_escopo();
            depth--;

            /* Fecha o bloco do ramo 'else' e emite a label final */
            emit_c("}");
            emit_c("%s: ;", if_stack[if_top].label_fim);

            pop_if();
            pr("[endif]");
        }
    ;

comandobreak
    : BREAK
        {
            pr("[break]");

            if (loop_depth == 0) {
                erro_semantico(yylineno,
                    "Comando 'break' so pode ser utilizado dentro de um while.");
            } else {
                emit_c("goto %s;", loop_stack[loop_top].fim);
            }
        }
    ;

retorno
    : RETURN expressao
        {
            pr("[return] %s", $2.texto);
            encontrou_return = 1;
            if (retorno_esperado.base == TIPO_VOID) {
                erro_semantico(yylineno, "Subrotina sem tipo de retorno (ou main) nao deve retornar valor.");
            } else if (!tipos_compativeis(retorno_esperado, $2.tipo)) {
                erro_semantico(yylineno, "Tipo de retorno incompativel: esperado '%s', obtido '%s'.",
                               tipo_para_string(retorno_esperado), tipo_para_string($2.tipo));
            } else {
                emit_c("return %s;", $2.texto);
            }
            free($2.texto);
        }
    ;

chamadafuncao
    : ID LPAREN STRUCT_ID RPAREN
        {
            if (strcmp($1, "create") != 0) {
                erro_semantico(yylineno, "Subrotina '%s' nao pode receber o tipo '%s' como argumento.", $1, $3);
                $$.tipo = criar_tipo(TIPO_ERRO, 0);
                $$.texto = strdup("NULL");
                $$.is_ref = 0;
            } else {
                $$.tipo = criar_tipo_struct($3, 0);
                $$.texto = fmt("calloc(1, sizeof(struct %s))", $3);
                $$.is_ref = 1;
            }
            free($1);
            free($3);
        }
    | ID LPAREN argumentos_opt RPAREN
        {
            /* 1. Built-in: comprimento(array) -> int */
            if (strcmp($1, "comprimento") == 0) {
                $$.is_ref = 0;
                if ($3->count != 1) {
                    erro_semantico(yylineno, "'comprimento' espera 1 argumento, recebeu %d.", $3->count);
                    $$.tipo = criar_tipo(TIPO_ERRO, 0);
                    $$.texto = strdup("0");
                } else if ($3->is_ref[0]) {
                    erro_semantico(yylineno, "'comprimento' nao aceita argumento por referencia ('ref').");
                    $$.tipo = criar_tipo(TIPO_ERRO, 0);
                    $$.texto = strdup("0");
                } else if ($3->tipos[0].dimensao <= 0 && $3->tipos[0].base != TIPO_ERRO) {
                    erro_semantico(yylineno, "'comprimento' espera um array, recebeu '%s'.",
                                   tipo_para_string($3->tipos[0]));
                    $$.tipo = criar_tipo(TIPO_ERRO, 0);
                    $$.texto = strdup("0");
                } else {
                    $$.tipo = criar_tipo(TIPO_INT, 0);
                    const char *nome_raw = $3->nomes[0];
                    Simbolo *s_arr = buscar_simbolo(nome_raw);
                    if (s_arr && s_arr->is_ref) {
                        $$.texto = fmt("(*_len_%s)", nome_raw);
                    } else {
                        $$.texto = fmt("_len_%s", nome_raw);
                    }
                }
            }
            /* 2. Built-in: swap(a, b) -> void */
            else if (strcmp($1, "swap") == 0) {
                $$.is_ref = 0;
                if ($3->count != 2) {
                    erro_semantico(yylineno, "'swap' espera 2 argumentos, recebeu %d.", $3->count);
                } else if (!tipos_identicos($3->tipos[0], $3->tipos[1])) {
                    erro_semantico(yylineno, "'swap' requer argumentos exatamente do mesmo tipo, recebeu '%s' e '%s'.",
                                   tipo_para_string($3->tipos[0]), tipo_para_string($3->tipos[1]));
                }
                $$.tipo = criar_tipo(TIPO_VOID, 0);
                $$.texto = fmt("%s(%s)", $1, $3->c_texto);
            }
            /* 3. Built-in: append(ref array, elemento) -> void */
            else if (strcmp($1, "append") == 0) {
                $$.is_ref = 0;
                $$.tipo = criar_tipo(TIPO_VOID, 0);
                if ($3->count != 2) {
                    erro_semantico(yylineno, "'append' espera 2 argumentos (ref array, elemento), recebeu %d.", $3->count);
                    $$.texto = strdup("");
                } else if (!$3->is_ref[0]) {
                    erro_semantico(yylineno, "O primeiro argumento de 'append' deve ser passado por referencia ('ref').");
                    $$.texto = strdup("");
                } else if ($3->tipos[0].base != TIPO_ERRO && $3->tipos[0].dimensao <= 0) {
                    erro_semantico(yylineno, "O primeiro argumento de 'append' deve ser um array, recebeu '%s'.",
                                   tipo_para_string($3->tipos[0]));
                    $$.texto = strdup("");
                } else if ($3->is_ref[1]) {
                    erro_semantico(yylineno, "O segundo argumento de 'append' nao pode ser passado por referencia ('ref').");
                    $$.texto = strdup("");
                } else {
                    Tipo elem_esperado = criar_tipo($3->tipos[0].base, $3->tipos[0].dimensao - 1);
                    if ($3->tipos[0].base == TIPO_STRUCT) {
                        strcpy(elem_esperado.nome_struct, $3->tipos[0].nome_struct);
                    }
                    if (!tipos_compativeis(elem_esperado, $3->tipos[1])) {
                        erro_semantico(yylineno, "Elemento incompativel em 'append': esperado '%s', recebeu '%s'.",
                                       tipo_para_string(elem_esperado), tipo_para_string($3->tipos[1]));
                        $$.texto = strdup("");
                    } else if ($3->tipos[0].base != TIPO_ERRO) {
                        const char *nome_raw = $3->nomes[0];
                        Simbolo *s_arr = buscar_simbolo(nome_raw);
                        if (s_arr && s_arr->is_ref) {
                            $$.texto = fmt("do { (*%s) = realloc((*%s), ((*_len_%s) + 1) * sizeof(*(*%s))); (*%s)[(*_len_%s)++] = %s; } while(0)",
                                           nome_raw, nome_raw, nome_raw, nome_raw, nome_raw, nome_raw, $3->c_itens[1]);
                        } else {
                            $$.texto = fmt("do { %s = realloc(%s, (_len_%s + 1) * sizeof(*%s)); %s[_len_%s++] = %s; } while(0)",
                                           nome_raw, nome_raw, nome_raw, nome_raw, nome_raw, nome_raw, $3->c_itens[1]);
                        }
                    } else {
                        $$.texto = strdup("");
                    }
                }
            }
            /* 4. Uso incorreto de create() sem nome de struct */
            else if (strcmp($1, "create") == 0) {
                erro_semantico(yylineno, "'create' espera o nome de uma struct declarada (ex: create(node)).");
                $$.tipo = criar_tipo(TIPO_ERRO, 0);
                $$.texto = strdup("NULL");
                $$.is_ref = 0;
            }
            /* 5. Demais subrotinas definidas pelo usuario */
            else {
                $$.is_ref = 0;
                $$.texto = fmt("%s(%s)", $1, $3->c_texto);
                Simbolo *s = buscar_simbolo($1);
                if (!s) {
                    erro_semantico(yylineno, "Subrotina '%s' nao declarada.", $1);
                    $$.tipo = criar_tipo(TIPO_ERRO, 0);
                } else if (!s->is_func) {
                    erro_semantico(yylineno, "'%s' e uma variavel e nao pode ser chamada como subrotina.", $1);
                    $$.tipo = criar_tipo(TIPO_ERRO, 0);
                } else {
                    if (s->num_params != $3->count) {
                        erro_semantico(yylineno, "Subrotina '%s' espera %d argumento(s), recebeu %d.",
                                       $1, s->num_params, $3->count);
                    } else {
                        for (int i = 0; i < s->num_params; i++) {
                            int param_ref = s->params_ref ? s->params_ref[i] : 0;
                            int arg_ref   = $3->is_ref[i];

                            if (param_ref && !arg_ref) {
                                erro_semantico(yylineno, "Argumento %d de '%s' deve ser passado por referencia ('ref').",
                                               i + 1, $1);
                            } else if (!param_ref && arg_ref) {
                                erro_semantico(yylineno, "Argumento %d de '%s' nao aceita passagem por referencia ('ref').",
                                               i + 1, $1);
                            } else if (param_ref) {
                                /* Ponteiros em C exigem tipo identico (inclusive nome da struct) */
                                if (!tipos_identicos(s->params[i], $3->tipos[i])) {
                                    erro_semantico(yylineno, "Argumento %d por referencia de '%s' exige tipo exato '%s', recebido '%s'.",
                                                   i + 1, $1, tipo_para_string(s->params[i]), tipo_para_string($3->tipos[i]));
                                }
                            } else if (!tipos_compativeis(s->params[i], $3->tipos[i])) {
                                erro_semantico(yylineno, "Argumento %d de '%s' invalido: esperado '%s', recebido '%s'.",
                                               i + 1, $1, tipo_para_string(s->params[i]), tipo_para_string($3->tipos[i]));
                            }
                        }
                    }
                    $$.tipo = s->tipo;
                }
            }
            free($1);
            free_lista_args($3);
        }
    ;

argumentos_opt
    : lista_argumentos { $$ = $1; }
    | /* vazio */
        {
            ListaArgs *la = calloc(1, sizeof(ListaArgs));
            la->count = 0;
            la->texto = strdup("");
            la->c_texto = strdup("");
            $$ = la;
        }
    ;

lista_argumentos
    : lista_argumentos COMMA REF l_value
        {
            if ($1->count < 32) {
                int idx = $1->count++;
                $1->tipos[idx] = $4.tipo;
                $1->is_ref[idx] = 1;
                $1->nomes[idx] = strdup($4.texto);
                $1->c_itens[idx] = $4.is_ref ? strdup($4.texto) : fmt("&(%s)", $4.texto);
                char *novo_t = fmt("%s, ref %s", $1->texto, $4.texto);
                char *novo_c = ($4.tipo.dimensao > 0)
                    ? fmt("%s, &(%s), &(_len_%s)", $1->c_texto, $4.texto, $4.texto)
                    : fmt("%s, %s", $1->c_texto, $1->c_itens[idx]);
                free($1->texto); free($1->c_texto);
                $1->texto = novo_t;
                $1->c_texto = novo_c;
            } else {
                erro_semantico(yylineno, "Limite maximo de 32 argumentos/elementos excedido.");
            }
            free($4.texto);
            $$ = $1;
        }
    | REF l_value
        {
            ListaArgs *la = calloc(1, sizeof(ListaArgs));
            la->tipos[0] = $2.tipo;
            la->is_ref[0] = 1;
            la->nomes[0] = strdup($2.texto);
            la->c_itens[0] = $2.is_ref ? strdup($2.texto) : fmt("&(%s)", $2.texto);
            la->count = 1;
            la->texto = fmt("ref %s", $2.texto);
            la->c_texto = ($2.tipo.dimensao > 0)
                ? fmt("&(%s), &(_len_%s)", $2.texto, $2.texto)
                : strdup(la->c_itens[0]);
            free($2.texto);
            $$ = la;
        }
    | lista_argumentos COMMA expressao
        {
            if ($1->count < 32) {
                int idx = $1->count++;
                $1->tipos[idx] = $3.tipo;
                $1->is_ref[idx] = 0;
                $1->nomes[idx] = strdup($3.texto);
                $1->c_itens[idx] = strdup($3.texto);
                char *novo_t = fmt("%s, %s", $1->texto, $3.texto);
                char *novo_c = ($3.tipo.dimensao > 0)
                    ? fmt("%s, %s, _len_%s", $1->c_texto, $3.texto, $3.texto)
                    : fmt("%s, %s", $1->c_texto, $3.texto);
                free($1->texto); free($1->c_texto);
                $1->texto = novo_t;
                $1->c_texto = novo_c;
            } else {
                erro_semantico(yylineno, "Limite maximo de 32 argumentos/elementos excedido.");
            }
            free($3.texto);
            $$ = $1;
        }
    | expressao
        {
            ListaArgs *la = calloc(1, sizeof(ListaArgs));
            la->tipos[0] = $1.tipo;
            la->is_ref[0] = 0;
            la->nomes[0] = strdup($1.texto);
            la->c_itens[0] = strdup($1.texto);
            la->count = 1;
            la->texto = strdup($1.texto);
            la->c_texto = ($1.tipo.dimensao > 0)
                ? fmt("%s, _len_%s", $1.texto, $1.texto)
                : strdup($1.texto);
            free($1.texto);
            $$ = la;
        }
    ;

/* ---------- tipos ---------- */

tipo
    : INT                      { $$ = strdup("int"); }
    | REAL                     { $$ = strdup("real"); }
    | STRING                   { $$ = strdup("string"); }
    | BOOL                     { $$ = strdup("bool"); }
    | STRUCT_ID                { $$ = $1; }
    | tipo LBRACKET RBRACKET   { $$ = fmt("%s[]", $1); free($1); }
    ;

/* ---------- expressoes ---------- */

expressao
    : explogica { $$ = $1; }
    ;

explogica
    : explogica OR exp_and
        {
            $$.texto = fmt("%s || %s", $1.texto, $3.texto);
            $$.tipo = verificar_binario_logico($1.tipo, $3.tipo, "||", yylineno);
            $$.is_ref = 0;
            free($1.texto); free($3.texto);
        }
    | exp_and { $$ = $1; }
    ;

exp_and
    : exp_and AND exp_not
        {
            $$.texto = fmt("%s && %s", $1.texto, $3.texto);
            $$.tipo = verificar_binario_logico($1.tipo, $3.tipo, "&&", yylineno);
            $$.is_ref = 0;
            free($1.texto); free($3.texto);
        }
    | exp_not { $$ = $1; }
    ;

exp_not
    : NOT exp_not
        {
            $$.texto = fmt("!(%s)", $2.texto);
            $$.tipo = verificar_unario_logico($2.tipo, "!", yylineno);
            $$.is_ref = 0;
            free($2.texto);
        }
    | exprelacional { $$ = $1; }
    ;

exprelacional
    : exparitimetica op_relacional exparitimetica
        {
            $$.texto = fmt("%s %s %s", $1.texto, $2, $3.texto);
            $$.is_ref = 0;
            if (($1.is_ref || $3.is_ref) &&
                (strcmp($2, "==") == 0 || strcmp($2, "!=") == 0) &&
                ($1.tipo.base == TIPO_NULL || $3.tipo.base == TIPO_NULL ||
                 ($1.is_ref && $3.is_ref && tipos_identicos($1.tipo, $3.tipo)))) {
                $$.tipo = criar_tipo(TIPO_BOOL, 0);
            } else {
                $$.tipo = verificar_relacional($1.tipo, $3.tipo, $2, yylineno);
            }
            free($1.texto); free($2); free($3.texto);
        }
    | exparitimetica { $$ = $1; }
    ;

op_relacional
    : LT   { $$ = strdup("<");  }
    | LEQ  { $$ = strdup("<="); }
    | GT   { $$ = strdup(">");  }
    | GEQ  { $$ = strdup(">="); }
    | EQ   { $$ = strdup("=="); }
    | NEQ  { $$ = strdup("!="); }
    ;

exparitimetica
    : exparitimetica PLUS termo
        {
            $$.texto = fmt("%s + %s", $1.texto, $3.texto);
            $$.tipo = verificar_binario_aritmetico($1.tipo, $3.tipo, "+", yylineno);
            $$.is_ref = 0;
            free($1.texto); free($3.texto);
        }
    | exparitimetica MINUS termo
        {
            $$.texto = fmt("%s - %s", $1.texto, $3.texto);
            $$.tipo = verificar_binario_aritmetico($1.tipo, $3.tipo, "-", yylineno);
            $$.is_ref = 0;
            free($1.texto); free($3.texto);
        }
    | termo { $$ = $1; }
    ;

termo
    : termo MUL unario
        {
            $$.texto = fmt("%s * %s", $1.texto, $3.texto);
            $$.tipo = verificar_binario_aritmetico($1.tipo, $3.tipo, "*", yylineno);
            $$.is_ref = 0;
            free($1.texto); free($3.texto);
        }
    | termo DIV unario
        {
            $$.texto = fmt("%s / %s", $1.texto, $3.texto);
            $$.tipo = verificar_binario_aritmetico($1.tipo, $3.tipo, "/", yylineno);
            $$.is_ref = 0;
            free($1.texto); free($3.texto);
        }
    | unario { $$ = $1; }
    ;

unario
    : MINUS unario
        {
            $$.tipo = verificar_unario_aritmetico($2.tipo, "-", yylineno);
            $$.texto = fmt("-(%s)", $2.texto);
            $$.is_ref = 0;
            free($2.texto);
        }
    | PLUS unario
        {
            $$.tipo = verificar_unario_aritmetico($2.tipo, "+", yylineno);
            $$.texto = fmt("+(%s)", $2.texto);
            $$.is_ref = 0;
            free($2.texto);
        }
    | potencia { $$ = $1; }
    ;

potencia
    : fator POW unario
        {
            $$.tipo = verificar_binario_aritmetico($1.tipo, $3.tipo, "**", yylineno);
            $$.is_ref = 0;
            if ($$.tipo.base == TIPO_INT) {
                $$.texto = fmt("((int)pow(%s, %s))", $1.texto, $3.texto);
            } else {
                $$.texto = fmt("pow(%s, %s)", $1.texto, $3.texto);
            }
            free($1.texto); free($3.texto);
        }
    | fator { $$ = $1; }
    ;

l_value
    : ID
        {
            Simbolo *s = buscar_simbolo($1);
            if (!s) {
                erro_semantico(yylineno, "Variavel '%s' nao declarada.", $1);
                $$.texto = $1;
                $$.tipo = criar_tipo(TIPO_ERRO, 0);
                $$.is_ref = 0;
            } else if (s->is_func) {
                erro_semantico(yylineno, "'%s' e uma subrotina e nao pode ser usada como variavel.", $1);
                $$.texto = $1;
                $$.tipo = criar_tipo(TIPO_ERRO, 0);
                $$.is_ref = 0;
            } else {
                $$.tipo = s->tipo;
                if (s->is_ref == 1) {
                    if (s->tipo.base == TIPO_STRUCT && s->tipo.dimensao == 0) {
                        /* Parametro 'ref' de struct ja e ponteiro (struct T*) */
                        $$.texto = $1;
                        $$.is_ref = 1;
                    } else {
                        /* Parametro 'ref' escalar/array: desreferencia automaticamente */
                        $$.texto = fmt("(*%s)", $1);
                        $$.is_ref = 0;
                        free($1);
                    }
                } else if (s->is_ref == 2) {
                    /* Variavel local declarada com 'ref tipo id' */
                    $$.texto = $1;
                    $$.is_ref = 1;
                } else {
                    $$.texto = $1;
                    $$.is_ref = 0;
                }
            }
        }
    | l_value LBRACKET expressao RBRACKET
        {
            if ($1.is_ref) $$.texto = fmt("(%s)[%s]", $1.texto, $3.texto);
            else           $$.texto = fmt("%s[%s]", $1.texto, $3.texto);
            $$.is_ref = 0;
            if ($3.tipo.base != TIPO_ERRO && ($3.tipo.base != TIPO_INT || $3.tipo.dimensao != 0)) {
                erro_semantico(yylineno, "Indice de array deve ser 'int', recebeu '%s'.",
                               tipo_para_string($3.tipo));
            }
            if ($1.tipo.base != TIPO_ERRO) {
                if ($1.tipo.dimensao <= 0) {
                    erro_semantico(yylineno, "Tentativa de indexar tipo nao-array '%s'.",
                                   tipo_para_string($1.tipo));
                    $$.tipo = criar_tipo(TIPO_ERRO, 0);
                } else if ($1.tipo.base == TIPO_STRUCT) {
                    $$.tipo = criar_tipo_struct($1.tipo.nome_struct, $1.tipo.dimensao - 1);
                } else {
                    $$.tipo = criar_tipo($1.tipo.base, $1.tipo.dimensao - 1);
                }
            } else {
                $$.tipo = criar_tipo(TIPO_ERRO, 0);
            }
            free($1.texto); free($3.texto);
        }
    | l_value DOT ID
        {
            if ($1.is_ref) $$.texto = fmt("(%s)->%s", $1.texto, $3);
            else           $$.texto = fmt("%s.%s", $1.texto, $3);

            if ($1.tipo.base == TIPO_ERRO) {
                $$.tipo = criar_tipo(TIPO_ERRO, 0);
                $$.is_ref = 0;
            } else if ($1.tipo.base != TIPO_STRUCT || $1.tipo.dimensao != 0) {
                erro_semantico(yylineno, "Acesso ao campo '%s' em '%s', que nao e uma struct.",
                               $3, tipo_para_string($1.tipo));
                $$.tipo = criar_tipo(TIPO_ERRO, 0);
                $$.is_ref = 0;
            } else {
                CampoStruct *cf = buscar_campo_struct_info($1.tipo.nome_struct, $3);
                if (!cf) {
                    erro_semantico(yylineno, "Struct '%s' nao possui o campo '%s'.",
                                   $1.tipo.nome_struct, $3);
                    $$.tipo = criar_tipo(TIPO_ERRO, 0);
                    $$.is_ref = 0;
                } else {
                    $$.tipo = cf->tipo;
                    $$.is_ref = cf->is_ref;
                }
            }
            free($1.texto); free($3);
        }
    ;

fator
    : chamadafuncao           { $$ = $1; }
    | l_value                 { $$ = $1; }
    | INT_NUM                 { $$.texto = fmt("%d", $1); $$.tipo = criar_tipo(TIPO_INT, 0); $$.is_ref = 0; }
    | REAL_NUM                { $$.texto = fmt("%g", $1); $$.tipo = criar_tipo(TIPO_REAL, 0); $$.is_ref = 0; }
    | STRING_LITERAL          { $$.texto = $1; $$.tipo = criar_tipo(TIPO_STRING, 0); $$.is_ref = 0; }
    | TRUE                    { $$.texto = strdup("true"); $$.tipo = criar_tipo(TIPO_BOOL, 0); $$.is_ref = 0; }
    | FALSE                   { $$.texto = strdup("false"); $$.tipo = criar_tipo(TIPO_BOOL, 0); $$.is_ref = 0; }
    | NULL_VAL                { $$.texto = strdup("NULL"); $$.tipo = criar_tipo(TIPO_NULL, 0); $$.is_ref = 1; }
    | arrayLiteral            { $$ = $1; $$.is_ref = 0; }
    | LPAREN expressao RPAREN { $$.texto = fmt("(%s)", $2.texto); $$.tipo = $2.tipo; $$.is_ref = $2.is_ref; free($2.texto); }
    ;

arrayLiteral
    : LBRACKET array_elements_opt RBRACKET
        {
            $$.texto = fmt("[%s]", $2->c_texto);
            if ($2->count == 0) {
                erro_semantico(yylineno, "Array literal vazio nao permitido sem inferencia de tipo.");
                $$.tipo = criar_tipo(TIPO_ERRO, 0);
            } else {
                Tipo elem_tipo = $2->tipos[0];
                int erro_elem = 0;
                for (int i = 0; i < $2->count; i++) {
                    if ($2->is_ref[i]) {
                        erro_semantico(yylineno, "Elementos de array literal nao podem usar 'ref'.");
                        erro_elem = 1;
                        break;
                    }
                    if (i == 0) continue;
                    if (tipos_compativeis(elem_tipo, $2->tipos[i])) {
                        continue;
                    } else if (tipos_compativeis($2->tipos[i], elem_tipo)) {
                        /* Promove int[] para real[] se houver um real no meio */
                        elem_tipo = $2->tipos[i];
                    } else {
                        erro_semantico(yylineno, "Elementos incompativeis no array literal: '%s' e '%s'.",
                                       tipo_para_string(elem_tipo), tipo_para_string($2->tipos[i]));
                        erro_elem = 1;
                        break;
                    }
                }
                if (erro_elem || elem_tipo.base == TIPO_ERRO) {
                    $$.tipo = criar_tipo(TIPO_ERRO, 0);
                } else if (elem_tipo.base == TIPO_STRUCT) {
                    $$.tipo = criar_tipo_struct(elem_tipo.nome_struct, elem_tipo.dimensao + 1);
                } else {
                    $$.tipo = criar_tipo(elem_tipo.base, elem_tipo.dimensao + 1);
                }
            }
            free_lista_args($2);
        }
    ;

array_elements_opt
    : lista_argumentos { $$ = $1; }
    | /* vazio */
        {
            ListaArgs *la = calloc(1, sizeof(ListaArgs));
            la->count = 0;
            la->texto = strdup("");
            la->c_texto = strdup("");
            $$ = la;
        }
    ;

%%

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo.tpyc>\n", argv[0]);
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror("Erro ao abrir arquivo de entrada");
        return 1;
    }

    /* Extrai o nome base do arquivo (ignora diretorios e remove a extensao) */
    const char *barra = strrchr(argv[1], '/');
    const char *inicio = barra ? barra + 1 : argv[1];

    char nome_base[256];
    strncpy(nome_base, inicio, sizeof(nome_base) - 1);
    nome_base[sizeof(nome_base) - 1] = '\0';

    char *ponto = strrchr(nome_base, '.');
    if (ponto) {
        *ponto = '\0';
    }

    char nome_c[300];
    snprintf(nome_c, sizeof(nome_c), "%s.c", nome_base);

    iniciar_semantico();
    out_c = fopen(nome_c, "w");
    if (!out_c) {
        perror("Erro ao criar arquivo C de saida");
        fclose(yyin);
        return 1;
    }

    fprintf(out_c, "#include <stdio.h>\n#include <stdlib.h>\n#include <stdbool.h>\n#include <string.h>\n#include <math.h>\n\n");
    int res = yyparse();
    fclose(out_c);
    fclose(yyin);

    if (res != 0 || obter_erros_semanticos() > 0) {
        remove(nome_c); /* Remove o arquivo C se houve erro */
        return 1;
    }

    /* Compila <nome_base>.c para o executavel ./<nome_base> */
    char cmd[700];
    snprintf(cmd, sizeof(cmd), "gcc \"%s\" -o \"%s\" -lm", nome_c, nome_base);
    int gcc_res = system(cmd);
    if (gcc_res != 0) {
        fprintf(stderr, "Erro ao compilar %s com o gcc.\n", nome_c);
        return 1;
    }

    printf("Executavel './%s' gerado com sucesso.\n", nome_base);
    return 0;
}

int yyerror(char *msg) {
    fprintf(stderr, "Linha %d: %s at '%s'\n", yylineno, msg, yytext);
    return 0;
}