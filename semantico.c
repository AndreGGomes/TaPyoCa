#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "semantico.h"

static Simbolo *tabela_simbolos = NULL;
static int escopo_atual = 0;
static int erros_semanticos = 0;

void iniciar_semantico(void) {
    tabela_simbolos = NULL;
    escopo_atual = 0;
    erros_semanticos = 0;
}

int obter_erros_semanticos(void) {
    return erros_semanticos;
}

void erro_semantico(int linha, const char *fmt, ...) {
    erros_semanticos++;
    fprintf(stderr, "Erro semantico na linha %d: ", linha);
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fprintf(stderr, "\n");
}

void entrar_escopo(void) {
    escopo_atual++;
}

void sair_escopo(void) {
    /* Remove todos os símbolos declarados no escopo que está fechando */
    while (tabela_simbolos != NULL && tabela_simbolos->nivel_escopo == escopo_atual) {
        Simbolo *temp = tabela_simbolos;
        tabela_simbolos = tabela_simbolos->prox;
        free(temp->nome);
        if (temp->params) free(temp->params);
        free(temp);
    }
    escopo_atual--;
}

Simbolo* buscar_no_escopo_atual(const char *nome) {
    for (Simbolo *s = tabela_simbolos; s != NULL; s = s->prox) {
        if (s->nivel_escopo < escopo_atual) break;
        if (strcmp(s->nome, nome) == 0) return s;
    }
    return NULL;
}

Simbolo* buscar_simbolo(const char *nome) {
    for (Simbolo *s = tabela_simbolos; s != NULL; s = s->prox) {
        if (strcmp(s->nome, nome) == 0) return s;
    }
    return NULL;
}

Simbolo* inserir_variavel(const char *nome, Tipo tipo, int linha) {
    if (buscar_no_escopo_atual(nome) != NULL) {
        erro_semantico(linha, "Identificador '%s' ja declarado neste escopo.", nome);
        return NULL;
    }
    Simbolo *s = malloc(sizeof(Simbolo));
    s->nome = strdup(nome);
    s->tipo = tipo;
    s->is_func = 0;
    s->num_params = 0;
    s->params = NULL;
    s->nivel_escopo = escopo_atual;
    s->prox = tabela_simbolos;
    tabela_simbolos = s;
    return s;
}

Simbolo* inserir_funcao(const char *nome, Tipo retorno, int num_params, Tipo *params, int linha) {
    if (buscar_no_escopo_atual(nome) != NULL) {
        erro_semantico(linha, "Subrotina '%s' ja declarada.", nome);
        return NULL;
    }
    Simbolo *s = malloc(sizeof(Simbolo));
    s->nome = strdup(nome);
    s->tipo = retorno;
    s->is_func = 1;
    s->num_params = num_params;
    s->params = params;
    s->nivel_escopo = escopo_atual;
    s->prox = tabela_simbolos;
    tabela_simbolos = s;
    return s;
}

Tipo criar_tipo(TipoBase base, int dimensao) {
    Tipo t = { base, dimensao };
    return t;
}

Tipo parse_tipo_string(const char *str) {
    if (!str || str[0] == '\0') return criar_tipo(TIPO_VOID, 0);
    int dim = 0;
    for (const char *p = str; *p; p++) {
        if (*p == '[') dim++;
    }
    if (strncmp(str, "int", 3) == 0) return criar_tipo(TIPO_INT, dim);
    if (strncmp(str, "real", 4) == 0) return criar_tipo(TIPO_REAL, dim);
    if (strncmp(str, "string", 6) == 0) return criar_tipo(TIPO_STRING, dim);
    return criar_tipo(TIPO_ERRO, 0);
}

const char* tipo_para_string(Tipo t) {
    static char buf[64];
    const char *base_str = "erro";
    switch (t.base) {
        case TIPO_VOID:   base_str = "void"; break;
        case TIPO_INT:    base_str = "int"; break;
        case TIPO_REAL:   base_str = "real"; break;
        case TIPO_STRING: base_str = "string"; break;
        default: break;
    }
    strcpy(buf, base_str);
    for (int i = 0; i < t.dimensao && i < 10; i++) {
        strcat(buf, "[]");
    }
    return buf;
}

int tipos_compativeis(Tipo destino, Tipo origem) {
    if (destino.base == TIPO_ERRO || origem.base == TIPO_ERRO) return 1; /* Evita cascata de erros */
    if (destino.dimensao != origem.dimensao) return 0;
    if (destino.base == origem.base) return 1;
    /* Coerção implícita permitida: int -> real (apenas para escalares) */
    if (destino.dimensao == 0 && destino.base == TIPO_REAL && origem.base == TIPO_INT) return 1;
    return 0;
}

Tipo verificar_binario_aritmetico(Tipo t1, Tipo t2, const char *op, int linha) {
    if (t1.base == TIPO_ERRO || t2.base == TIPO_ERRO) return criar_tipo(TIPO_ERRO, 0);

    if (t1.dimensao > 0 || t2.dimensao > 0) {
        erro_semantico(linha, "Operador '%s' nao pode ser aplicado diretamente a arrays.", op);
        return criar_tipo(TIPO_ERRO, 0);
    }

    /* Concatenação de strings com '+' */
    if (strcmp(op, "+") == 0 && t1.base == TIPO_STRING && t2.base == TIPO_STRING) {
        return criar_tipo(TIPO_STRING, 0);
    }

    if ((t1.base == TIPO_INT || t1.base == TIPO_REAL) &&
        (t2.base == TIPO_INT || t2.base == TIPO_REAL)) {
        if (t1.base == TIPO_REAL || t2.base == TIPO_REAL) {
            return criar_tipo(TIPO_REAL, 0);
        }
        return criar_tipo(TIPO_INT, 0);
    }

    erro_semantico(linha, "Tipos incompativeis '%s' e '%s' para o operador '%s'.",
                   tipo_para_string(t1), tipo_para_string(t2), op);
    return criar_tipo(TIPO_ERRO, 0);
}

Tipo verificar_relacional(Tipo t1, Tipo t2, const char *op, int linha) {
    if (t1.base == TIPO_ERRO || t2.base == TIPO_ERRO) return criar_tipo(TIPO_ERRO, 0);

    if (t1.dimensao > 0 || t2.dimensao > 0) {
        erro_semantico(linha, "Comparacao '%s' invalida entre arrays.", op);
        return criar_tipo(TIPO_ERRO, 0);
    }

    if ((t1.base == TIPO_INT || t1.base == TIPO_REAL) &&
        (t2.base == TIPO_INT || t2.base == TIPO_REAL)) {
        return criar_tipo(TIPO_INT, 0); /* Booleano representado como int */
    }

    if (t1.base == TIPO_STRING && t2.base == TIPO_STRING &&
       (strcmp(op, "==") == 0 || strcmp(op, "!=") == 0)) {
        return criar_tipo(TIPO_INT, 0);
    }

    erro_semantico(linha, "Nao e possivel comparar '%s' com '%s' usando '%s'.",
                   tipo_para_string(t1), tipo_para_string(t2), op);
    return criar_tipo(TIPO_ERRO, 0);
}