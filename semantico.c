#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "semantico.h"

static Simbolo *tabela_simbolos = NULL;
static StructDef *tabela_structs = NULL;
static int escopo_atual = 0;
static int erros_semanticos = 0;

void iniciar_semantico(void) {
    tabela_simbolos = NULL;
    tabela_structs = NULL;
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

/* ---------- Gerenciamento de Structs ---------- */

StructDef* buscar_struct(const char *nome) {
    for (StructDef *st = tabela_structs; st != NULL; st = st->prox) {
        if (strcmp(st->nome, nome) == 0) return st;
    }
    return NULL;
}

StructDef* inserir_struct(const char *nome, int linha) {
    if (buscar_struct(nome) != NULL || buscar_simbolo(nome) != NULL) {
        erro_semantico(linha, "Struct '%s' ja declarada.", nome);
        return NULL;
    }
    StructDef *st = calloc(1, sizeof(StructDef));
    strncpy(st->nome, nome, 63);
    st->nome[63] = '\0';
    st->prox = tabela_structs;
    tabela_structs = st;
    return st;
}

int adicionar_campo_struct(StructDef *st, const char *nome_campo, Tipo tipo_campo, int linha) {
    if (!st) return 0;
    for (int i = 0; i < st->num_campos; i++) {
        if (strcmp(st->campos[i].nome, nome_campo) == 0) {
            erro_semantico(linha, "Campo '%s' duplicado na struct '%s'.", nome_campo, st->nome);
            return 0;
        }
    }
    if (st->num_campos >= 32) {
        erro_semantico(linha, "Limite maximo de 32 campos excedido na struct '%s'.", st->nome);
        return 0;
    }
    strncpy(st->campos[st->num_campos].nome, nome_campo, 63);
    st->campos[st->num_campos].nome[63] = '\0';
    st->campos[st->num_campos].tipo = tipo_campo;
    st->num_campos++;
    return 1;
}

Tipo buscar_campo_struct(const char *nome_struct, const char *nome_campo) {
    StructDef *st = buscar_struct(nome_struct);
    if (!st) return criar_tipo(TIPO_ERRO, 0);
    for (int i = 0; i < st->num_campos; i++) {
        if (strcmp(st->campos[i].nome, nome_campo) == 0) {
            return st->campos[i].tipo;
        }
    }
    return criar_tipo(TIPO_ERRO, 0);
}

/* ---------- Utilitarios de Tipo e Verificacoes ---------- */

Tipo criar_tipo(TipoBase base, int dimensao) {
    Tipo t;
    t.base = base;
    t.dimensao = dimensao;
    t.nome_struct[0] = '\0';
    return t;
}

Tipo criar_tipo_struct(const char *nome_struct, int dimensao) {
    Tipo t;
    t.base = TIPO_STRUCT;
    t.dimensao = dimensao;
    strncpy(t.nome_struct, nome_struct, 63);
    t.nome_struct[63] = '\0';
    return t;
}

Tipo parse_tipo_string(const char *str) {
    if (!str || str[0] == '\0') return criar_tipo(TIPO_VOID, 0);

    int dim = 0;
    for (const char *p = str; *p; p++) {
        if (*p == '[') dim++;
    }

    char base_nome[64];
    size_t len = strcspn(str, "[");
    if (len >= sizeof(base_nome)) len = sizeof(base_nome) - 1;
    strncpy(base_nome, str, len);
    base_nome[len] = '\0';

    if (strcmp(base_nome, "int") == 0)    return criar_tipo(TIPO_INT, dim);
    if (strcmp(base_nome, "real") == 0)   return criar_tipo(TIPO_REAL, dim);
    if (strcmp(base_nome, "string") == 0) return criar_tipo(TIPO_STRING, dim);

    return criar_tipo_struct(base_nome, dim);
}

const char* tipo_para_string(Tipo t) {
    static char bufs[4][64];
    static int idx = 0;
    char *buf = bufs[idx];
    idx = (idx + 1) % 4;

    const char *base_str = "erro";
    switch (t.base) {
        case TIPO_VOID:   base_str = "void"; break;
        case TIPO_INT:    base_str = "int"; break;
        case TIPO_REAL:   base_str = "real"; break;
        case TIPO_STRING: base_str = "string"; break;
        case TIPO_STRUCT: base_str = t.nome_struct; break;
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

    if (destino.base == TIPO_STRUCT || origem.base == TIPO_STRUCT) {
        return destino.base == TIPO_STRUCT && origem.base == TIPO_STRUCT &&
               strcmp(destino.nome_struct, origem.nome_struct) == 0;
    }

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