%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

int yylex(void);
int yyerror(char *s);
extern int yylineno;
extern char * yytext;

/* ---------- utilitarios de impressao ---------- */

static int depth = 0;   /* nivel de aninhamento atual (4 espacos por nivel) */

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

%}

%union {
    int ival;
    double fval;
    char* sval;
}

%token MAIN LPAREN RPAREN NEWLINE INDENT DEDENT ENDMAIN SUB ENDSUB COMMA
%token ASSIGN RETURN IF ENDIF WHILE ENDWHILE INT LBRACKET RBRACKET STRING REAL
%token LT LEQ GT GEQ EQ NEQ PLUS MINUS MUL DIV PRINT

%token <ival> INT_NUM
%token <fval> REAL_NUM
%token <sval> ID
%token <sval> STRING_LITERAL

/* Nao-terminais que carregam o "texto" da construcao */
%type <sval> tipo tipo_opt parametros_opt lista_parametros
%type <sval> argumentos_opt lista_argumentos array_elements_opt
%type <sval> expressao exprelacional op_relacional exparitimetica termo
%type <sval> l_value fator arrayLiteral chamadafuncao

%start programa

%%
programa : declaracoes_globais
        { printf("Analise sintatica concluida com sucesso.\n"); }
    ;

declaracoes_globais : declaracoes_globais declaracao {}
        | declaracao                                 {}
        ;

declaracao : subrotina {}
    | main_block       {}
    ;

main_block
    : MAIN LPAREN RPAREN
        { pr("[main]"); depth++; }
      NEWLINE INDENT comandos DEDENT ENDMAIN NEWLINE
        { depth--; pr("[endmain]"); printf("\n"); }
    ;

subrotina
    : SUB tipo_opt ID LPAREN parametros_opt RPAREN
        {
            if ($2[0]) pr("[sub] %s %s(%s)", $2, $3, $5);
            else       pr("[sub] %s(%s)", $3, $5);
            depth++;
            free($2); free($3); free($5);
        }
      NEWLINE INDENT comandos DEDENT ENDSUB NEWLINE
        { depth--; pr("[endsub]"); printf("\n"); }
    ;

tipo_opt
    : tipo         { $$ = $1; }
    | /* vazio */  { $$ = strdup(""); }
    ;

parametros_opt
    : lista_parametros { $$ = $1; }
    | /* vazio */      { $$ = strdup(""); }
    ;

lista_parametros
    : lista_parametros COMMA tipo ID
        { $$ = fmt("%s, %s %s", $1, $3, $4); free($1); free($3); free($4); }
    | tipo ID
        { $$ = fmt("%s %s", $1, $2); free($1); free($2); }
    ;

comandos
    : comandos comando NEWLINE {}
    | /* vazio */              {}
    ;

comando
    : declaracaovar {}
    | atribuicao    {}
    | comandoif     {}
    | comandowhile  {}
    | chamadafuncao { pr("[chamada] %s", $1); free($1); }
    | retorno       {}
    | comandoprint  {}
    ;

comandoprint
    : PRINT LPAREN argumentos_opt RPAREN
        { pr("[print] %s", $3); free($3); }
    ;

declaracaovar
    : tipo ID ASSIGN expressao
        { pr("[decl] %s %s <- %s", $1, $2, $4); free($1); free($2); free($4); }
    ;

atribuicao
    : l_value ASSIGN expressao
        { pr("[atrib] %s <- %s", $1, $3); free($1); free($3); }
    ;

/* O NEWLINE apos ENDWHILE/ENDIF e consumido pela regra "comandos" */
comandowhile
    : WHILE LPAREN expressao RPAREN
        { pr("[while] while (%s)", $3); depth++; free($3); }
      NEWLINE INDENT comandos DEDENT ENDWHILE
        { depth--; pr("[endwhile]"); }
    ;

comandoif
    : IF LPAREN expressao RPAREN
        { pr("[if] if (%s)", $3); depth++; free($3); }
      NEWLINE INDENT comandos DEDENT ENDIF
        { depth--; pr("[endif]"); }
    ;

retorno
    : RETURN expressao
        { pr("[return] %s", $2); free($2); }
    ;

chamadafuncao
    : ID LPAREN argumentos_opt RPAREN
        { $$ = fmt("%s(%s)", $1, $3); free($1); free($3); }
    ;

argumentos_opt
    : lista_argumentos { $$ = $1; }
    | /* vazio */      { $$ = strdup(""); }
    ;

lista_argumentos
    : lista_argumentos COMMA expressao
        { $$ = fmt("%s, %s", $1, $3); free($1); free($3); }
    | expressao { $$ = $1; }
    ;

tipo
    : INT                      { $$ = strdup("int"); }
    | REAL                     { $$ = strdup("real"); }
    | STRING                   { $$ = strdup("string"); }
    | tipo LBRACKET RBRACKET   { $$ = fmt("%s[]", $1); free($1); }
    ;

expressao
    : exprelacional { $$ = $1; }
    ;

exprelacional
    : exparitimetica op_relacional exparitimetica
        { $$ = fmt("%s %s %s", $1, $2, $3); free($1); free($2); free($3); }
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
        { $$ = fmt("%s + %s", $1, $3); free($1); free($3); }
    | exparitimetica MINUS termo
        { $$ = fmt("%s - %s", $1, $3); free($1); free($3); }
    | termo { $$ = $1; }
    ;

termo
    : termo MUL fator
        { $$ = fmt("%s * %s", $1, $3); free($1); free($3); }
    | termo DIV fator
        { $$ = fmt("%s / %s", $1, $3); free($1); free($3); }
    | fator { $$ = $1; }
    ;

l_value
    : ID { $$ = $1; }
    | l_value LBRACKET expressao RBRACKET
        { $$ = fmt("%s[%s]", $1, $3); free($1); free($3); }
    ;

fator
    : chamadafuncao           { $$ = $1; }
    | l_value                 { $$ = $1; }
    | INT_NUM                 { $$ = fmt("%d", $1); }
    | REAL_NUM                { $$ = fmt("%g", $1); }
    | STRING_LITERAL          { $$ = $1; }
    | arrayLiteral            { $$ = $1; }
    | LPAREN expressao RPAREN { $$ = fmt("(%s)", $2); free($2); }
    ;

arrayLiteral
    : LBRACKET array_elements_opt RBRACKET
        { $$ = fmt("[%s]", $2); free($2); }
    ;

array_elements_opt
    : lista_argumentos { $$ = $1; }
    | /* vazio */      { $$ = strdup(""); }
    ;

%%

int main (void) {
    /* Retorna o status de sucesso ou erro diretamente do parser */
    return yyparse();
}

int yyerror (char *msg) {
    fprintf (stderr, "Linha %d: %s at '%s'\n", yylineno, msg, yytext);
    return 0;
}
