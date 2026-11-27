/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 28 "parserTaPyoCa.y"

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
void yyerror(const char *msg);
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


#line 216 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 1 "parserTaPyoCa.y"

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

#line 278 "y.tab.c"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    MAIN = 258,                    /* MAIN  */
    LPAREN = 259,                  /* LPAREN  */
    RPAREN = 260,                  /* RPAREN  */
    NEWLINE = 261,                 /* NEWLINE  */
    INDENT = 262,                  /* INDENT  */
    DEDENT = 263,                  /* DEDENT  */
    ENDMAIN = 264,                 /* ENDMAIN  */
    SUB = 265,                     /* SUB  */
    ENDSUB = 266,                  /* ENDSUB  */
    COMMA = 267,                   /* COMMA  */
    ASSIGN = 268,                  /* ASSIGN  */
    RETURN = 269,                  /* RETURN  */
    IF = 270,                      /* IF  */
    ELSE = 271,                    /* ELSE  */
    ENDIF = 272,                   /* ENDIF  */
    WHILE = 273,                   /* WHILE  */
    ENDWHILE = 274,                /* ENDWHILE  */
    INT = 275,                     /* INT  */
    LBRACKET = 276,                /* LBRACKET  */
    RBRACKET = 277,                /* RBRACKET  */
    STRING = 278,                  /* STRING  */
    REAL = 279,                    /* REAL  */
    BOOL = 280,                    /* BOOL  */
    TRUE = 281,                    /* TRUE  */
    FALSE = 282,                   /* FALSE  */
    NULL_VAL = 283,                /* NULL_VAL  */
    LT = 284,                      /* LT  */
    LEQ = 285,                     /* LEQ  */
    GT = 286,                      /* GT  */
    GEQ = 287,                     /* GEQ  */
    EQ = 288,                      /* EQ  */
    NEQ = 289,                     /* NEQ  */
    AND = 290,                     /* AND  */
    OR = 291,                      /* OR  */
    NOT = 292,                     /* NOT  */
    PLUS = 293,                    /* PLUS  */
    MINUS = 294,                   /* MINUS  */
    MUL = 295,                     /* MUL  */
    DIV = 296,                     /* DIV  */
    POW = 297,                     /* POW  */
    PRINT = 298,                   /* PRINT  */
    READ = 299,                    /* READ  */
    REF = 300,                     /* REF  */
    STRUCT = 301,                  /* STRUCT  */
    ENDSTRUCT = 302,               /* ENDSTRUCT  */
    DOT = 303,                     /* DOT  */
    BREAK = 304,                   /* BREAK  */
    INT_NUM = 305,                 /* INT_NUM  */
    REAL_NUM = 306,                /* REAL_NUM  */
    ID = 307,                      /* ID  */
    STRUCT_ID = 308,               /* STRUCT_ID  */
    STRING_LITERAL = 309           /* STRING_LITERAL  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 186 "parserTaPyoCa.y"

    int ival;
    double fval;
    char *sval;
    ExprInfo expr;
    ListaParams *plist;
    ListaArgs *alist;

#line 358 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_MAIN = 3,                       /* MAIN  */
  YYSYMBOL_LPAREN = 4,                     /* LPAREN  */
  YYSYMBOL_RPAREN = 5,                     /* RPAREN  */
  YYSYMBOL_NEWLINE = 6,                    /* NEWLINE  */
  YYSYMBOL_INDENT = 7,                     /* INDENT  */
  YYSYMBOL_DEDENT = 8,                     /* DEDENT  */
  YYSYMBOL_ENDMAIN = 9,                    /* ENDMAIN  */
  YYSYMBOL_SUB = 10,                       /* SUB  */
  YYSYMBOL_ENDSUB = 11,                    /* ENDSUB  */
  YYSYMBOL_COMMA = 12,                     /* COMMA  */
  YYSYMBOL_ASSIGN = 13,                    /* ASSIGN  */
  YYSYMBOL_RETURN = 14,                    /* RETURN  */
  YYSYMBOL_IF = 15,                        /* IF  */
  YYSYMBOL_ELSE = 16,                      /* ELSE  */
  YYSYMBOL_ENDIF = 17,                     /* ENDIF  */
  YYSYMBOL_WHILE = 18,                     /* WHILE  */
  YYSYMBOL_ENDWHILE = 19,                  /* ENDWHILE  */
  YYSYMBOL_INT = 20,                       /* INT  */
  YYSYMBOL_LBRACKET = 21,                  /* LBRACKET  */
  YYSYMBOL_RBRACKET = 22,                  /* RBRACKET  */
  YYSYMBOL_STRING = 23,                    /* STRING  */
  YYSYMBOL_REAL = 24,                      /* REAL  */
  YYSYMBOL_BOOL = 25,                      /* BOOL  */
  YYSYMBOL_TRUE = 26,                      /* TRUE  */
  YYSYMBOL_FALSE = 27,                     /* FALSE  */
  YYSYMBOL_NULL_VAL = 28,                  /* NULL_VAL  */
  YYSYMBOL_LT = 29,                        /* LT  */
  YYSYMBOL_LEQ = 30,                       /* LEQ  */
  YYSYMBOL_GT = 31,                        /* GT  */
  YYSYMBOL_GEQ = 32,                       /* GEQ  */
  YYSYMBOL_EQ = 33,                        /* EQ  */
  YYSYMBOL_NEQ = 34,                       /* NEQ  */
  YYSYMBOL_AND = 35,                       /* AND  */
  YYSYMBOL_OR = 36,                        /* OR  */
  YYSYMBOL_NOT = 37,                       /* NOT  */
  YYSYMBOL_PLUS = 38,                      /* PLUS  */
  YYSYMBOL_MINUS = 39,                     /* MINUS  */
  YYSYMBOL_MUL = 40,                       /* MUL  */
  YYSYMBOL_DIV = 41,                       /* DIV  */
  YYSYMBOL_POW = 42,                       /* POW  */
  YYSYMBOL_PRINT = 43,                     /* PRINT  */
  YYSYMBOL_READ = 44,                      /* READ  */
  YYSYMBOL_REF = 45,                       /* REF  */
  YYSYMBOL_STRUCT = 46,                    /* STRUCT  */
  YYSYMBOL_ENDSTRUCT = 47,                 /* ENDSTRUCT  */
  YYSYMBOL_DOT = 48,                       /* DOT  */
  YYSYMBOL_BREAK = 49,                     /* BREAK  */
  YYSYMBOL_INT_NUM = 50,                   /* INT_NUM  */
  YYSYMBOL_REAL_NUM = 51,                  /* REAL_NUM  */
  YYSYMBOL_ID = 52,                        /* ID  */
  YYSYMBOL_STRUCT_ID = 53,                 /* STRUCT_ID  */
  YYSYMBOL_STRING_LITERAL = 54,            /* STRING_LITERAL  */
  YYSYMBOL_YYACCEPT = 55,                  /* $accept  */
  YYSYMBOL_programa = 56,                  /* programa  */
  YYSYMBOL_declaracoes_globais = 57,       /* declaracoes_globais  */
  YYSYMBOL_declaracao = 58,                /* declaracao  */
  YYSYMBOL_declaracao_struct = 59,         /* declaracao_struct  */
  YYSYMBOL_60_1 = 60,                      /* $@1  */
  YYSYMBOL_corpo_struct = 61,              /* corpo_struct  */
  YYSYMBOL_campos = 62,                    /* campos  */
  YYSYMBOL_campo = 63,                     /* campo  */
  YYSYMBOL_main_block = 64,                /* main_block  */
  YYSYMBOL_65_2 = 65,                      /* $@2  */
  YYSYMBOL_subrotina = 66,                 /* subrotina  */
  YYSYMBOL_67_3 = 67,                      /* $@3  */
  YYSYMBOL_tipo_opt = 68,                  /* tipo_opt  */
  YYSYMBOL_parametros_opt = 69,            /* parametros_opt  */
  YYSYMBOL_lista_parametros = 70,          /* lista_parametros  */
  YYSYMBOL_comandos = 71,                  /* comandos  */
  YYSYMBOL_corpo_bloco = 72,               /* corpo_bloco  */
  YYSYMBOL_comando = 73,                   /* comando  */
  YYSYMBOL_comandoprint = 74,              /* comandoprint  */
  YYSYMBOL_declaracaovar = 75,             /* declaracaovar  */
  YYSYMBOL_atribuicao = 76,                /* atribuicao  */
  YYSYMBOL_comandowhile = 77,              /* comandowhile  */
  YYSYMBOL_78_4 = 78,                      /* $@4  */
  YYSYMBOL_comandoif = 79,                 /* comandoif  */
  YYSYMBOL_80_5 = 80,                      /* $@5  */
  YYSYMBOL_cauda_if = 81,                  /* cauda_if  */
  YYSYMBOL_82_6 = 82,                      /* $@6  */
  YYSYMBOL_comandobreak = 83,              /* comandobreak  */
  YYSYMBOL_retorno = 84,                   /* retorno  */
  YYSYMBOL_chamadafuncao = 85,             /* chamadafuncao  */
  YYSYMBOL_argumentos_opt = 86,            /* argumentos_opt  */
  YYSYMBOL_lista_argumentos = 87,          /* lista_argumentos  */
  YYSYMBOL_tipo = 88,                      /* tipo  */
  YYSYMBOL_expressao = 89,                 /* expressao  */
  YYSYMBOL_explogica = 90,                 /* explogica  */
  YYSYMBOL_exp_and = 91,                   /* exp_and  */
  YYSYMBOL_exp_not = 92,                   /* exp_not  */
  YYSYMBOL_exprelacional = 93,             /* exprelacional  */
  YYSYMBOL_op_relacional = 94,             /* op_relacional  */
  YYSYMBOL_exparitimetica = 95,            /* exparitimetica  */
  YYSYMBOL_termo = 96,                     /* termo  */
  YYSYMBOL_unario = 97,                    /* unario  */
  YYSYMBOL_potencia = 98,                  /* potencia  */
  YYSYMBOL_l_value = 99,                   /* l_value  */
  YYSYMBOL_fator = 100,                    /* fator  */
  YYSYMBOL_arrayLiteral = 101,             /* arrayLiteral  */
  YYSYMBOL_array_elements_opt = 102        /* array_elements_opt  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;



/* Unqualified %code blocks.  */
#line 173 "parserTaPyoCa.y"

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

#line 500 "y.tab.c"

#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  19
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   308

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  55
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  48
/* YYNRULES -- Number of rules.  */
#define YYNRULES  114
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  202

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   309


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   221,   221,   233,   234,   238,   239,   240,   247,   246,
     268,   269,   273,   274,   278,   287,   302,   301,   323,   322,
     363,   364,   368,   370,   380,   405,   422,   447,   469,   470,
     474,   475,   479,   480,   481,   482,   483,   484,   485,   486,
     490,   560,   603,   644,   660,   677,   697,   711,   749,   767,
     795,   794,   853,   852,   885,   900,   899,   931,   945,   962,
     977,  1113,  1115,  1125,  1146,  1161,  1182,  1202,  1203,  1204,
    1205,  1206,  1207,  1213,  1217,  1224,  1228,  1235,  1239,  1246,
    1250,  1264,  1268,  1269,  1270,  1271,  1272,  1273,  1277,  1284,
    1291,  1295,  1302,  1309,  1313,  1320,  1327,  1331,  1342,  1346,
    1382,  1406,  1436,  1437,  1438,  1439,  1440,  1441,  1442,  1443,
    1444,  1445,  1449,  1490,  1492
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "MAIN", "LPAREN",
  "RPAREN", "NEWLINE", "INDENT", "DEDENT", "ENDMAIN", "SUB", "ENDSUB",
  "COMMA", "ASSIGN", "RETURN", "IF", "ELSE", "ENDIF", "WHILE", "ENDWHILE",
  "INT", "LBRACKET", "RBRACKET", "STRING", "REAL", "BOOL", "TRUE", "FALSE",
  "NULL_VAL", "LT", "LEQ", "GT", "GEQ", "EQ", "NEQ", "AND", "OR", "NOT",
  "PLUS", "MINUS", "MUL", "DIV", "POW", "PRINT", "READ", "REF", "STRUCT",
  "ENDSTRUCT", "DOT", "BREAK", "INT_NUM", "REAL_NUM", "ID", "STRUCT_ID",
  "STRING_LITERAL", "$accept", "programa", "declaracoes_globais",
  "declaracao", "declaracao_struct", "$@1", "corpo_struct", "campos",
  "campo", "main_block", "$@2", "subrotina", "$@3", "tipo_opt",
  "parametros_opt", "lista_parametros", "comandos", "corpo_bloco",
  "comando", "comandoprint", "declaracaovar", "atribuicao", "comandowhile",
  "$@4", "comandoif", "$@5", "cauda_if", "$@6", "comandobreak", "retorno",
  "chamadafuncao", "argumentos_opt", "lista_argumentos", "tipo",
  "expressao", "explogica", "exp_and", "exp_not", "exprelacional",
  "op_relacional", "exparitimetica", "termo", "unario", "potencia",
  "l_value", "fator", "arrayLiteral", "array_elements_opt", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-115)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      23,     6,   133,   -38,    22,    23,  -115,  -115,  -115,  -115,
      18,  -115,  -115,  -115,  -115,  -115,   -28,     4,  -115,  -115,
    -115,  -115,    24,     8,    25,    29,    47,  -115,    61,    69,
     133,    31,    67,   -10,  -115,    38,  -115,    78,    -9,  -115,
     226,  -115,    21,    83,   244,    84,  -115,    85,   133,    36,
    -115,   133,    91,    52,  -115,  -115,   203,    94,    95,    97,
     133,  -115,    98,   115,  -115,  -115,  -115,  -115,  -115,  -115,
    -115,  -115,    57,    -8,  -115,    69,    59,  -115,    60,  -115,
    -115,   203,   124,  -115,  -115,  -115,   203,    56,    56,  -115,
    -115,  -115,  -115,  -115,    86,    88,  -115,  -115,   269,   -32,
    -115,  -115,   -14,    82,  -115,   203,   203,   124,    65,    11,
    -115,   113,    92,   203,    75,   121,  -115,  -115,   128,    89,
     122,  -115,   116,  -115,  -115,  -115,   203,   203,  -115,  -115,
    -115,  -115,  -115,  -115,    56,    56,    56,    56,    56,    56,
     135,   142,   149,   122,   136,   150,   154,   144,   156,    89,
    -115,   146,  -115,   158,  -115,  -115,   -14,   163,  -115,    88,
    -115,   -32,   -32,   -22,  -115,  -115,  -115,  -115,  -115,  -115,
     183,  -115,  -115,   162,  -115,   168,   -14,  -115,  -115,    89,
    -115,   171,   173,    89,  -115,   175,  -115,   -14,    69,    69,
     -14,  -115,     2,   166,  -115,  -115,  -115,  -115,   186,    69,
     176,  -115
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,    21,     0,     0,     2,     4,     7,     6,     5,
       0,    67,    69,    68,    70,    71,     0,    20,     8,     1,
       3,    16,     0,     0,     0,     0,    23,    72,    11,    31,
       0,     0,    22,     0,    13,     0,    29,     0,     0,    18,
       0,    27,     0,     0,     0,     0,    25,     0,     0,     0,
      10,     0,     0,     0,     9,    30,     0,     0,     0,     0,
       0,    57,    99,     0,    39,    32,    33,    35,    34,    36,
      38,    37,     0,     0,    17,    31,     0,    26,     0,    12,
      15,     0,   114,   107,   108,   109,     0,     0,     0,   104,
     105,   106,   102,    58,    73,    75,    77,    79,    81,    90,
      93,    96,   103,    98,   110,     0,     0,    62,     0,    62,
      28,    43,     0,     0,     0,     0,    24,    14,     0,     0,
     113,    66,     0,    78,    95,    94,     0,     0,    82,    83,
      84,    85,    86,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    61,    46,     0,     0,     0,     0,     0,
      49,     0,   101,     0,   111,    99,    64,     0,   112,    74,
      76,    88,    89,    80,    91,    92,    97,    52,    50,    40,
       0,    59,    60,     0,    42,     0,    48,   100,    19,     0,
      65,     0,     0,     0,    45,     0,    47,    63,    31,    31,
      44,    41,     0,     0,    55,    54,    53,    51,     0,    31,
       0,    56
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -115,  -115,  -115,   192,  -115,  -115,  -115,  -115,  -115,  -115,
    -115,  -115,  -115,  -115,  -115,  -115,  -115,   -74,  -115,  -115,
    -115,  -115,  -115,  -115,  -115,  -115,  -115,  -115,  -115,  -115,
     155,    96,   130,   230,   -54,  -115,    77,   -80,  -115,  -115,
      70,  -114,   -84,  -115,   -44,  -115,  -115,  -115
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     4,     5,     6,     7,    24,    35,    42,    52,     8,
      25,     9,    47,    16,    31,    32,    44,    37,    63,    64,
      65,    66,    67,   182,    68,   181,   196,   198,    69,    70,
      92,   142,   143,    17,   121,    94,    95,    96,    97,   136,
      98,    99,   100,   101,   102,   103,   104,   122
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      73,   115,    93,   124,   125,   112,   123,   113,   137,   138,
      10,    23,    23,   113,    18,    81,   134,   135,   194,   195,
     161,   162,    19,    21,    22,    23,     1,   118,    26,    50,
      27,    28,    82,     2,   114,    29,    39,    83,    84,    85,
     114,    11,    41,    46,    12,    13,    14,   160,    86,    87,
      88,   140,   141,   164,   165,   166,   119,    23,   150,   151,
      81,    89,    90,    62,   145,    91,    51,    11,    34,     3,
      12,    13,    14,    23,    15,   156,    36,    82,    23,    40,
      23,    23,    83,    84,    85,    43,    23,    45,    77,    54,
      74,    75,    30,   174,    87,    88,    81,    79,   105,   106,
      15,   107,   109,   180,    80,   176,    89,    90,    62,   111,
      91,   116,   117,    82,   192,   193,   184,   144,    83,    84,
      85,   110,   126,   127,   139,   200,   147,   152,    81,    86,
      87,    88,   153,   154,   157,   187,   148,   149,   158,   190,
     167,   155,    89,    90,    62,    82,    91,   168,    81,   170,
      83,    84,    85,    11,   169,   171,    12,    13,    14,   172,
     175,    86,    87,    88,   178,    82,   185,    81,   177,   119,
      83,    84,    85,   186,    89,    90,    62,   188,    91,   189,
     191,    86,    87,    88,    82,   197,    15,    81,   173,    83,
      84,    85,   199,   201,    89,    90,    62,    20,    91,    71,
      86,    87,    88,   159,    82,   146,   163,    81,   179,    83,
      84,    85,   120,    89,    90,    62,     0,    91,     0,     0,
      86,    87,    88,     0,    82,     0,     0,     0,   183,    83,
      84,    85,     0,    89,    90,    62,     0,    91,     0,     0,
      86,    87,    88,     0,     0,     0,    11,     0,     0,    12,
      13,    14,    55,    89,    90,    62,    33,    91,    56,    57,
      38,     0,    58,     0,    11,     0,     0,    12,    13,    14,
      49,    48,    53,     0,    72,     0,     0,     0,    76,    15,
       0,    78,     0,     0,     0,     0,     0,    59,     0,    60,
     108,     0,     0,    61,     0,     0,    62,    15,   128,   129,
     130,   131,   132,   133,     0,     0,     0,   134,   135
};

static const yytype_int16 yycheck[] =
{
      44,    75,    56,    87,    88,    13,    86,    21,    40,    41,
       4,    21,    21,    21,    52,     4,    38,    39,    16,    17,
     134,   135,     0,     5,    52,    21,     3,    81,     4,     8,
      22,     6,    21,    10,    48,     6,     5,    26,    27,    28,
      48,    20,    52,    52,    23,    24,    25,   127,    37,    38,
      39,   105,   106,   137,   138,   139,    45,    21,   112,   113,
       4,    50,    51,    52,    53,    54,    45,    20,     7,    46,
      23,    24,    25,    21,    53,   119,     7,    21,    21,    12,
      21,    21,    26,    27,    28,    47,    21,     9,    52,     6,
       6,     6,    45,   147,    38,    39,     4,     6,     4,     4,
      53,     4,     4,   157,    52,   149,    50,    51,    52,    52,
      54,    52,    52,    21,   188,   189,   170,    52,    26,    27,
      28,     6,    36,    35,    42,   199,    13,    52,     4,    37,
      38,    39,    11,     5,    12,   179,    44,    45,    22,   183,
       5,    52,    50,    51,    52,    21,    54,     5,     4,    13,
      26,    27,    28,    20,     5,     5,    23,    24,    25,     5,
       4,    37,    38,    39,     6,    21,     4,     4,    22,    45,
      26,    27,    28,     5,    50,    51,    52,     6,    54,     6,
       5,    37,    38,    39,    21,    19,    53,     4,    44,    26,
      27,    28,     6,    17,    50,    51,    52,     5,    54,    44,
      37,    38,    39,   126,    21,   109,   136,     4,    45,    26,
      27,    28,    82,    50,    51,    52,    -1,    54,    -1,    -1,
      37,    38,    39,    -1,    21,    -1,    -1,    -1,    45,    26,
      27,    28,    -1,    50,    51,    52,    -1,    54,    -1,    -1,
      37,    38,    39,    -1,    -1,    -1,    20,    -1,    -1,    23,
      24,    25,     8,    50,    51,    52,    26,    54,    14,    15,
      30,    -1,    18,    -1,    20,    -1,    -1,    23,    24,    25,
      40,    45,    42,    -1,    44,    -1,    -1,    -1,    48,    53,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    43,    -1,    45,
      60,    -1,    -1,    49,    -1,    -1,    52,    53,    29,    30,
      31,    32,    33,    34,    -1,    -1,    -1,    38,    39
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    10,    46,    56,    57,    58,    59,    64,    66,
       4,    20,    23,    24,    25,    53,    68,    88,    52,     0,
      58,     5,    52,    21,    60,    65,     4,    22,     6,     6,
      45,    69,    70,    88,     7,    61,     7,    72,    88,     5,
      12,    52,    62,    47,    71,     9,    52,    67,    45,    88,
       8,    45,    63,    88,     6,     8,    14,    15,    18,    43,
      45,    49,    52,    73,    74,    75,    76,    77,    79,    83,
      84,    85,    88,    99,     6,     6,    88,    52,    88,     6,
      52,     4,    21,    26,    27,    28,    37,    38,    39,    50,
      51,    54,    85,    89,    90,    91,    92,    93,    95,    96,
      97,    98,    99,   100,   101,     4,     4,     4,    88,     4,
       6,    52,    13,    21,    48,    72,    52,    52,    89,    45,
      87,    89,   102,    92,    97,    97,    36,    35,    29,    30,
      31,    32,    33,    34,    38,    39,    94,    40,    41,    42,
      89,    89,    86,    87,    52,    53,    86,    13,    44,    45,
      89,    89,    52,    11,     5,    52,    99,    12,    22,    91,
      92,    96,    96,    95,    97,    97,    97,     5,     5,     5,
      13,     5,     5,    44,    89,     4,    99,    22,     6,    45,
      89,    80,    78,    45,    89,     4,     5,    99,     6,     6,
      99,     5,    72,    72,    16,    17,    81,    19,    82,     6,
      72,    17
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    55,    56,    57,    57,    58,    58,    58,    60,    59,
      61,    61,    62,    62,    63,    63,    65,    64,    67,    66,
      68,    68,    69,    69,    70,    70,    70,    70,    71,    71,
      72,    72,    73,    73,    73,    73,    73,    73,    73,    73,
      74,    75,    75,    75,    75,    75,    75,    76,    76,    76,
      78,    77,    80,    79,    81,    82,    81,    83,    84,    85,
      85,    86,    86,    87,    87,    87,    87,    88,    88,    88,
      88,    88,    88,    89,    90,    90,    91,    91,    92,    92,
      93,    93,    94,    94,    94,    94,    94,    94,    95,    95,
      95,    96,    96,    96,    97,    97,    97,    98,    98,    99,
      99,    99,   100,   100,   100,   100,   100,   100,   100,   100,
     100,   100,   101,   102,   102
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     0,     7,
       3,     0,     3,     0,     3,     2,     0,     8,     0,    11,
       1,     0,     1,     0,     5,     3,     4,     2,     3,     0,
       3,     0,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     6,     4,     2,     6,     5,     3,     5,     4,     3,
       0,     8,     0,     8,     1,     0,     5,     1,     2,     4,
       4,     1,     0,     4,     2,     3,     1,     1,     1,     1,
       1,     1,     3,     1,     3,     1,     3,     1,     2,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       1,     3,     3,     1,     2,     2,     1,     3,     1,     1,
       4,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     1,     0
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* programa: declaracoes_globais  */
#line 222 "parserTaPyoCa.y"
        {
            if (obter_erros_semanticos() == 0) {
                printf("Analise sintatica e semantica concluida com sucesso.\n");
            } else {
                fprintf(stderr, "Falha na compilacao: %d erro(s) semantico(s).\n", obter_erros_semanticos());
                YYABORT;
            }
        }
#line 1898 "y.tab.c"
    break;

  case 3: /* declaracoes_globais: declaracoes_globais declaracao  */
#line 233 "parserTaPyoCa.y"
                                     {}
#line 1904 "y.tab.c"
    break;

  case 4: /* declaracoes_globais: declaracao  */
#line 234 "parserTaPyoCa.y"
                                     {}
#line 1910 "y.tab.c"
    break;

  case 5: /* declaracao: subrotina  */
#line 238 "parserTaPyoCa.y"
                        {}
#line 1916 "y.tab.c"
    break;

  case 6: /* declaracao: main_block  */
#line 239 "parserTaPyoCa.y"
                        {}
#line 1922 "y.tab.c"
    break;

  case 7: /* declaracao: declaracao_struct  */
#line 240 "parserTaPyoCa.y"
                        {}
#line 1928 "y.tab.c"
    break;

  case 8: /* $@1: %empty  */
#line 247 "parserTaPyoCa.y"
        {
            pr("[struct] %s", (yyvsp[0].sval));
            struct_atual = inserir_struct((yyvsp[0].sval), yylineno);
            emit_c("struct %s {", (yyvsp[0].sval));
            depth++;
        }
#line 1939 "y.tab.c"
    break;

  case 9: /* declaracao_struct: STRUCT ID $@1 NEWLINE corpo_struct ENDSTRUCT NEWLINE  */
#line 254 "parserTaPyoCa.y"
        {
            if (struct_atual && struct_atual->num_campos == 0) {
                erro_semantico(yylineno, "Struct '%s' deve possuir ao menos um campo.", (yyvsp[-5].sval));
            }
            depth--;
            emit_c("};\n");
            pr("[endstruct]");
            printf("\n");
            struct_atual = NULL;
            free((yyvsp[-5].sval));
        }
#line 1955 "y.tab.c"
    break;

  case 10: /* corpo_struct: INDENT campos DEDENT  */
#line 268 "parserTaPyoCa.y"
                           {}
#line 1961 "y.tab.c"
    break;

  case 11: /* corpo_struct: %empty  */
#line 269 "parserTaPyoCa.y"
                           {}
#line 1967 "y.tab.c"
    break;

  case 12: /* campos: campos campo NEWLINE  */
#line 273 "parserTaPyoCa.y"
                           {}
#line 1973 "y.tab.c"
    break;

  case 13: /* campos: %empty  */
#line 274 "parserTaPyoCa.y"
                           {}
#line 1979 "y.tab.c"
    break;

  case 14: /* campo: REF tipo ID  */
#line 279 "parserTaPyoCa.y"
        {
            pr("[campo] ref %s %s", (yyvsp[-1].sval), (yyvsp[0].sval));
            Tipo t = parse_tipo_string((yyvsp[-1].sval));
            adicionar_campo_struct(struct_atual, (yyvsp[0].sval), t, 1, yylineno);
            char *t_c = tipo_para_c(t);
            emit_c("%s *%s;", t_c, (yyvsp[0].sval));
            free(t_c); free((yyvsp[-1].sval)); free((yyvsp[0].sval));
        }
#line 1992 "y.tab.c"
    break;

  case 15: /* campo: tipo ID  */
#line 288 "parserTaPyoCa.y"
        {
            pr("[campo] %s %s", (yyvsp[-1].sval), (yyvsp[0].sval));
            Tipo t = parse_tipo_string((yyvsp[-1].sval));
            adicionar_campo_struct(struct_atual, (yyvsp[0].sval), t, 0, yylineno);
            char *t_c = tipo_para_c(t);
            emit_c("%s %s;", t_c, (yyvsp[0].sval));
            free(t_c); free((yyvsp[-1].sval)); free((yyvsp[0].sval));
        }
#line 2005 "y.tab.c"
    break;

  case 16: /* $@2: %empty  */
#line 302 "parserTaPyoCa.y"
        {
            pr("[main]");
            emit_c("int main(void) %c", 123);
            depth++;
            retorno_esperado = criar_tipo(TIPO_VOID, 0);
            encontrou_return = 0;
            entrar_escopo();
        }
#line 2018 "y.tab.c"
    break;

  case 17: /* main_block: MAIN LPAREN RPAREN $@2 NEWLINE corpo_bloco ENDMAIN NEWLINE  */
#line 311 "parserTaPyoCa.y"
        {
            sair_escopo();
            emit_c("return 0;");
            depth--;
            emit_c("%c\n", 125);
            pr("[endmain]");
            printf("\n");
        }
#line 2031 "y.tab.c"
    break;

  case 18: /* $@3: %empty  */
#line 323 "parserTaPyoCa.y"
        {
            if ((yyvsp[-4].sval)[0]) pr("[sub] %s %s(%s)", (yyvsp[-4].sval), (yyvsp[-3].sval), (yyvsp[-1].plist)->texto);
            else       pr("[sub] %s(%s)", (yyvsp[-3].sval), (yyvsp[-1].plist)->texto);

            Tipo ret = parse_tipo_string((yyvsp[-4].sval));
            char *ret_c = tipo_para_c(ret);
            emit_c("%s %s(%s) {", ret_c, (yyvsp[-3].sval), (yyvsp[-1].plist)->c_texto);
            free(ret_c);
            depth++;

            /* Registra a funcao no escopo global com os tipos e flags is_ref */
            inserir_funcao_ref((yyvsp[-3].sval), ret, (yyvsp[-1].plist)->count, (yyvsp[-1].plist)->tipos, (yyvsp[-1].plist)->is_ref, yylineno);

            /* Abre o escopo local e insere os parametros marcando quais sao 'ref' */
            entrar_escopo();
            for (int i = 0; i < (yyvsp[-1].plist)->count; i++) {
                inserir_variavel_ref((yyvsp[-1].plist)->nomes[i], (yyvsp[-1].plist)->tipos[i], (yyvsp[-1].plist)->is_ref[i], yylineno);
                free((yyvsp[-1].plist)->nomes[i]);
            }

            retorno_esperado = ret;
            encontrou_return = 0;

            free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-1].plist)->texto); free((yyvsp[-1].plist)->c_texto); free((yyvsp[-1].plist));
        }
#line 2061 "y.tab.c"
    break;

  case 19: /* subrotina: SUB tipo_opt ID LPAREN parametros_opt RPAREN $@3 NEWLINE corpo_bloco ENDSUB NEWLINE  */
#line 349 "parserTaPyoCa.y"
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
#line 2077 "y.tab.c"
    break;

  case 20: /* tipo_opt: tipo  */
#line 363 "parserTaPyoCa.y"
                   { (yyval.sval) = (yyvsp[0].sval); }
#line 2083 "y.tab.c"
    break;

  case 21: /* tipo_opt: %empty  */
#line 364 "parserTaPyoCa.y"
                   { (yyval.sval) = strdup(""); }
#line 2089 "y.tab.c"
    break;

  case 22: /* parametros_opt: lista_parametros  */
#line 368 "parserTaPyoCa.y"
                       { (yyval.plist) = (yyvsp[0].plist); }
#line 2095 "y.tab.c"
    break;

  case 23: /* parametros_opt: %empty  */
#line 370 "parserTaPyoCa.y"
        {
            ListaParams *lp = calloc(1, sizeof(ListaParams));
            lp->count = 0;
            lp->texto = strdup("");
            lp->c_texto = strdup("");
            (yyval.plist) = lp;
        }
#line 2107 "y.tab.c"
    break;

  case 24: /* lista_parametros: lista_parametros COMMA REF tipo ID  */
#line 381 "parserTaPyoCa.y"
        {
            (yyval.plist) = (yyvsp[-4].plist);
            Tipo t = parse_tipo_string((yyvsp[-1].sval));
            char *t_c = tipo_para_c(t);

            if ((yyval.plist)->count < 32) {
                int idx = (yyval.plist)->count++;
                (yyval.plist)->nomes[idx] = (yyvsp[0].sval);
                (yyval.plist)->tipos[idx] = t;
                (yyval.plist)->is_ref[idx] = 1; /* Marca como parametro por referencia */

                char *novo = fmt("%s, ref %s %s", (yyval.plist)->texto, (yyvsp[-1].sval), (yyval.plist)->nomes[idx]);
                char *novo_c = (t.dimensao > 0)
                    ? fmt("%s, %s *%s, int *_len_%s", (yyval.plist)->c_texto, t_c, (yyval.plist)->nomes[idx], (yyval.plist)->nomes[idx])
                    : fmt("%s, %s *%s", (yyval.plist)->c_texto, t_c, (yyval.plist)->nomes[idx]);
                free((yyval.plist)->texto); free((yyval.plist)->c_texto);
                (yyval.plist)->texto = novo;
                (yyval.plist)->c_texto = novo_c;
            } else {
                erro_semantico(yylineno, "Limite maximo de 32 parametros excedido.");
                free((yyvsp[0].sval));
            }
            free((yyvsp[-1].sval)); free(t_c);
        }
#line 2136 "y.tab.c"
    break;

  case 25: /* lista_parametros: REF tipo ID  */
#line 406 "parserTaPyoCa.y"
        {
            ListaParams *lp = calloc(1, sizeof(ListaParams));
            Tipo t = parse_tipo_string((yyvsp[-1].sval));
            char *t_c = tipo_para_c(t);

            lp->nomes[0] = (yyvsp[0].sval);
            lp->tipos[0] = t;
            lp->is_ref[0] = 1; /* Marca como parametro por referencia */
            lp->count = 1;
            lp->texto = fmt("ref %s %s", (yyvsp[-1].sval), (yyvsp[0].sval));
            lp->c_texto = (t.dimensao > 0)
                ? fmt("%s *%s, int *_len_%s", t_c, (yyvsp[0].sval), (yyvsp[0].sval))
                : fmt("%s *%s", t_c, (yyvsp[0].sval));
            free((yyvsp[-1].sval)); free(t_c);
            (yyval.plist) = lp;
        }
#line 2157 "y.tab.c"
    break;

  case 26: /* lista_parametros: lista_parametros COMMA tipo ID  */
#line 423 "parserTaPyoCa.y"
        {
            (yyval.plist) = (yyvsp[-3].plist);
            Tipo t = parse_tipo_string((yyvsp[-1].sval));
            char *t_c = tipo_para_c(t);

            if ((yyval.plist)->count < 32) {
                int idx = (yyval.plist)->count++;
                (yyval.plist)->nomes[idx] = (yyvsp[0].sval);
                (yyval.plist)->tipos[idx] = t;
                (yyval.plist)->is_ref[idx] = 0; /* Parametro normal (por valor) */

                char *novo = fmt("%s, %s %s", (yyval.plist)->texto, (yyvsp[-1].sval), (yyval.plist)->nomes[idx]);
                char *novo_c = (t.dimensao > 0)
                    ? fmt("%s, %s %s, int _len_%s", (yyval.plist)->c_texto, t_c, (yyval.plist)->nomes[idx], (yyval.plist)->nomes[idx])
                    : fmt("%s, %s %s", (yyval.plist)->c_texto, t_c, (yyval.plist)->nomes[idx]);
                free((yyval.plist)->texto); free((yyval.plist)->c_texto);
                (yyval.plist)->texto = novo;
                (yyval.plist)->c_texto = novo_c;
            } else {
                erro_semantico(yylineno, "Limite maximo de 32 parametros excedido.");
                free((yyvsp[0].sval));
            }
            free((yyvsp[-1].sval)); free(t_c);
        }
#line 2186 "y.tab.c"
    break;

  case 27: /* lista_parametros: tipo ID  */
#line 448 "parserTaPyoCa.y"
        {
            ListaParams *lp = calloc(1, sizeof(ListaParams));
            Tipo t = parse_tipo_string((yyvsp[-1].sval));
            char *t_c = tipo_para_c(t);

            lp->nomes[0] = (yyvsp[0].sval);
            lp->tipos[0] = t;
            lp->is_ref[0] = 0; /* Parametro normal (por valor) */
            lp->count = 1;
            lp->texto = fmt("%s %s", (yyvsp[-1].sval), (yyvsp[0].sval));
            lp->c_texto = (t.dimensao > 0)
                ? fmt("%s %s, int _len_%s", t_c, (yyvsp[0].sval), (yyvsp[0].sval))
                : fmt("%s %s", t_c, (yyvsp[0].sval));
            free((yyvsp[-1].sval)); free(t_c);
            (yyval.plist) = lp;
        }
#line 2207 "y.tab.c"
    break;

  case 28: /* comandos: comandos comando NEWLINE  */
#line 469 "parserTaPyoCa.y"
                               {}
#line 2213 "y.tab.c"
    break;

  case 29: /* comandos: %empty  */
#line 470 "parserTaPyoCa.y"
                               {}
#line 2219 "y.tab.c"
    break;

  case 30: /* corpo_bloco: INDENT comandos DEDENT  */
#line 474 "parserTaPyoCa.y"
                             {}
#line 2225 "y.tab.c"
    break;

  case 31: /* corpo_bloco: %empty  */
#line 475 "parserTaPyoCa.y"
                             {}
#line 2231 "y.tab.c"
    break;

  case 32: /* comando: declaracaovar  */
#line 479 "parserTaPyoCa.y"
                    {}
#line 2237 "y.tab.c"
    break;

  case 33: /* comando: atribuicao  */
#line 480 "parserTaPyoCa.y"
                    {}
#line 2243 "y.tab.c"
    break;

  case 34: /* comando: comandoif  */
#line 481 "parserTaPyoCa.y"
                    {}
#line 2249 "y.tab.c"
    break;

  case 35: /* comando: comandowhile  */
#line 482 "parserTaPyoCa.y"
                    {}
#line 2255 "y.tab.c"
    break;

  case 36: /* comando: comandobreak  */
#line 483 "parserTaPyoCa.y"
                    {}
#line 2261 "y.tab.c"
    break;

  case 37: /* comando: chamadafuncao  */
#line 484 "parserTaPyoCa.y"
                    { pr("[chamada] %s", (yyvsp[0].expr).texto); emit_c("%s;", (yyvsp[0].expr).texto); free((yyvsp[0].expr).texto); }
#line 2267 "y.tab.c"
    break;

  case 38: /* comando: retorno  */
#line 485 "parserTaPyoCa.y"
                    {}
#line 2273 "y.tab.c"
    break;

  case 39: /* comando: comandoprint  */
#line 486 "parserTaPyoCa.y"
                    {}
#line 2279 "y.tab.c"
    break;

  case 40: /* comandoprint: PRINT LPAREN argumentos_opt RPAREN  */
#line 491 "parserTaPyoCa.y"
        {
            pr("[print] print %s", (yyvsp[-1].alist)->texto);
            if ((yyvsp[-1].alist)->count == 0) {
                emit_c("printf(\"\\n\");");
            } else {
                char formato[512] = "";
                char args_c[2048] = "";
                int tem_erro = 0;

                for (int i = 0; i < (yyvsp[-1].alist)->count; i++) {
                    Tipo t = (yyvsp[-1].alist)->tipos[i];

                    if ((yyvsp[-1].alist)->is_ref[i]) {
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
                        strcat(args_c, (yyvsp[-1].alist)->c_itens[i]);
                    } else if (t.base == TIPO_STRING) {
                        strcat(formato, "%s");
                        strcat(args_c, (yyvsp[-1].alist)->c_itens[i]);
                    } else if (t.base == TIPO_BOOL) {
                        strcat(formato, "%s");
                        char *b_expr = fmt("(%s ? \"true\" : \"false\")", (yyvsp[-1].alist)->c_itens[i]);
                        strcat(args_c, b_expr);
                        free(b_expr);
                    } else {
                        strcat(formato, "%d");
                        strcat(args_c, (yyvsp[-1].alist)->c_itens[i]);
                    }
                }

                if (!tem_erro) {
                    emit_c("printf(\"%s\\n\", %s);", formato, args_c);
                }
            }
            free_lista_args((yyvsp[-1].alist));
        }
#line 2350 "y.tab.c"
    break;

  case 41: /* declaracaovar: tipo ID ASSIGN READ LPAREN RPAREN  */
#line 561 "parserTaPyoCa.y"
        {
            pr("[decl] %s %s <- read()", (yyvsp[-5].sval), (yyvsp[-4].sval));
            Tipo t_var = parse_tipo_string((yyvsp[-5].sval));
            if (inserir_variavel((yyvsp[-4].sval), t_var, yylineno) != NULL) {
                if (t_var.dimensao > 0 || t_var.base == TIPO_STRUCT) {
                    erro_semantico(yylineno, "Nao e possivel usar 'read()' diretamente em '%s'.", (yyvsp[-4].sval));
                } else {
                    char *t_c = tipo_para_c(t_var);
                    int l_ok = novo_label();
                    if (t_var.base == TIPO_INT) {
                        emit_c("%s %s;", t_c, (yyvsp[-4].sval));
                        emit_c("if( scanf(\"%%d\", &%s) == 1 ) goto L%d;", (yyvsp[-4].sval), l_ok);
                        emit_c("fprintf(stderr, \"Erro em tempo de execucao: esperado 'int'.\\n\");");
                        emit_c("exit(1);");
                        emit_c("L%d:;", l_ok);
                    } else if (t_var.base == TIPO_REAL) {
                        emit_c("%s %s;", t_c, (yyvsp[-4].sval));
                        emit_c("if( scanf(\"%%lf\", &%s) == 1 ) goto L%d;", (yyvsp[-4].sval), l_ok);
                        emit_c("fprintf(stderr, \"Erro em tempo de execucao: esperado 'real'.\\n\");");
                        emit_c("exit(1);");
                        emit_c("L%d:;", l_ok);
                    } else if (t_var.base == TIPO_STRING) {
                        emit_c("char *%s = malloc(256);", (yyvsp[-4].sval));
                        emit_c("if( scanf(\"%%255s\", %s) == 1 ) goto L%d;", (yyvsp[-4].sval), l_ok);
                        emit_c("fprintf(stderr, \"Erro em tempo de execucao: esperado 'string'.\\n\");");
                        emit_c("exit(1);");
                        emit_c("L%d:;", l_ok);
                    } else if (t_var.base == TIPO_BOOL) {
                        emit_c("%s %s;", t_c, (yyvsp[-4].sval));
                        emit_c("{ char _b[16]; if (scanf(\"%%15s\", _b) == 1) {");
                        emit_c("    if (strcmp(_b, \"true\") == 0 || strcmp(_b, \"1\") == 0) { %s = true; goto L%d; }", (yyvsp[-4].sval), l_ok);
                        emit_c("    if (strcmp(_b, \"false\") == 0 || strcmp(_b, \"0\") == 0) { %s = false; goto L%d; }", (yyvsp[-4].sval), l_ok);
                        emit_c("} }");
                        emit_c("fprintf(stderr, \"Erro em tempo de execucao: esperado 'bool'.\\n\");");
                        emit_c("exit(1);");
                        emit_c("L%d:;", l_ok);
                    }
                    free(t_c);
                }
            }
            free((yyvsp[-5].sval)); free((yyvsp[-4].sval));
        }
#line 2397 "y.tab.c"
    break;

  case 42: /* declaracaovar: tipo ID ASSIGN expressao  */
#line 604 "parserTaPyoCa.y"
        {
            pr("[decl] %s %s <- %s", (yyvsp[-3].sval), (yyvsp[-2].sval), (yyvsp[0].expr).texto);
            Tipo t_var = parse_tipo_string((yyvsp[-3].sval));
            if (inserir_variavel((yyvsp[-2].sval), t_var, yylineno) != NULL) {
                if (!tipos_compativeis(t_var, (yyvsp[0].expr).tipo)) {
                    erro_semantico(yylineno, "Nao e possivel inicializar '%s' (%s) com tipo '%s'.",
                                   (yyvsp[-2].sval), tipo_para_string(t_var), tipo_para_string((yyvsp[0].expr).tipo));
                } else {
                    char *t_c = tipo_para_c(t_var);
                    if (t_var.dimensao > 0 && (yyvsp[0].expr).texto[0] == '[') {
                        Tipo t_elem = criar_tipo(t_var.base, t_var.dimensao - 1);
                        if (t_var.base == TIPO_STRUCT)
                            strcpy(t_elem.nome_struct, t_var.nome_struct);
                        char *elem_c = tipo_para_c(t_elem);
                        char *init = strdup((yyvsp[0].expr).texto);
                        init[0] = '{';
                        init[strlen(init) - 1] = '}';
                        /* Aloca no heap para permitir realloc futuro com append() */
                        emit_c("%s _tmp_%s[] = %s;", elem_c, (yyvsp[-2].sval), init);
                        emit_c("int _len_%s = sizeof(_tmp_%s) / sizeof(%s);", (yyvsp[-2].sval), (yyvsp[-2].sval), elem_c);
                        emit_c("%s %s = malloc(sizeof(_tmp_%s));", t_c, (yyvsp[-2].sval), (yyvsp[-2].sval));
                        emit_c("memcpy(%s, _tmp_%s, sizeof(_tmp_%s));", (yyvsp[-2].sval), (yyvsp[-2].sval), (yyvsp[-2].sval));
                        free(elem_c); free(init);
                    } else if (t_var.dimensao > 0) {
                        emit_c("%s %s = %s;", t_c, (yyvsp[-2].sval), (yyvsp[0].expr).texto);
                        if (strchr((yyvsp[0].expr).texto, '(') == NULL && strchr((yyvsp[0].expr).texto, '[') == NULL) {
                            emit_c("int _len_%s = _len_%s;", (yyvsp[-2].sval), (yyvsp[0].expr).texto);
                        } else {
                            emit_c("int _len_%s = 0;", (yyvsp[-2].sval));
                        }
                    } else if ((yyvsp[0].expr).is_ref) {
                        emit_c("%s %s = *(%s);", t_c, (yyvsp[-2].sval), (yyvsp[0].expr).texto);
                    } else {
                        emit_c("%s %s = %s;", t_c, (yyvsp[-2].sval), (yyvsp[0].expr).texto);
                    }
                    free(t_c);
                }
            }
            free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[0].expr).texto);
        }
#line 2442 "y.tab.c"
    break;

  case 43: /* declaracaovar: tipo ID  */
#line 645 "parserTaPyoCa.y"
        {
            pr("[decl] %s %s", (yyvsp[-1].sval), (yyvsp[0].sval));
            Tipo t_var = parse_tipo_string((yyvsp[-1].sval));
            if (inserir_variavel((yyvsp[0].sval), t_var, yylineno) != NULL) {
                char *t_c = tipo_para_c(t_var);
                if (t_var.dimensao > 0) {
                    emit_c("%s %s = NULL;", t_c, (yyvsp[0].sval));
                    emit_c("int _len_%s = 0;", (yyvsp[0].sval));
                } else {
                    emit_c("%s %s;", t_c, (yyvsp[0].sval));
                }
                free(t_c);
            }
            free((yyvsp[-1].sval)); free((yyvsp[0].sval));
        }
#line 2462 "y.tab.c"
    break;

  case 44: /* declaracaovar: REF tipo ID ASSIGN REF l_value  */
#line 661 "parserTaPyoCa.y"
        {
            pr("[decl] ref %s %s <- ref %s", (yyvsp[-4].sval), (yyvsp[-3].sval), (yyvsp[0].expr).texto);
            Tipo t_var = parse_tipo_string((yyvsp[-4].sval));
            if (inserir_variavel_ref((yyvsp[-3].sval), t_var, 2, yylineno) != NULL) {
                if (!tipos_identicos(t_var, (yyvsp[0].expr).tipo)) {
                    erro_semantico(yylineno, "Referencia '%s' (%s) incompativel com '%s'.",
                                   (yyvsp[-3].sval), tipo_para_string(t_var), tipo_para_string((yyvsp[0].expr).tipo));
                } else {
                    char *t_c = tipo_para_c(t_var);
                    if ((yyvsp[0].expr).is_ref) emit_c("%s *%s = %s;", t_c, (yyvsp[-3].sval), (yyvsp[0].expr).texto);
                    else           emit_c("%s *%s = &(%s);", t_c, (yyvsp[-3].sval), (yyvsp[0].expr).texto);
                    free(t_c);
                }
            }
            free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[0].expr).texto);
        }
#line 2483 "y.tab.c"
    break;

  case 45: /* declaracaovar: REF tipo ID ASSIGN expressao  */
#line 678 "parserTaPyoCa.y"
        {
            pr("[decl] ref %s %s <- %s", (yyvsp[-3].sval), (yyvsp[-2].sval), (yyvsp[0].expr).texto);
            Tipo t_var = parse_tipo_string((yyvsp[-3].sval));
            if (inserir_variavel_ref((yyvsp[-2].sval), t_var, 2, yylineno) != NULL) {
                if ((yyvsp[0].expr).tipo.base == TIPO_NULL) {
                    char *t_c = tipo_para_c(t_var);
                    emit_c("%s *%s = NULL;", t_c, (yyvsp[-2].sval));
                    free(t_c);
                } else if (!(yyvsp[0].expr).is_ref || !tipos_identicos(t_var, (yyvsp[0].expr).tipo)) {
                    erro_semantico(yylineno, "Referencia '%s' (%s) exige 'ref' ou 'null'.",
                                   (yyvsp[-2].sval), tipo_para_string(t_var));
                } else {
                    char *t_c = tipo_para_c(t_var);
                    emit_c("%s *%s = %s;", t_c, (yyvsp[-2].sval), (yyvsp[0].expr).texto);
                    free(t_c);
                }
            }
            free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[0].expr).texto);
        }
#line 2507 "y.tab.c"
    break;

  case 46: /* declaracaovar: REF tipo ID  */
#line 698 "parserTaPyoCa.y"
        {
            pr("[decl] ref %s %s", (yyvsp[-1].sval), (yyvsp[0].sval));
            Tipo t_var = parse_tipo_string((yyvsp[-1].sval));
            if (inserir_variavel_ref((yyvsp[0].sval), t_var, 2, yylineno) != NULL) {
                char *t_c = tipo_para_c(t_var);
                emit_c("%s *%s = NULL;", t_c, (yyvsp[0].sval));
                free(t_c);
            }
            free((yyvsp[-1].sval)); free((yyvsp[0].sval));
        }
#line 2522 "y.tab.c"
    break;

  case 47: /* atribuicao: l_value ASSIGN READ LPAREN RPAREN  */
#line 712 "parserTaPyoCa.y"
        {
            pr("[atrib] %s <- read()", (yyvsp[-4].expr).texto);
            if ((yyvsp[-4].expr).tipo.base != TIPO_ERRO) {
                if ((yyvsp[-4].expr).tipo.dimensao > 0 || (yyvsp[-4].expr).tipo.base == TIPO_STRUCT) {
                    erro_semantico(yylineno, "Nao e possivel usar 'read()' diretamente em '%s'.", (yyvsp[-4].expr).texto);
                } else if ((yyvsp[-4].expr).tipo.base == TIPO_INT) {
                    int l_ok = novo_label();
                    emit_c("if( scanf(\"%%d\", &%s) == 1 ) goto L%d;", (yyvsp[-4].expr).texto, l_ok);
                    emit_c("fprintf(stderr, \"Erro em tempo de execucao: esperado 'int'.\\n\");");
                    emit_c("exit(1);");
                    emit_c("L%d:;", l_ok);
                } else if ((yyvsp[-4].expr).tipo.base == TIPO_REAL) {
                    int l_ok = novo_label();
                    emit_c("if( scanf(\"%%lf\", &%s) == 1 ) goto L%d;", (yyvsp[-4].expr).texto, l_ok);
                    emit_c("fprintf(stderr, \"Erro em tempo de execucao: esperado 'real'.\\n\");");
                    emit_c("exit(1);");
                    emit_c("L%d:;", l_ok);
                } else if ((yyvsp[-4].expr).tipo.base == TIPO_STRING) {
                    int l_ok = novo_label();
                    emit_c("%s = malloc(256);", (yyvsp[-4].expr).texto);
                    emit_c("if( scanf(\"%%255s\", %s) == 1 ) goto L%d;", (yyvsp[-4].expr).texto, l_ok);
                    emit_c("fprintf(stderr, \"Erro em tempo de execucao: esperado 'string'.\\n\");");
                    emit_c("exit(1);");
                    emit_c("L%d:;", l_ok);
                } else if ((yyvsp[-4].expr).tipo.base == TIPO_BOOL) {
                    int l_ok = novo_label();
                    emit_c("{ char _b[16]; if (scanf(\"%%15s\", _b) == 1) {");
                    emit_c("    if (strcmp(_b, \"true\") == 0 || strcmp(_b, \"1\") == 0) { %s = true; goto L%d; }", (yyvsp[-4].expr).texto, l_ok);
                    emit_c("    if (strcmp(_b, \"false\") == 0 || strcmp(_b, \"0\") == 0) { %s = false; goto L%d; }", (yyvsp[-4].expr).texto, l_ok);
                    emit_c("} }");
                    emit_c("fprintf(stderr, \"Erro em tempo de execucao: esperado 'bool'.\\n\");");
                    emit_c("exit(1);");
                    emit_c("L%d:;", l_ok);
                }
            }
            free((yyvsp[-4].expr).texto);
        }
#line 2564 "y.tab.c"
    break;

  case 48: /* atribuicao: l_value ASSIGN REF l_value  */
#line 750 "parserTaPyoCa.y"
        {
            pr("[atrib] %s <- ref %s", (yyvsp[-3].expr).texto, (yyvsp[0].expr).texto);
            if ((yyvsp[-3].expr).tipo.base != TIPO_ERRO && (yyvsp[0].expr).tipo.base != TIPO_ERRO) {
                if (!(yyvsp[-3].expr).is_ref) {
                    erro_semantico(yylineno, "'%s' nao e uma referencia ('ref') e nao pode receber 'ref %s'.",
                                   (yyvsp[-3].expr).texto, (yyvsp[0].expr).texto);
                } else if (!tipos_identicos((yyvsp[-3].expr).tipo, (yyvsp[0].expr).tipo)) {
                    erro_semantico(yylineno, "Referencia incompativel: '%s' exige '%s', recebeu '%s'.",
                                   (yyvsp[-3].expr).texto, tipo_para_string((yyvsp[-3].expr).tipo), tipo_para_string((yyvsp[0].expr).tipo));
                } else if ((yyvsp[0].expr).is_ref) {
                    emit_c("%s = %s;", (yyvsp[-3].expr).texto, (yyvsp[0].expr).texto);
                } else {
                    emit_c("%s = &(%s);", (yyvsp[-3].expr).texto, (yyvsp[0].expr).texto);
                }
            }
            free((yyvsp[-3].expr).texto); free((yyvsp[0].expr).texto);
        }
#line 2586 "y.tab.c"
    break;

  case 49: /* atribuicao: l_value ASSIGN expressao  */
#line 768 "parserTaPyoCa.y"
        {
            pr("[atrib] %s <- %s", (yyvsp[-2].expr).texto, (yyvsp[0].expr).texto);
            if ((yyvsp[-2].expr).tipo.base != TIPO_ERRO && (yyvsp[0].expr).tipo.base != TIPO_ERRO) {
                if ((yyvsp[-2].expr).is_ref) {
                    if ((yyvsp[0].expr).tipo.base == TIPO_NULL) {
                        emit_c("%s = NULL;", (yyvsp[-2].expr).texto);
                    } else if ((yyvsp[0].expr).is_ref && tipos_identicos((yyvsp[-2].expr).tipo, (yyvsp[0].expr).tipo)) {
                        emit_c("%s = %s;", (yyvsp[-2].expr).texto, (yyvsp[0].expr).texto);
                    } else {
                        erro_semantico(yylineno, "Atribuicao invalida para referencia '%s': use 'ref' ou 'null'.",
                                       (yyvsp[-2].expr).texto);
                    }
                } else if (!tipos_compativeis((yyvsp[-2].expr).tipo, (yyvsp[0].expr).tipo)) {
                    erro_semantico(yylineno, "Atribuicao incompativel: '%s' espera '%s', mas recebeu '%s'.",
                                   (yyvsp[-2].expr).texto, tipo_para_string((yyvsp[-2].expr).tipo), tipo_para_string((yyvsp[0].expr).tipo));
                } else if ((yyvsp[0].expr).is_ref) {
                    emit_c("%s = *(%s);", (yyvsp[-2].expr).texto, (yyvsp[0].expr).texto);
                } else {
                    emit_c("%s = %s;", (yyvsp[-2].expr).texto, (yyvsp[0].expr).texto);
                }
            }
            free((yyvsp[-2].expr).texto); free((yyvsp[0].expr).texto);
        }
#line 2614 "y.tab.c"
    break;

  case 50: /* $@4: %empty  */
#line 795 "parserTaPyoCa.y"
        {
            char *label_inicio = new_label();
            char *label_fim = new_label();

            pr("[while] while (%s)", (yyvsp[-1].expr).texto);

            /* Valida a condicao do while */
            if ((yyvsp[-1].expr).tipo.base != TIPO_ERRO &&
                ((yyvsp[-1].expr).tipo.base != TIPO_BOOL || (yyvsp[-1].expr).tipo.dimensao != 0)) {
                erro_semantico(yylineno,
                    "Condicao do 'while' deve ser booleana ('bool'), recebeu '%s'.",
                    tipo_para_string((yyvsp[-1].expr).tipo));
            }

            /* Armazena as labels na pilha */
            push_loop(label_inicio, label_fim);

            /* Gera a label de inicio */
            emit_c("%s:", label_inicio);

            /* Verifica a condicao e permite sair do laco */
            emit_c("if( !(%s) ) goto %s;", (yyvsp[-1].expr).texto, label_fim);

            /* Abre um bloco para preservar o escopo */
            emit_c("{");

            depth++;
            loop_depth++;

            entrar_escopo();

            free((yyvsp[-1].expr).texto);
        }
#line 2652 "y.tab.c"
    break;

  case 51: /* comandowhile: WHILE LPAREN expressao RPAREN $@4 NEWLINE corpo_bloco ENDWHILE  */
#line 829 "parserTaPyoCa.y"
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
#line 2677 "y.tab.c"
    break;

  case 52: /* $@5: %empty  */
#line 853 "parserTaPyoCa.y"
        {
            char *label_else = new_label();
            char *label_fim  = new_label();

            pr("[if] if (%s)", (yyvsp[-1].expr).texto);

            /* Valida a condicao */
            if ((yyvsp[-1].expr).tipo.base != TIPO_ERRO &&
                ((yyvsp[-1].expr).tipo.base != TIPO_BOOL || (yyvsp[-1].expr).tipo.dimensao != 0)) {
                erro_semantico(yylineno,
                    "Condicao do 'if' deve ser booleana ('bool'), recebeu '%s'.",
                    tipo_para_string((yyvsp[-1].expr).tipo));
            }

            /* Armazena as labels na pilha */
            push_if(label_else, label_fim);

            /* Desvia para o else (ou para o fim, se nao houver else) quando falso */
            emit_c("if( !(%s) ) goto %s;", (yyvsp[-1].expr).texto, label_else);

            /* Abre um bloco para preservar o escopo do ramo 'if' */
            emit_c("{");

            depth++;
            entrar_escopo();

            free((yyvsp[-1].expr).texto);
        }
#line 2710 "y.tab.c"
    break;

  case 54: /* cauda_if: ENDIF  */
#line 886 "parserTaPyoCa.y"
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
#line 2728 "y.tab.c"
    break;

  case 55: /* $@6: %empty  */
#line 900 "parserTaPyoCa.y"
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
#line 2749 "y.tab.c"
    break;

  case 56: /* cauda_if: ELSE $@6 NEWLINE corpo_bloco ENDIF  */
#line 917 "parserTaPyoCa.y"
        {
            sair_escopo();
            depth--;

            /* Fecha o bloco do ramo 'else' e emite a label final */
            emit_c("}");
            emit_c("%s: ;", if_stack[if_top].label_fim);

            pop_if();
            pr("[endif]");
        }
#line 2765 "y.tab.c"
    break;

  case 57: /* comandobreak: BREAK  */
#line 932 "parserTaPyoCa.y"
        {
            pr("[break]");

            if (loop_depth == 0) {
                erro_semantico(yylineno,
                    "Comando 'break' so pode ser utilizado dentro de um while.");
            } else {
                emit_c("goto %s;", loop_stack[loop_top].fim);
            }
        }
#line 2780 "y.tab.c"
    break;

  case 58: /* retorno: RETURN expressao  */
#line 946 "parserTaPyoCa.y"
        {
            pr("[return] %s", (yyvsp[0].expr).texto);
            encontrou_return = 1;
            if (retorno_esperado.base == TIPO_VOID) {
                erro_semantico(yylineno, "Subrotina sem tipo de retorno (ou main) nao deve retornar valor.");
            } else if (!tipos_compativeis(retorno_esperado, (yyvsp[0].expr).tipo)) {
                erro_semantico(yylineno, "Tipo de retorno incompativel: esperado '%s', obtido '%s'.",
                               tipo_para_string(retorno_esperado), tipo_para_string((yyvsp[0].expr).tipo));
            } else {
                emit_c("return %s;", (yyvsp[0].expr).texto);
            }
            free((yyvsp[0].expr).texto);
        }
#line 2798 "y.tab.c"
    break;

  case 59: /* chamadafuncao: ID LPAREN STRUCT_ID RPAREN  */
#line 963 "parserTaPyoCa.y"
        {
            if (strcmp((yyvsp[-3].sval), "create") != 0) {
                erro_semantico(yylineno, "Subrotina '%s' nao pode receber o tipo '%s' como argumento.", (yyvsp[-3].sval), (yyvsp[-1].sval));
                (yyval.expr).tipo = criar_tipo(TIPO_ERRO, 0);
                (yyval.expr).texto = strdup("NULL");
                (yyval.expr).is_ref = 0;
            } else {
                (yyval.expr).tipo = criar_tipo_struct((yyvsp[-1].sval), 0);
                (yyval.expr).texto = fmt("calloc(1, sizeof(struct %s))", (yyvsp[-1].sval));
                (yyval.expr).is_ref = 1;
            }
            free((yyvsp[-3].sval));
            free((yyvsp[-1].sval));
        }
#line 2817 "y.tab.c"
    break;

  case 60: /* chamadafuncao: ID LPAREN argumentos_opt RPAREN  */
#line 978 "parserTaPyoCa.y"
        {
            /* 1. Built-in: comprimento(array) -> int */
            if (strcmp((yyvsp[-3].sval), "comprimento") == 0) {
                (yyval.expr).is_ref = 0;
                if ((yyvsp[-1].alist)->count != 1) {
                    erro_semantico(yylineno, "'comprimento' espera 1 argumento, recebeu %d.", (yyvsp[-1].alist)->count);
                    (yyval.expr).tipo = criar_tipo(TIPO_ERRO, 0);
                    (yyval.expr).texto = strdup("0");
                } else if ((yyvsp[-1].alist)->is_ref[0]) {
                    erro_semantico(yylineno, "'comprimento' nao aceita argumento por referencia ('ref').");
                    (yyval.expr).tipo = criar_tipo(TIPO_ERRO, 0);
                    (yyval.expr).texto = strdup("0");
                } else if ((yyvsp[-1].alist)->tipos[0].dimensao <= 0 && (yyvsp[-1].alist)->tipos[0].base != TIPO_ERRO) {
                    erro_semantico(yylineno, "'comprimento' espera um array, recebeu '%s'.",
                                   tipo_para_string((yyvsp[-1].alist)->tipos[0]));
                    (yyval.expr).tipo = criar_tipo(TIPO_ERRO, 0);
                    (yyval.expr).texto = strdup("0");
                } else {
                    (yyval.expr).tipo = criar_tipo(TIPO_INT, 0);
                    const char *nome_raw = (yyvsp[-1].alist)->nomes[0];
                    Simbolo *s_arr = buscar_simbolo(nome_raw);
                    if (s_arr && s_arr->is_ref) {
                        (yyval.expr).texto = fmt("(*_len_%s)", nome_raw);
                    } else {
                        (yyval.expr).texto = fmt("_len_%s", nome_raw);
                    }
                }
            }
            /* 2. Built-in: swap(a, b) -> void */
            else if (strcmp((yyvsp[-3].sval), "swap") == 0) {
                (yyval.expr).is_ref = 0;
                if ((yyvsp[-1].alist)->count != 2) {
                    erro_semantico(yylineno, "'swap' espera 2 argumentos, recebeu %d.", (yyvsp[-1].alist)->count);
                } else if (!tipos_identicos((yyvsp[-1].alist)->tipos[0], (yyvsp[-1].alist)->tipos[1])) {
                    erro_semantico(yylineno, "'swap' requer argumentos exatamente do mesmo tipo, recebeu '%s' e '%s'.",
                                   tipo_para_string((yyvsp[-1].alist)->tipos[0]), tipo_para_string((yyvsp[-1].alist)->tipos[1]));
                }
                (yyval.expr).tipo = criar_tipo(TIPO_VOID, 0);
                (yyval.expr).texto = fmt("%s(%s)", (yyvsp[-3].sval), (yyvsp[-1].alist)->c_texto);
            }
            /* 3. Built-in: append(ref array, elemento) -> void */
            else if (strcmp((yyvsp[-3].sval), "append") == 0) {
                (yyval.expr).is_ref = 0;
                (yyval.expr).tipo = criar_tipo(TIPO_VOID, 0);
                if ((yyvsp[-1].alist)->count != 2) {
                    erro_semantico(yylineno, "'append' espera 2 argumentos (ref array, elemento), recebeu %d.", (yyvsp[-1].alist)->count);
                    (yyval.expr).texto = strdup("");
                } else if (!(yyvsp[-1].alist)->is_ref[0]) {
                    erro_semantico(yylineno, "O primeiro argumento de 'append' deve ser passado por referencia ('ref').");
                    (yyval.expr).texto = strdup("");
                } else if ((yyvsp[-1].alist)->tipos[0].base != TIPO_ERRO && (yyvsp[-1].alist)->tipos[0].dimensao <= 0) {
                    erro_semantico(yylineno, "O primeiro argumento de 'append' deve ser um array, recebeu '%s'.",
                                   tipo_para_string((yyvsp[-1].alist)->tipos[0]));
                    (yyval.expr).texto = strdup("");
                } else if ((yyvsp[-1].alist)->is_ref[1]) {
                    erro_semantico(yylineno, "O segundo argumento de 'append' nao pode ser passado por referencia ('ref').");
                    (yyval.expr).texto = strdup("");
                } else {
                    Tipo elem_esperado = criar_tipo((yyvsp[-1].alist)->tipos[0].base, (yyvsp[-1].alist)->tipos[0].dimensao - 1);
                    if ((yyvsp[-1].alist)->tipos[0].base == TIPO_STRUCT) {
                        strcpy(elem_esperado.nome_struct, (yyvsp[-1].alist)->tipos[0].nome_struct);
                    }
                    if (!tipos_compativeis(elem_esperado, (yyvsp[-1].alist)->tipos[1])) {
                        erro_semantico(yylineno, "Elemento incompativel em 'append': esperado '%s', recebeu '%s'.",
                                       tipo_para_string(elem_esperado), tipo_para_string((yyvsp[-1].alist)->tipos[1]));
                        (yyval.expr).texto = strdup("");
                    } else if ((yyvsp[-1].alist)->tipos[0].base != TIPO_ERRO) {
                        const char *nome_raw = (yyvsp[-1].alist)->nomes[0];
                        Simbolo *s_arr = buscar_simbolo(nome_raw);
                        if (s_arr && s_arr->is_ref) {
                            (yyval.expr).texto = fmt("do { (*%s) = realloc((*%s), ((*_len_%s) + 1) * sizeof(*(*%s))); (*%s)[(*_len_%s)++] = %s; } while(0)",
                                           nome_raw, nome_raw, nome_raw, nome_raw, nome_raw, nome_raw, (yyvsp[-1].alist)->c_itens[1]);
                        } else {
                            (yyval.expr).texto = fmt("do { %s = realloc(%s, (_len_%s + 1) * sizeof(*%s)); %s[_len_%s++] = %s; } while(0)",
                                           nome_raw, nome_raw, nome_raw, nome_raw, nome_raw, nome_raw, (yyvsp[-1].alist)->c_itens[1]);
                        }
                    } else {
                        (yyval.expr).texto = strdup("");
                    }
                }
            }
            /* 4. Uso incorreto de create() sem nome de struct */
            else if (strcmp((yyvsp[-3].sval), "create") == 0) {
                erro_semantico(yylineno, "'create' espera o nome de uma struct declarada (ex: create(node)).");
                (yyval.expr).tipo = criar_tipo(TIPO_ERRO, 0);
                (yyval.expr).texto = strdup("NULL");
                (yyval.expr).is_ref = 0;
            }
            /* 5. Demais subrotinas definidas pelo usuario */
            else {
                (yyval.expr).is_ref = 0;
                (yyval.expr).texto = fmt("%s(%s)", (yyvsp[-3].sval), (yyvsp[-1].alist)->c_texto);
                Simbolo *s = buscar_simbolo((yyvsp[-3].sval));
                if (!s) {
                    erro_semantico(yylineno, "Subrotina '%s' nao declarada.", (yyvsp[-3].sval));
                    (yyval.expr).tipo = criar_tipo(TIPO_ERRO, 0);
                } else if (!s->is_func) {
                    erro_semantico(yylineno, "'%s' e uma variavel e nao pode ser chamada como subrotina.", (yyvsp[-3].sval));
                    (yyval.expr).tipo = criar_tipo(TIPO_ERRO, 0);
                } else {
                    if (s->num_params != (yyvsp[-1].alist)->count) {
                        erro_semantico(yylineno, "Subrotina '%s' espera %d argumento(s), recebeu %d.",
                                       (yyvsp[-3].sval), s->num_params, (yyvsp[-1].alist)->count);
                    } else {
                        for (int i = 0; i < s->num_params; i++) {
                            int param_ref = s->params_ref ? s->params_ref[i] : 0;
                            int arg_ref   = (yyvsp[-1].alist)->is_ref[i];

                            if (param_ref && !arg_ref) {
                                erro_semantico(yylineno, "Argumento %d de '%s' deve ser passado por referencia ('ref').",
                                               i + 1, (yyvsp[-3].sval));
                            } else if (!param_ref && arg_ref) {
                                erro_semantico(yylineno, "Argumento %d de '%s' nao aceita passagem por referencia ('ref').",
                                               i + 1, (yyvsp[-3].sval));
                            } else if (param_ref) {
                                /* Ponteiros em C exigem tipo identico (inclusive nome da struct) */
                                if (!tipos_identicos(s->params[i], (yyvsp[-1].alist)->tipos[i])) {
                                    erro_semantico(yylineno, "Argumento %d por referencia de '%s' exige tipo exato '%s', recebido '%s'.",
                                                   i + 1, (yyvsp[-3].sval), tipo_para_string(s->params[i]), tipo_para_string((yyvsp[-1].alist)->tipos[i]));
                                }
                            } else if (!tipos_compativeis(s->params[i], (yyvsp[-1].alist)->tipos[i])) {
                                erro_semantico(yylineno, "Argumento %d de '%s' invalido: esperado '%s', recebido '%s'.",
                                               i + 1, (yyvsp[-3].sval), tipo_para_string(s->params[i]), tipo_para_string((yyvsp[-1].alist)->tipos[i]));
                            }
                        }
                    }
                    (yyval.expr).tipo = s->tipo;
                }
            }
            free((yyvsp[-3].sval));
            free_lista_args((yyvsp[-1].alist));
        }
#line 2954 "y.tab.c"
    break;

  case 61: /* argumentos_opt: lista_argumentos  */
#line 1113 "parserTaPyoCa.y"
                       { (yyval.alist) = (yyvsp[0].alist); }
#line 2960 "y.tab.c"
    break;

  case 62: /* argumentos_opt: %empty  */
#line 1115 "parserTaPyoCa.y"
        {
            ListaArgs *la = calloc(1, sizeof(ListaArgs));
            la->count = 0;
            la->texto = strdup("");
            la->c_texto = strdup("");
            (yyval.alist) = la;
        }
#line 2972 "y.tab.c"
    break;

  case 63: /* lista_argumentos: lista_argumentos COMMA REF l_value  */
#line 1126 "parserTaPyoCa.y"
        {
            if ((yyvsp[-3].alist)->count < 32) {
                int idx = (yyvsp[-3].alist)->count++;
                (yyvsp[-3].alist)->tipos[idx] = (yyvsp[0].expr).tipo;
                (yyvsp[-3].alist)->is_ref[idx] = 1;
                (yyvsp[-3].alist)->nomes[idx] = strdup((yyvsp[0].expr).texto);
                (yyvsp[-3].alist)->c_itens[idx] = (yyvsp[0].expr).is_ref ? strdup((yyvsp[0].expr).texto) : fmt("&(%s)", (yyvsp[0].expr).texto);
                char *novo_t = fmt("%s, ref %s", (yyvsp[-3].alist)->texto, (yyvsp[0].expr).texto);
                char *novo_c = ((yyvsp[0].expr).tipo.dimensao > 0)
                    ? fmt("%s, &(%s), &(_len_%s)", (yyvsp[-3].alist)->c_texto, (yyvsp[0].expr).texto, (yyvsp[0].expr).texto)
                    : fmt("%s, %s", (yyvsp[-3].alist)->c_texto, (yyvsp[-3].alist)->c_itens[idx]);
                free((yyvsp[-3].alist)->texto); free((yyvsp[-3].alist)->c_texto);
                (yyvsp[-3].alist)->texto = novo_t;
                (yyvsp[-3].alist)->c_texto = novo_c;
            } else {
                erro_semantico(yylineno, "Limite maximo de 32 argumentos/elementos excedido.");
            }
            free((yyvsp[0].expr).texto);
            (yyval.alist) = (yyvsp[-3].alist);
        }
#line 2997 "y.tab.c"
    break;

  case 64: /* lista_argumentos: REF l_value  */
#line 1147 "parserTaPyoCa.y"
        {
            ListaArgs *la = calloc(1, sizeof(ListaArgs));
            la->tipos[0] = (yyvsp[0].expr).tipo;
            la->is_ref[0] = 1;
            la->nomes[0] = strdup((yyvsp[0].expr).texto);
            la->c_itens[0] = (yyvsp[0].expr).is_ref ? strdup((yyvsp[0].expr).texto) : fmt("&(%s)", (yyvsp[0].expr).texto);
            la->count = 1;
            la->texto = fmt("ref %s", (yyvsp[0].expr).texto);
            la->c_texto = ((yyvsp[0].expr).tipo.dimensao > 0)
                ? fmt("&(%s), &(_len_%s)", (yyvsp[0].expr).texto, (yyvsp[0].expr).texto)
                : strdup(la->c_itens[0]);
            free((yyvsp[0].expr).texto);
            (yyval.alist) = la;
        }
#line 3016 "y.tab.c"
    break;

  case 65: /* lista_argumentos: lista_argumentos COMMA expressao  */
#line 1162 "parserTaPyoCa.y"
        {
            if ((yyvsp[-2].alist)->count < 32) {
                int idx = (yyvsp[-2].alist)->count++;
                (yyvsp[-2].alist)->tipos[idx] = (yyvsp[0].expr).tipo;
                (yyvsp[-2].alist)->is_ref[idx] = 0;
                (yyvsp[-2].alist)->nomes[idx] = strdup((yyvsp[0].expr).texto);
                (yyvsp[-2].alist)->c_itens[idx] = strdup((yyvsp[0].expr).texto);
                char *novo_t = fmt("%s, %s", (yyvsp[-2].alist)->texto, (yyvsp[0].expr).texto);
                char *novo_c = ((yyvsp[0].expr).tipo.dimensao > 0)
                    ? fmt("%s, %s, _len_%s", (yyvsp[-2].alist)->c_texto, (yyvsp[0].expr).texto, (yyvsp[0].expr).texto)
                    : fmt("%s, %s", (yyvsp[-2].alist)->c_texto, (yyvsp[0].expr).texto);
                free((yyvsp[-2].alist)->texto); free((yyvsp[-2].alist)->c_texto);
                (yyvsp[-2].alist)->texto = novo_t;
                (yyvsp[-2].alist)->c_texto = novo_c;
            } else {
                erro_semantico(yylineno, "Limite maximo de 32 argumentos/elementos excedido.");
            }
            free((yyvsp[0].expr).texto);
            (yyval.alist) = (yyvsp[-2].alist);
        }
#line 3041 "y.tab.c"
    break;

  case 66: /* lista_argumentos: expressao  */
#line 1183 "parserTaPyoCa.y"
        {
            ListaArgs *la = calloc(1, sizeof(ListaArgs));
            la->tipos[0] = (yyvsp[0].expr).tipo;
            la->is_ref[0] = 0;
            la->nomes[0] = strdup((yyvsp[0].expr).texto);
            la->c_itens[0] = strdup((yyvsp[0].expr).texto);
            la->count = 1;
            la->texto = strdup((yyvsp[0].expr).texto);
            la->c_texto = ((yyvsp[0].expr).tipo.dimensao > 0)
                ? fmt("%s, _len_%s", (yyvsp[0].expr).texto, (yyvsp[0].expr).texto)
                : strdup((yyvsp[0].expr).texto);
            free((yyvsp[0].expr).texto);
            (yyval.alist) = la;
        }
#line 3060 "y.tab.c"
    break;

  case 67: /* tipo: INT  */
#line 1202 "parserTaPyoCa.y"
                               { (yyval.sval) = strdup("int"); }
#line 3066 "y.tab.c"
    break;

  case 68: /* tipo: REAL  */
#line 1203 "parserTaPyoCa.y"
                               { (yyval.sval) = strdup("real"); }
#line 3072 "y.tab.c"
    break;

  case 69: /* tipo: STRING  */
#line 1204 "parserTaPyoCa.y"
                               { (yyval.sval) = strdup("string"); }
#line 3078 "y.tab.c"
    break;

  case 70: /* tipo: BOOL  */
#line 1205 "parserTaPyoCa.y"
                               { (yyval.sval) = strdup("bool"); }
#line 3084 "y.tab.c"
    break;

  case 71: /* tipo: STRUCT_ID  */
#line 1206 "parserTaPyoCa.y"
                               { (yyval.sval) = (yyvsp[0].sval); }
#line 3090 "y.tab.c"
    break;

  case 72: /* tipo: tipo LBRACKET RBRACKET  */
#line 1207 "parserTaPyoCa.y"
                               { (yyval.sval) = fmt("%s[]", (yyvsp[-2].sval)); free((yyvsp[-2].sval)); }
#line 3096 "y.tab.c"
    break;

  case 73: /* expressao: explogica  */
#line 1213 "parserTaPyoCa.y"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 3102 "y.tab.c"
    break;

  case 74: /* explogica: explogica OR exp_and  */
#line 1218 "parserTaPyoCa.y"
        {
            (yyval.expr).texto = fmt("%s || %s", (yyvsp[-2].expr).texto, (yyvsp[0].expr).texto);
            (yyval.expr).tipo = verificar_binario_logico((yyvsp[-2].expr).tipo, (yyvsp[0].expr).tipo, "||", yylineno);
            (yyval.expr).is_ref = 0;
            free((yyvsp[-2].expr).texto); free((yyvsp[0].expr).texto);
        }
#line 3113 "y.tab.c"
    break;

  case 75: /* explogica: exp_and  */
#line 1224 "parserTaPyoCa.y"
              { (yyval.expr) = (yyvsp[0].expr); }
#line 3119 "y.tab.c"
    break;

  case 76: /* exp_and: exp_and AND exp_not  */
#line 1229 "parserTaPyoCa.y"
        {
            (yyval.expr).texto = fmt("%s && %s", (yyvsp[-2].expr).texto, (yyvsp[0].expr).texto);
            (yyval.expr).tipo = verificar_binario_logico((yyvsp[-2].expr).tipo, (yyvsp[0].expr).tipo, "&&", yylineno);
            (yyval.expr).is_ref = 0;
            free((yyvsp[-2].expr).texto); free((yyvsp[0].expr).texto);
        }
#line 3130 "y.tab.c"
    break;

  case 77: /* exp_and: exp_not  */
#line 1235 "parserTaPyoCa.y"
              { (yyval.expr) = (yyvsp[0].expr); }
#line 3136 "y.tab.c"
    break;

  case 78: /* exp_not: NOT exp_not  */
#line 1240 "parserTaPyoCa.y"
        {
            (yyval.expr).texto = fmt("!(%s)", (yyvsp[0].expr).texto);
            (yyval.expr).tipo = verificar_unario_logico((yyvsp[0].expr).tipo, "!", yylineno);
            (yyval.expr).is_ref = 0;
            free((yyvsp[0].expr).texto);
        }
#line 3147 "y.tab.c"
    break;

  case 79: /* exp_not: exprelacional  */
#line 1246 "parserTaPyoCa.y"
                    { (yyval.expr) = (yyvsp[0].expr); }
#line 3153 "y.tab.c"
    break;

  case 80: /* exprelacional: exparitimetica op_relacional exparitimetica  */
#line 1251 "parserTaPyoCa.y"
        {
            (yyval.expr).texto = fmt("%s %s %s", (yyvsp[-2].expr).texto, (yyvsp[-1].sval), (yyvsp[0].expr).texto);
            (yyval.expr).is_ref = 0;
            if (((yyvsp[-2].expr).is_ref || (yyvsp[0].expr).is_ref) &&
                (strcmp((yyvsp[-1].sval), "==") == 0 || strcmp((yyvsp[-1].sval), "!=") == 0) &&
                ((yyvsp[-2].expr).tipo.base == TIPO_NULL || (yyvsp[0].expr).tipo.base == TIPO_NULL ||
                 ((yyvsp[-2].expr).is_ref && (yyvsp[0].expr).is_ref && tipos_identicos((yyvsp[-2].expr).tipo, (yyvsp[0].expr).tipo)))) {
                (yyval.expr).tipo = criar_tipo(TIPO_BOOL, 0);
            } else {
                (yyval.expr).tipo = verificar_relacional((yyvsp[-2].expr).tipo, (yyvsp[0].expr).tipo, (yyvsp[-1].sval), yylineno);
            }
            free((yyvsp[-2].expr).texto); free((yyvsp[-1].sval)); free((yyvsp[0].expr).texto);
        }
#line 3171 "y.tab.c"
    break;

  case 81: /* exprelacional: exparitimetica  */
#line 1264 "parserTaPyoCa.y"
                     { (yyval.expr) = (yyvsp[0].expr); }
#line 3177 "y.tab.c"
    break;

  case 82: /* op_relacional: LT  */
#line 1268 "parserTaPyoCa.y"
           { (yyval.sval) = strdup("<");  }
#line 3183 "y.tab.c"
    break;

  case 83: /* op_relacional: LEQ  */
#line 1269 "parserTaPyoCa.y"
           { (yyval.sval) = strdup("<="); }
#line 3189 "y.tab.c"
    break;

  case 84: /* op_relacional: GT  */
#line 1270 "parserTaPyoCa.y"
           { (yyval.sval) = strdup(">");  }
#line 3195 "y.tab.c"
    break;

  case 85: /* op_relacional: GEQ  */
#line 1271 "parserTaPyoCa.y"
           { (yyval.sval) = strdup(">="); }
#line 3201 "y.tab.c"
    break;

  case 86: /* op_relacional: EQ  */
#line 1272 "parserTaPyoCa.y"
           { (yyval.sval) = strdup("=="); }
#line 3207 "y.tab.c"
    break;

  case 87: /* op_relacional: NEQ  */
#line 1273 "parserTaPyoCa.y"
           { (yyval.sval) = strdup("!="); }
#line 3213 "y.tab.c"
    break;

  case 88: /* exparitimetica: exparitimetica PLUS termo  */
#line 1278 "parserTaPyoCa.y"
        {
            (yyval.expr).texto = fmt("%s + %s", (yyvsp[-2].expr).texto, (yyvsp[0].expr).texto);
            (yyval.expr).tipo = verificar_binario_aritmetico((yyvsp[-2].expr).tipo, (yyvsp[0].expr).tipo, "+", yylineno);
            (yyval.expr).is_ref = 0;
            free((yyvsp[-2].expr).texto); free((yyvsp[0].expr).texto);
        }
#line 3224 "y.tab.c"
    break;

  case 89: /* exparitimetica: exparitimetica MINUS termo  */
#line 1285 "parserTaPyoCa.y"
        {
            (yyval.expr).texto = fmt("%s - %s", (yyvsp[-2].expr).texto, (yyvsp[0].expr).texto);
            (yyval.expr).tipo = verificar_binario_aritmetico((yyvsp[-2].expr).tipo, (yyvsp[0].expr).tipo, "-", yylineno);
            (yyval.expr).is_ref = 0;
            free((yyvsp[-2].expr).texto); free((yyvsp[0].expr).texto);
        }
#line 3235 "y.tab.c"
    break;

  case 90: /* exparitimetica: termo  */
#line 1291 "parserTaPyoCa.y"
            { (yyval.expr) = (yyvsp[0].expr); }
#line 3241 "y.tab.c"
    break;

  case 91: /* termo: termo MUL unario  */
#line 1296 "parserTaPyoCa.y"
        {
            (yyval.expr).texto = fmt("%s * %s", (yyvsp[-2].expr).texto, (yyvsp[0].expr).texto);
            (yyval.expr).tipo = verificar_binario_aritmetico((yyvsp[-2].expr).tipo, (yyvsp[0].expr).tipo, "*", yylineno);
            (yyval.expr).is_ref = 0;
            free((yyvsp[-2].expr).texto); free((yyvsp[0].expr).texto);
        }
#line 3252 "y.tab.c"
    break;

  case 92: /* termo: termo DIV unario  */
#line 1303 "parserTaPyoCa.y"
        {
            (yyval.expr).texto = fmt("%s / %s", (yyvsp[-2].expr).texto, (yyvsp[0].expr).texto);
            (yyval.expr).tipo = verificar_binario_aritmetico((yyvsp[-2].expr).tipo, (yyvsp[0].expr).tipo, "/", yylineno);
            (yyval.expr).is_ref = 0;
            free((yyvsp[-2].expr).texto); free((yyvsp[0].expr).texto);
        }
#line 3263 "y.tab.c"
    break;

  case 93: /* termo: unario  */
#line 1309 "parserTaPyoCa.y"
             { (yyval.expr) = (yyvsp[0].expr); }
#line 3269 "y.tab.c"
    break;

  case 94: /* unario: MINUS unario  */
#line 1314 "parserTaPyoCa.y"
        {
            (yyval.expr).tipo = verificar_unario_aritmetico((yyvsp[0].expr).tipo, "-", yylineno);
            (yyval.expr).texto = fmt("-(%s)", (yyvsp[0].expr).texto);
            (yyval.expr).is_ref = 0;
            free((yyvsp[0].expr).texto);
        }
#line 3280 "y.tab.c"
    break;

  case 95: /* unario: PLUS unario  */
#line 1321 "parserTaPyoCa.y"
        {
            (yyval.expr).tipo = verificar_unario_aritmetico((yyvsp[0].expr).tipo, "+", yylineno);
            (yyval.expr).texto = fmt("+(%s)", (yyvsp[0].expr).texto);
            (yyval.expr).is_ref = 0;
            free((yyvsp[0].expr).texto);
        }
#line 3291 "y.tab.c"
    break;

  case 96: /* unario: potencia  */
#line 1327 "parserTaPyoCa.y"
               { (yyval.expr) = (yyvsp[0].expr); }
#line 3297 "y.tab.c"
    break;

  case 97: /* potencia: fator POW unario  */
#line 1332 "parserTaPyoCa.y"
        {
            (yyval.expr).tipo = verificar_binario_aritmetico((yyvsp[-2].expr).tipo, (yyvsp[0].expr).tipo, "**", yylineno);
            (yyval.expr).is_ref = 0;
            if ((yyval.expr).tipo.base == TIPO_INT) {
                (yyval.expr).texto = fmt("((int)pow(%s, %s))", (yyvsp[-2].expr).texto, (yyvsp[0].expr).texto);
            } else {
                (yyval.expr).texto = fmt("pow(%s, %s)", (yyvsp[-2].expr).texto, (yyvsp[0].expr).texto);
            }
            free((yyvsp[-2].expr).texto); free((yyvsp[0].expr).texto);
        }
#line 3312 "y.tab.c"
    break;

  case 98: /* potencia: fator  */
#line 1342 "parserTaPyoCa.y"
            { (yyval.expr) = (yyvsp[0].expr); }
#line 3318 "y.tab.c"
    break;

  case 99: /* l_value: ID  */
#line 1347 "parserTaPyoCa.y"
        {
            Simbolo *s = buscar_simbolo((yyvsp[0].sval));
            if (!s) {
                erro_semantico(yylineno, "Variavel '%s' nao declarada.", (yyvsp[0].sval));
                (yyval.expr).texto = (yyvsp[0].sval);
                (yyval.expr).tipo = criar_tipo(TIPO_ERRO, 0);
                (yyval.expr).is_ref = 0;
            } else if (s->is_func) {
                erro_semantico(yylineno, "'%s' e uma subrotina e nao pode ser usada como variavel.", (yyvsp[0].sval));
                (yyval.expr).texto = (yyvsp[0].sval);
                (yyval.expr).tipo = criar_tipo(TIPO_ERRO, 0);
                (yyval.expr).is_ref = 0;
            } else {
                (yyval.expr).tipo = s->tipo;
                if (s->is_ref == 1) {
                    if (s->tipo.base == TIPO_STRUCT && s->tipo.dimensao == 0) {
                        /* Parametro 'ref' de struct ja e ponteiro (struct T*) */
                        (yyval.expr).texto = (yyvsp[0].sval);
                        (yyval.expr).is_ref = 1;
                    } else {
                        /* Parametro 'ref' escalar/array: desreferencia automaticamente */
                        (yyval.expr).texto = fmt("(*%s)", (yyvsp[0].sval));
                        (yyval.expr).is_ref = 0;
                        free((yyvsp[0].sval));
                    }
                } else if (s->is_ref == 2) {
                    /* Variavel local declarada com 'ref tipo id' */
                    (yyval.expr).texto = (yyvsp[0].sval);
                    (yyval.expr).is_ref = 1;
                } else {
                    (yyval.expr).texto = (yyvsp[0].sval);
                    (yyval.expr).is_ref = 0;
                }
            }
        }
#line 3358 "y.tab.c"
    break;

  case 100: /* l_value: l_value LBRACKET expressao RBRACKET  */
#line 1383 "parserTaPyoCa.y"
        {
            if ((yyvsp[-3].expr).is_ref) (yyval.expr).texto = fmt("(%s)[%s]", (yyvsp[-3].expr).texto, (yyvsp[-1].expr).texto);
            else           (yyval.expr).texto = fmt("%s[%s]", (yyvsp[-3].expr).texto, (yyvsp[-1].expr).texto);
            (yyval.expr).is_ref = 0;
            if ((yyvsp[-1].expr).tipo.base != TIPO_ERRO && ((yyvsp[-1].expr).tipo.base != TIPO_INT || (yyvsp[-1].expr).tipo.dimensao != 0)) {
                erro_semantico(yylineno, "Indice de array deve ser 'int', recebeu '%s'.",
                               tipo_para_string((yyvsp[-1].expr).tipo));
            }
            if ((yyvsp[-3].expr).tipo.base != TIPO_ERRO) {
                if ((yyvsp[-3].expr).tipo.dimensao <= 0) {
                    erro_semantico(yylineno, "Tentativa de indexar tipo nao-array '%s'.",
                                   tipo_para_string((yyvsp[-3].expr).tipo));
                    (yyval.expr).tipo = criar_tipo(TIPO_ERRO, 0);
                } else if ((yyvsp[-3].expr).tipo.base == TIPO_STRUCT) {
                    (yyval.expr).tipo = criar_tipo_struct((yyvsp[-3].expr).tipo.nome_struct, (yyvsp[-3].expr).tipo.dimensao - 1);
                } else {
                    (yyval.expr).tipo = criar_tipo((yyvsp[-3].expr).tipo.base, (yyvsp[-3].expr).tipo.dimensao - 1);
                }
            } else {
                (yyval.expr).tipo = criar_tipo(TIPO_ERRO, 0);
            }
            free((yyvsp[-3].expr).texto); free((yyvsp[-1].expr).texto);
        }
#line 3386 "y.tab.c"
    break;

  case 101: /* l_value: l_value DOT ID  */
#line 1407 "parserTaPyoCa.y"
        {
            if ((yyvsp[-2].expr).is_ref) (yyval.expr).texto = fmt("(%s)->%s", (yyvsp[-2].expr).texto, (yyvsp[0].sval));
            else           (yyval.expr).texto = fmt("%s.%s", (yyvsp[-2].expr).texto, (yyvsp[0].sval));

            if ((yyvsp[-2].expr).tipo.base == TIPO_ERRO) {
                (yyval.expr).tipo = criar_tipo(TIPO_ERRO, 0);
                (yyval.expr).is_ref = 0;
            } else if ((yyvsp[-2].expr).tipo.base != TIPO_STRUCT || (yyvsp[-2].expr).tipo.dimensao != 0) {
                erro_semantico(yylineno, "Acesso ao campo '%s' em '%s', que nao e uma struct.",
                               (yyvsp[0].sval), tipo_para_string((yyvsp[-2].expr).tipo));
                (yyval.expr).tipo = criar_tipo(TIPO_ERRO, 0);
                (yyval.expr).is_ref = 0;
            } else {
                CampoStruct *cf = buscar_campo_struct_info((yyvsp[-2].expr).tipo.nome_struct, (yyvsp[0].sval));
                if (!cf) {
                    erro_semantico(yylineno, "Struct '%s' nao possui o campo '%s'.",
                                   (yyvsp[-2].expr).tipo.nome_struct, (yyvsp[0].sval));
                    (yyval.expr).tipo = criar_tipo(TIPO_ERRO, 0);
                    (yyval.expr).is_ref = 0;
                } else {
                    (yyval.expr).tipo = cf->tipo;
                    (yyval.expr).is_ref = cf->is_ref;
                }
            }
            free((yyvsp[-2].expr).texto); free((yyvsp[0].sval));
        }
#line 3417 "y.tab.c"
    break;

  case 102: /* fator: chamadafuncao  */
#line 1436 "parserTaPyoCa.y"
                              { (yyval.expr) = (yyvsp[0].expr); }
#line 3423 "y.tab.c"
    break;

  case 103: /* fator: l_value  */
#line 1437 "parserTaPyoCa.y"
                              { (yyval.expr) = (yyvsp[0].expr); }
#line 3429 "y.tab.c"
    break;

  case 104: /* fator: INT_NUM  */
#line 1438 "parserTaPyoCa.y"
                              { (yyval.expr).texto = fmt("%d", (yyvsp[0].ival)); (yyval.expr).tipo = criar_tipo(TIPO_INT, 0); (yyval.expr).is_ref = 0; }
#line 3435 "y.tab.c"
    break;

  case 105: /* fator: REAL_NUM  */
#line 1439 "parserTaPyoCa.y"
                              { (yyval.expr).texto = fmt("%g", (yyvsp[0].fval)); (yyval.expr).tipo = criar_tipo(TIPO_REAL, 0); (yyval.expr).is_ref = 0; }
#line 3441 "y.tab.c"
    break;

  case 106: /* fator: STRING_LITERAL  */
#line 1440 "parserTaPyoCa.y"
                              { (yyval.expr).texto = (yyvsp[0].sval); (yyval.expr).tipo = criar_tipo(TIPO_STRING, 0); (yyval.expr).is_ref = 0; }
#line 3447 "y.tab.c"
    break;

  case 107: /* fator: TRUE  */
#line 1441 "parserTaPyoCa.y"
                              { (yyval.expr).texto = strdup("true"); (yyval.expr).tipo = criar_tipo(TIPO_BOOL, 0); (yyval.expr).is_ref = 0; }
#line 3453 "y.tab.c"
    break;

  case 108: /* fator: FALSE  */
#line 1442 "parserTaPyoCa.y"
                              { (yyval.expr).texto = strdup("false"); (yyval.expr).tipo = criar_tipo(TIPO_BOOL, 0); (yyval.expr).is_ref = 0; }
#line 3459 "y.tab.c"
    break;

  case 109: /* fator: NULL_VAL  */
#line 1443 "parserTaPyoCa.y"
                              { (yyval.expr).texto = strdup("NULL"); (yyval.expr).tipo = criar_tipo(TIPO_NULL, 0); (yyval.expr).is_ref = 1; }
#line 3465 "y.tab.c"
    break;

  case 110: /* fator: arrayLiteral  */
#line 1444 "parserTaPyoCa.y"
                              { (yyval.expr) = (yyvsp[0].expr); (yyval.expr).is_ref = 0; }
#line 3471 "y.tab.c"
    break;

  case 111: /* fator: LPAREN expressao RPAREN  */
#line 1445 "parserTaPyoCa.y"
                              { (yyval.expr).texto = fmt("(%s)", (yyvsp[-1].expr).texto); (yyval.expr).tipo = (yyvsp[-1].expr).tipo; (yyval.expr).is_ref = (yyvsp[-1].expr).is_ref; free((yyvsp[-1].expr).texto); }
#line 3477 "y.tab.c"
    break;

  case 112: /* arrayLiteral: LBRACKET array_elements_opt RBRACKET  */
#line 1450 "parserTaPyoCa.y"
        {
            (yyval.expr).texto = fmt("[%s]", (yyvsp[-1].alist)->c_texto);
            if ((yyvsp[-1].alist)->count == 0) {
                erro_semantico(yylineno, "Array literal vazio nao permitido sem inferencia de tipo.");
                (yyval.expr).tipo = criar_tipo(TIPO_ERRO, 0);
            } else {
                Tipo elem_tipo = (yyvsp[-1].alist)->tipos[0];
                int erro_elem = 0;
                for (int i = 0; i < (yyvsp[-1].alist)->count; i++) {
                    if ((yyvsp[-1].alist)->is_ref[i]) {
                        erro_semantico(yylineno, "Elementos de array literal nao podem usar 'ref'.");
                        erro_elem = 1;
                        break;
                    }
                    if (i == 0) continue;
                    if (tipos_compativeis(elem_tipo, (yyvsp[-1].alist)->tipos[i])) {
                        continue;
                    } else if (tipos_compativeis((yyvsp[-1].alist)->tipos[i], elem_tipo)) {
                        /* Promove int[] para real[] se houver um real no meio */
                        elem_tipo = (yyvsp[-1].alist)->tipos[i];
                    } else {
                        erro_semantico(yylineno, "Elementos incompativeis no array literal: '%s' e '%s'.",
                                       tipo_para_string(elem_tipo), tipo_para_string((yyvsp[-1].alist)->tipos[i]));
                        erro_elem = 1;
                        break;
                    }
                }
                if (erro_elem || elem_tipo.base == TIPO_ERRO) {
                    (yyval.expr).tipo = criar_tipo(TIPO_ERRO, 0);
                } else if (elem_tipo.base == TIPO_STRUCT) {
                    (yyval.expr).tipo = criar_tipo_struct(elem_tipo.nome_struct, elem_tipo.dimensao + 1);
                } else {
                    (yyval.expr).tipo = criar_tipo(elem_tipo.base, elem_tipo.dimensao + 1);
                }
            }
            free_lista_args((yyvsp[-1].alist));
        }
#line 3519 "y.tab.c"
    break;

  case 113: /* array_elements_opt: lista_argumentos  */
#line 1490 "parserTaPyoCa.y"
                       { (yyval.alist) = (yyvsp[0].alist); }
#line 3525 "y.tab.c"
    break;

  case 114: /* array_elements_opt: %empty  */
#line 1492 "parserTaPyoCa.y"
        {
            ListaArgs *la = calloc(1, sizeof(ListaArgs));
            la->count = 0;
            la->texto = strdup("");
            la->c_texto = strdup("");
            (yyval.alist) = la;
        }
#line 3537 "y.tab.c"
    break;


#line 3541 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 1501 "parserTaPyoCa.y"


void yyerror(const char *msg) {
    // Se o erro ocorrer no fim do arquivo (EOF), yytext estará vazio
    if (yytext[0] == '\0') {
        fprintf(stderr, "Erro de sintaxe na linha %d: fim de arquivo inesperado.\n", yylineno);
    } else {
        fprintf(stderr, "Erro de sintaxe na linha %d próximo a '%s'.\n", yylineno, yytext);
    }
}

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
