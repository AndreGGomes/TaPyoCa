%code requires {
    #include "semantico.h"

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

int yylex(void);
int yyerror(char *s);
extern int yylineno;
extern char *yytext;

/* ---------- utilitarios de impressao ---------- */

static int depth = 0;   /* nivel de aninhamento atual (4 espacos por tab) */

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
   (ex: int[] -> int*, string -> char*, Aluno -> struct Aluno) */
static char *tipo_para_c(Tipo t) {
    char base[96];
    switch (t.base) {
        case TIPO_VOID:   strcpy(base, "void"); break;
        case TIPO_REAL:   strcpy(base, "double"); break;
        case TIPO_STRING: strcpy(base, "char*"); break;
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

%union {
    int ival;
    double fval;
    char *sval;
    ExprInfo expr;
    ListaParams *plist;
    ListaArgs *alist;
}

%token MAIN LPAREN RPAREN NEWLINE INDENT DEDENT ENDMAIN SUB ENDSUB COMMA
%token ASSIGN RETURN IF ENDIF WHILE ENDWHILE INT LBRACKET RBRACKET STRING REAL
%token LT LEQ GT GEQ EQ NEQ PLUS MINUS MUL DIV PRINT READ REF
%token STRUCT ENDSTRUCT DOT

%token <ival> INT_NUM
%token <fval> REAL_NUM
%token <sval> ID
%token <sval> STRUCT_ID
%token <sval> STRING_LITERAL

/* Nao-terminais tipados */
%type <sval> tipo tipo_opt op_relacional
%type <plist> parametros_opt lista_parametros
%type <alist> argumentos_opt lista_argumentos array_elements_opt
%type <expr> expressao exprelacional exparitimetica termo
%type <expr> l_value fator arrayLiteral chamadafuncao

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
    : tipo ID
        {
            pr("[campo] %s %s", $1, $2);
            Tipo t = parse_tipo_string($1);
            adicionar_campo_struct(struct_atual, $2, t, yylineno);
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
                char *novo_c = fmt("%s, %s *%s", $$->c_texto, t_c, $$->nomes[idx]);
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
            lp->c_texto = fmt("%s *%s", t_c, $3);
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
                char *novo_c = fmt("%s, %s %s", $$->c_texto, t_c, $$->nomes[idx]);
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
            lp->c_texto = fmt("%s %s", t_c, $2);
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

                    /* Coloca um espaco entre os argumentos (estilo Python).
                       Se quiser colado, basta remover este bloco if. */
                    if (i > 0) {
                        strcat(formato, " ");
                    }

                    if (t.base == TIPO_REAL)        strcat(formato, "%g");
                    else if (t.base == TIPO_STRING) strcat(formato, "%s");
                    else                            strcat(formato, "%d");
                }

                if (!tem_erro) {
                    emit_c("printf(\"%s\\n\", %s);", formato, $3->c_texto);
                }
            }
            free($3->texto);
            free($3->c_texto);
            free($3);
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
                    if (t_var.base == TIPO_INT) {
                        emit_c("%s %s;", t_c, $2);
                        emit_c("if (scanf(\"%%d\", &%s) != 1) { fprintf(stderr, \"Erro em tempo de execucao: esperado 'int'.\\n\"); exit(1); }", $2);
                    } else if (t_var.base == TIPO_REAL) {
                        emit_c("%s %s;", t_c, $2);
                        emit_c("if (scanf(\"%%lf\", &%s) != 1) { fprintf(stderr, \"Erro em tempo de execucao: esperado 'real'.\\n\"); exit(1); }", $2);
                    } else if (t_var.base == TIPO_STRING) {
                        emit_c("char *%s = malloc(256);", $2);
                        emit_c("if (scanf(\"%%255s\", %s) != 1) { fprintf(stderr, \"Erro em tempo de execucao: esperado 'string'.\\n\"); exit(1); }", $2);
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
                        emit_c("%s %s[] = %s;", elem_c, $2, init);
                        free(elem_c); free(init);
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
                emit_c("%s %s;", t_c, $2);
                free(t_c);
            }
            free($1); free($2);
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
                    emit_c("if (scanf(\"%%d\", &%s) != 1) { fprintf(stderr, \"Erro em tempo de execucao: esperado 'int'.\\n\"); exit(1); }", $1.texto);
                } else if ($1.tipo.base == TIPO_REAL) {
                    emit_c("if (scanf(\"%%lf\", &%s) != 1) { fprintf(stderr, \"Erro em tempo de execucao: esperado 'real'.\\n\"); exit(1); }", $1.texto);
                } else if ($1.tipo.base == TIPO_STRING) {
                    emit_c("%s = malloc(256);", $1.texto);
                    emit_c("if (scanf(\"%%255s\", %s) != 1) { fprintf(stderr, \"Erro em tempo de execucao: esperado 'string'.\\n\"); exit(1); }", $1.texto);
                }
            }
            free($1.texto);
        }
    | l_value ASSIGN expressao
        {
            pr("[atrib] %s <- %s", $1.texto, $3.texto);
            if (!tipos_compativeis($1.tipo, $3.tipo)) {
                erro_semantico(yylineno, "Atribuicao incompativel: '%s' espera '%s', mas recebeu '%s'.",
                               $1.texto, tipo_para_string($1.tipo), tipo_para_string($3.tipo));
            } else {
                emit_c("%s = %s;", $1.texto, $3.texto);
            }
            free($1.texto); free($3.texto);
        }
    ;

comandowhile
    : WHILE LPAREN expressao RPAREN
        {
            pr("[while] while (%s)", $3.texto);
            depth++;
            if ($3.tipo.base != TIPO_ERRO && ($3.tipo.base != TIPO_INT || $3.tipo.dimensao != 0)) {
                erro_semantico(yylineno, "Condicao do 'while' deve ser logica/inteira, recebeu '%s'.",
                               tipo_para_string($3.tipo));
            }
            entrar_escopo();
            free($3.texto);
        }
      NEWLINE corpo_bloco ENDWHILE
        {
            sair_escopo();
            depth--;
            pr("[endwhile]");
        }
    ;

comandoif
    : IF LPAREN expressao RPAREN
        {
            pr("[if] if (%s)", $3.texto);
            depth++;
            if ($3.tipo.base != TIPO_ERRO && ($3.tipo.base != TIPO_INT || $3.tipo.dimensao != 0)) {
                erro_semantico(yylineno, "Condicao do 'if' deve ser logica/inteira, recebeu '%s'.",
                               tipo_para_string($3.tipo));
            }
            entrar_escopo();
            free($3.texto);
        }
      NEWLINE corpo_bloco ENDIF
        {
            sair_escopo();
            depth--;
            pr("[endif]");
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
            }
            free($2.texto);
        }
    ;

chamadafuncao
    : ID LPAREN argumentos_opt RPAREN
        {
            /* Emite a chamada usando c_texto (que contem '&(...)' nos argumentos ref) */
            $$.texto = fmt("%s(%s)", $1, $3->c_texto);

            /* 1. Built-in: comprimento(array) -> int */
            if (strcmp($1, "comprimento") == 0) {
                if ($3->count != 1) {
                    erro_semantico(yylineno, "'comprimento' espera 1 argumento, recebeu %d.", $3->count);
                    $$.tipo = criar_tipo(TIPO_ERRO, 0);
                } else if ($3->is_ref[0]) {
                    erro_semantico(yylineno, "'comprimento' nao aceita argumento por referencia ('ref').");
                    $$.tipo = criar_tipo(TIPO_ERRO, 0);
                } else if ($3->tipos[0].dimensao <= 0 && $3->tipos[0].base != TIPO_ERRO) {
                    erro_semantico(yylineno, "'comprimento' espera um array, recebeu '%s'.",
                                   tipo_para_string($3->tipos[0]));
                    $$.tipo = criar_tipo(TIPO_ERRO, 0);
                } else {
                    $$.tipo = criar_tipo(TIPO_INT, 0);
                }
            }
            /* 2. Built-in: swap(a, b) -> void */
            else if (strcmp($1, "swap") == 0) {
                if ($3->count != 2) {
                    erro_semantico(yylineno, "'swap' espera 2 argumentos, recebeu %d.", $3->count);
                } else if (!tipos_identicos($3->tipos[0], $3->tipos[1])) {
                    erro_semantico(yylineno, "'swap' requer argumentos exatamente do mesmo tipo, recebeu '%s' e '%s'.",
                                   tipo_para_string($3->tipos[0]), tipo_para_string($3->tipos[1]));
                }
                $$.tipo = criar_tipo(TIPO_VOID, 0);
            }
            /* 3. Demais subrotinas definidas pelo usuario */
            else {
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
            free($1); free($3->texto); free($3->c_texto); free($3);
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
                char *novo_t = fmt("%s, ref %s", $1->texto, $4.texto);
                char *novo_c = fmt("%s, &(%s)", $1->c_texto, $4.texto);
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
            la->count = 1;
            la->texto = fmt("ref %s", $2.texto);
            la->c_texto = fmt("&(%s)", $2.texto);
            free($2.texto);
            $$ = la;
        }
    | lista_argumentos COMMA expressao
        {
            if ($1->count < 32) {
                int idx = $1->count++;
                $1->tipos[idx] = $3.tipo;
                $1->is_ref[idx] = 0;
                char *novo_t = fmt("%s, %s", $1->texto, $3.texto);
                char *novo_c = fmt("%s, %s", $1->c_texto, $3.texto);
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
            la->count = 1;
            la->texto = strdup($1.texto);
            la->c_texto = $1.texto;
            $$ = la;
        }
    ;

/* ---------- tipos ---------- */

tipo
    : INT                      { $$ = strdup("int"); }
    | REAL                     { $$ = strdup("real"); }
    | STRING                   { $$ = strdup("string"); }
    | STRUCT_ID                { $$ = $1; }
    | tipo LBRACKET RBRACKET   { $$ = fmt("%s[]", $1); free($1); }
    ;

/* ---------- expressoes ---------- */

expressao
    : exprelacional { $$ = $1; }
    ;

exprelacional
    : exparitimetica op_relacional exparitimetica
        {
            $$.texto = fmt("%s %s %s", $1.texto, $2, $3.texto);
            $$.tipo = verificar_relacional($1.tipo, $3.tipo, $2, yylineno);
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
            free($1.texto); free($3.texto);
        }
    | exparitimetica MINUS termo
        {
            $$.texto = fmt("%s - %s", $1.texto, $3.texto);
            $$.tipo = verificar_binario_aritmetico($1.tipo, $3.tipo, "-", yylineno);
            free($1.texto); free($3.texto);
        }
    | termo { $$ = $1; }
    ;

termo
    : termo MUL fator
        {
            $$.texto = fmt("%s * %s", $1.texto, $3.texto);
            $$.tipo = verificar_binario_aritmetico($1.tipo, $3.tipo, "*", yylineno);
            free($1.texto); free($3.texto);
        }
    | termo DIV fator
        {
            $$.texto = fmt("%s / %s", $1.texto, $3.texto);
            $$.tipo = verificar_binario_aritmetico($1.tipo, $3.tipo, "/", yylineno);
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
            } else if (s->is_func) {
                erro_semantico(yylineno, "'%s' e uma subrotina e nao pode ser usada como variavel.", $1);
                $$.texto = $1;
                $$.tipo = criar_tipo(TIPO_ERRO, 0);
            } else {
                $$.tipo = s->tipo;
                if (s->is_ref) {
                    $$.texto = fmt("(*%s)", $1);
                    free($1);
                } else {
                    $$.texto = $1;
                }
            }
        }
    | l_value LBRACKET expressao RBRACKET
        {
            $$.texto = fmt("%s[%s]", $1.texto, $3.texto);
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
            $$.texto = fmt("%s.%s", $1.texto, $3);
            if ($1.tipo.base == TIPO_ERRO) {
                $$.tipo = criar_tipo(TIPO_ERRO, 0);
            } else if ($1.tipo.base != TIPO_STRUCT || $1.tipo.dimensao != 0) {
                erro_semantico(yylineno, "Acesso ao campo '%s' em '%s', que nao e uma struct.",
                               $3, tipo_para_string($1.tipo));
                $$.tipo = criar_tipo(TIPO_ERRO, 0);
            } else {
                Tipo tc = buscar_campo_struct($1.tipo.nome_struct, $3);
                if (tc.base == TIPO_ERRO) {
                    erro_semantico(yylineno, "Struct '%s' nao possui o campo '%s'.",
                                   $1.tipo.nome_struct, $3);
                }
                $$.tipo = tc;
            }
            free($1.texto); free($3);
        }
    ;

fator
    : chamadafuncao           { $$ = $1; }
    | l_value                 { $$ = $1; }
    | INT_NUM                 { $$.texto = fmt("%d", $1); $$.tipo = criar_tipo(TIPO_INT, 0); }
    | REAL_NUM                { $$.texto = fmt("%g", $1); $$.tipo = criar_tipo(TIPO_REAL, 0); }
    | STRING_LITERAL          { $$.texto = $1; $$.tipo = criar_tipo(TIPO_STRING, 0); }
    | arrayLiteral            { $$ = $1; }
    | LPAREN expressao RPAREN { $$.texto = fmt("(%s)", $2.texto); $$.tipo = $2.tipo; free($2.texto); }
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
            free($2->texto);
            free($2->c_texto);
            free($2);
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

int main(void) {
    iniciar_semantico();
    out_c = fopen("saida.c", "w");
    if (!out_c) {
        perror("Erro ao abrir saida.c");
        return 1;
    }
    fprintf(out_c, "#include <stdio.h>\n#include <stdlib.h>\n\n");
    int res = yyparse();
    fclose(out_c);

    if (res != 0 || obter_erros_semanticos() > 0) {
        remove("saida.c"); /* Remove o arquivo se houve erro */
        return 1;
    }

    /* Compila saida.c para o executavel ./saida */
    int gcc_res = system("gcc saida.c -o saida");
    if (gcc_res != 0) {
        fprintf(stderr, "Erro ao compilar saida.c com o gcc.\n");
        return 1;
    }

    printf("Executavel './saida' gerado com sucesso.\n");
    return 0;
}

int yyerror(char *msg) {
    fprintf(stderr, "Linha %d: %s at '%s'\n", yylineno, msg, yytext);
    return 0;
}