%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "y.tab.h" /* Inclui a definicao dos tokens gerados pelo Yacc */

#define MAX_INDENT 100
int indent_stack[MAX_INDENT] = {0};
int indent_top = 0;
int pending_dedents = 0;

int handle_indent(int current_indent);

#define YY_DECL int _yylex(void)

/* 1. MACRO PODEROSA: Inicia o Lexer forçando-o a checar a indentação na linha 1 */
#define YY_USER_INIT { BEGIN(INDENT_STATE); }
%}

%option yylineno
%option noyywrap

/* 2. Cria o estado exclusivo para processar indentação */
%x INDENT_STATE

DIGIT    [0-9]
ID       [a-zA-Z_][a-zA-Z0-9_]*
STRING   \"([^\\\"]|\\.)*\"
REAL_PAT {DIGIT}+"."{DIGIT}+

%%

 /* ================= ESTADO DE INDENTAÇÃO ================= */

 /* Ignora linhas vazias silenciosamente e continua esperando código */
<INDENT_STATE>[ \t]*\r?\n   { }

 /* Captura os espaços no início da linha (mesmo que seja 0 espaços) */
<INDENT_STATE>[ \t]+ {
    int current_indent = 0;
    for (int i = 0; i < yyleng; i++) {
        if (yytext[i] == ' ') current_indent++;
        else if (yytext[i] == '\t') current_indent += 4;
    }
    BEGIN(INITIAL);
    { int tok = handle_indent(current_indent); if (tok) return tok; }
}

 /* Linha comecando direto em codigo (indentacao 0): o flex nao casa o padrao
    vazio, entao tratamos aqui devolvendo o caractere para a entrada */
<INDENT_STATE>. {
    yyless(0);
    BEGIN(INITIAL);
    { int tok = handle_indent(0); if (tok) return tok; }
}

 /* ================= ESTADO NORMAL (INITIAL) ================= */

 /* Quando encontra uma quebra de linha normal, volta para o estado de indentação */
\n|\r\n    { 
    BEGIN(INDENT_STATE);
    return NEWLINE; 
}

 /* Ignora espaços e tabs no meio do código */
[ \t]+     ;

 /* Palavras-chave */
"main"     { return MAIN; }
"endmain"  { return ENDMAIN; }
"sub"      { return SUB; }
"endsub"   { return ENDSUB; }
"while"    { return WHILE; }
"endwhile" { return ENDWHILE; }
"if"       { return IF; }
"endif"    { return ENDIF; }
"return"   { return RETURN; }
"print"    { return PRINT; }

 /* Tipos de Dados */
"int"      { return INT; }
"real"     { return REAL; }
"string"   { return STRING; }

 /* Símbolos e Operadores */
"("        { return LPAREN; }
")"        { return RPAREN; }
"["        { return LBRACKET; }
"]"        { return RBRACKET; }
","        { return COMMA; }
"+"        { return PLUS; }
"-"        { return MINUS; }
"*"        { return MUL; }
"/"        { return DIV; }
"<="       { return LEQ; }
">="       { return GEQ; }
"=="       { return EQ; }
"!="       { return NEQ; }
"<"        { return LT; }
">"        { return GT; }
"="        { return ASSIGN; }

 /* Retornando valores */
{REAL_PAT} { 
    yylval.fval = atof(yytext); 
    return REAL_NUM; 
}
{DIGIT}+   { 
    yylval.ival = atoi(yytext); 
    return INT_NUM; 
}
{STRING}   { 
    yylval.sval = strdup(yytext); 
    return STRING_LITERAL; 
}
{ID}       { 
    yylval.sval = strdup(yytext); 
    return ID; 
}

 /* Caractere Invalido */
.          { printf("Erro lexico na linha %d: '%s'\n", yylineno, yytext); }

 /* ================= FIM DE ARQUIVO ================= */
<<EOF>>    { 
    if (indent_top > 0) {
        indent_top--;
        return DEDENT; 
    }
    yyterminate(); 
}

%%

/* Wrapper para distribuir múltiplos DEDENTs acumulados */
int yylex(void) {
    if (pending_dedents > 0) {
        pending_dedents--;
        return DEDENT;
    }
    return _yylex();
}

/* Compara a indentacao da linha com a pilha. Retorna INDENT, DEDENT ou 0 */
int handle_indent(int current_indent) {
    if (current_indent > indent_stack[indent_top]) {
        indent_stack[++indent_top] = current_indent;
        return INDENT;
    }
    if (current_indent < indent_stack[indent_top]) {
        while (indent_top > 0 && current_indent < indent_stack[indent_top]) {
            indent_top--;
            pending_dedents++;
        }
        if (pending_dedents > 0) {
            pending_dedents--;
            return DEDENT;
        }
    }
    return 0;
}
