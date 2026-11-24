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
#line 21 "parserTaPyoCa.y"

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

/* Converte Tipo (TaPyoCa) para sintaxe C (ex: int[] -> int*, string -> char*) */
static char *tipo_para_c(Tipo t) {
    const char *base = "int";
    switch (t.base) {
        case TIPO_VOID:   base = "void"; break;
        case TIPO_INT:    base = "int"; break;
        case TIPO_REAL:   base = "double"; break;
        case TIPO_STRING: base = "char*"; break;
        default:          base = "int"; break;
    }
    char buf[64];
    strcpy(buf, base);
    for (int i = 0; i < t.dimensao && i < 10; i++) {
        strcat(buf, "*");
    }
    return strdup(buf);
}

/* ---------- estruturas auxiliares para o semantico ---------- */

static Tipo retorno_esperado;
static int encontrou_return = 0;


#line 148 "y.tab.c"

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

#line 204 "y.tab.c"

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
    ENDIF = 271,                   /* ENDIF  */
    WHILE = 272,                   /* WHILE  */
    ENDWHILE = 273,                /* ENDWHILE  */
    INT = 274,                     /* INT  */
    LBRACKET = 275,                /* LBRACKET  */
    RBRACKET = 276,                /* RBRACKET  */
    STRING = 277,                  /* STRING  */
    REAL = 278,                    /* REAL  */
    LT = 279,                      /* LT  */
    LEQ = 280,                     /* LEQ  */
    GT = 281,                      /* GT  */
    GEQ = 282,                     /* GEQ  */
    EQ = 283,                      /* EQ  */
    NEQ = 284,                     /* NEQ  */
    PLUS = 285,                    /* PLUS  */
    MINUS = 286,                   /* MINUS  */
    MUL = 287,                     /* MUL  */
    DIV = 288,                     /* DIV  */
    PRINT = 289,                   /* PRINT  */
    READ = 290,                    /* READ  */
    REF = 291,                     /* REF  */
    INT_NUM = 292,                 /* INT_NUM  */
    REAL_NUM = 293,                /* REAL_NUM  */
    ID = 294,                      /* ID  */
    STRING_LITERAL = 295           /* STRING_LITERAL  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 98 "parserTaPyoCa.y"

    int ival;
    double fval;
    char *sval;
    ExprInfo expr;
    ListaParams *plist;
    ListaArgs *alist;

#line 270 "y.tab.c"

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
  YYSYMBOL_ENDIF = 16,                     /* ENDIF  */
  YYSYMBOL_WHILE = 17,                     /* WHILE  */
  YYSYMBOL_ENDWHILE = 18,                  /* ENDWHILE  */
  YYSYMBOL_INT = 19,                       /* INT  */
  YYSYMBOL_LBRACKET = 20,                  /* LBRACKET  */
  YYSYMBOL_RBRACKET = 21,                  /* RBRACKET  */
  YYSYMBOL_STRING = 22,                    /* STRING  */
  YYSYMBOL_REAL = 23,                      /* REAL  */
  YYSYMBOL_LT = 24,                        /* LT  */
  YYSYMBOL_LEQ = 25,                       /* LEQ  */
  YYSYMBOL_GT = 26,                        /* GT  */
  YYSYMBOL_GEQ = 27,                       /* GEQ  */
  YYSYMBOL_EQ = 28,                        /* EQ  */
  YYSYMBOL_NEQ = 29,                       /* NEQ  */
  YYSYMBOL_PLUS = 30,                      /* PLUS  */
  YYSYMBOL_MINUS = 31,                     /* MINUS  */
  YYSYMBOL_MUL = 32,                       /* MUL  */
  YYSYMBOL_DIV = 33,                       /* DIV  */
  YYSYMBOL_PRINT = 34,                     /* PRINT  */
  YYSYMBOL_READ = 35,                      /* READ  */
  YYSYMBOL_REF = 36,                       /* REF  */
  YYSYMBOL_INT_NUM = 37,                   /* INT_NUM  */
  YYSYMBOL_REAL_NUM = 38,                  /* REAL_NUM  */
  YYSYMBOL_ID = 39,                        /* ID  */
  YYSYMBOL_STRING_LITERAL = 40,            /* STRING_LITERAL  */
  YYSYMBOL_YYACCEPT = 41,                  /* $accept  */
  YYSYMBOL_programa = 42,                  /* programa  */
  YYSYMBOL_declaracoes_globais = 43,       /* declaracoes_globais  */
  YYSYMBOL_declaracao = 44,                /* declaracao  */
  YYSYMBOL_main_block = 45,                /* main_block  */
  YYSYMBOL_46_1 = 46,                      /* $@1  */
  YYSYMBOL_subrotina = 47,                 /* subrotina  */
  YYSYMBOL_48_2 = 48,                      /* $@2  */
  YYSYMBOL_tipo_opt = 49,                  /* tipo_opt  */
  YYSYMBOL_parametros_opt = 50,            /* parametros_opt  */
  YYSYMBOL_lista_parametros = 51,          /* lista_parametros  */
  YYSYMBOL_comandos = 52,                  /* comandos  */
  YYSYMBOL_corpo_bloco = 53,               /* corpo_bloco  */
  YYSYMBOL_comando = 54,                   /* comando  */
  YYSYMBOL_comandoprint = 55,              /* comandoprint  */
  YYSYMBOL_declaracaovar = 56,             /* declaracaovar  */
  YYSYMBOL_atribuicao = 57,                /* atribuicao  */
  YYSYMBOL_comandowhile = 58,              /* comandowhile  */
  YYSYMBOL_59_3 = 59,                      /* $@3  */
  YYSYMBOL_comandoif = 60,                 /* comandoif  */
  YYSYMBOL_61_4 = 61,                      /* $@4  */
  YYSYMBOL_retorno = 62,                   /* retorno  */
  YYSYMBOL_chamadafuncao = 63,             /* chamadafuncao  */
  YYSYMBOL_argumentos_opt = 64,            /* argumentos_opt  */
  YYSYMBOL_lista_argumentos = 65,          /* lista_argumentos  */
  YYSYMBOL_tipo = 66,                      /* tipo  */
  YYSYMBOL_expressao = 67,                 /* expressao  */
  YYSYMBOL_exprelacional = 68,             /* exprelacional  */
  YYSYMBOL_op_relacional = 69,             /* op_relacional  */
  YYSYMBOL_exparitimetica = 70,            /* exparitimetica  */
  YYSYMBOL_termo = 71,                     /* termo  */
  YYSYMBOL_l_value = 72,                   /* l_value  */
  YYSYMBOL_fator = 73,                     /* fator  */
  YYSYMBOL_arrayLiteral = 74,              /* arrayLiteral  */
  YYSYMBOL_array_elements_opt = 75         /* array_elements_opt  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




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

#if !defined yyoverflow

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
#endif /* !defined yyoverflow */

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
#define YYFINAL  14
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   150

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  41
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  35
/* YYNRULES -- Number of rules.  */
#define YYNRULES  78
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  141

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   295


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
      35,    36,    37,    38,    39,    40
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   128,   128,   140,   141,   145,   146,   151,   150,   172,
     171,   212,   213,   217,   219,   229,   251,   266,   288,   306,
     307,   311,   312,   316,   317,   318,   319,   320,   321,   322,
     326,   345,   369,   395,   409,   425,   440,   439,   460,   459,
     479,   494,   571,   573,   582,   595,   606,   619,   632,   633,
     634,   635,   639,   643,   649,   653,   654,   655,   656,   657,
     658,   662,   668,   674,   678,   684,   690,   694,   715,   738,
     739,   740,   741,   742,   743,   744,   748,   782,   784
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "MAIN", "LPAREN",
  "RPAREN", "NEWLINE", "INDENT", "DEDENT", "ENDMAIN", "SUB", "ENDSUB",
  "COMMA", "ASSIGN", "RETURN", "IF", "ENDIF", "WHILE", "ENDWHILE", "INT",
  "LBRACKET", "RBRACKET", "STRING", "REAL", "LT", "LEQ", "GT", "GEQ", "EQ",
  "NEQ", "PLUS", "MINUS", "MUL", "DIV", "PRINT", "READ", "REF", "INT_NUM",
  "REAL_NUM", "ID", "STRING_LITERAL", "$accept", "programa",
  "declaracoes_globais", "declaracao", "main_block", "$@1", "subrotina",
  "$@2", "tipo_opt", "parametros_opt", "lista_parametros", "comandos",
  "corpo_bloco", "comando", "comandoprint", "declaracaovar", "atribuicao",
  "comandowhile", "$@3", "comandoif", "$@4", "retorno", "chamadafuncao",
  "argumentos_opt", "lista_argumentos", "tipo", "expressao",
  "exprelacional", "op_relacional", "exparitimetica", "termo", "l_value",
  "fator", "arrayLiteral", "array_elements_opt", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-56)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       9,     3,    38,    32,     9,   -56,   -56,   -56,     4,   -56,
     -56,   -56,   -21,    17,   -56,   -56,   -56,    25,    31,    33,
      -8,   -56,    56,    38,    67,    65,   -17,   -56,    72,   -16,
     -56,    -6,   -56,    93,    77,   -56,    79,    38,   -15,   -56,
      58,    83,    87,    88,    89,   103,   -56,   -56,   -56,   -56,
     -56,   -56,   -56,   -14,    18,   -56,    56,   -12,   -56,    58,
       6,   -56,   -56,   -56,   -56,   -56,   -56,    94,    15,    80,
     -56,   -56,    58,    58,     6,     6,   -56,    86,    16,    58,
     100,   -56,   108,    75,   105,   -56,   107,   -56,   -56,   -56,
     -56,   -56,   -56,    58,    58,    58,    58,    58,   121,   124,
     125,   105,   126,    29,   129,   -56,   113,   130,   -56,   -56,
      80,    66,   -56,    15,    15,    28,   -56,   -56,   -56,   -56,
     -56,   -56,   131,   -56,   132,   -56,   -56,    75,   -56,   133,
     134,   136,   -56,    80,    56,    56,   -56,   122,   127,   -56,
     -56
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,    12,     0,     2,     4,     6,     5,     0,    48,
      50,    49,     0,    11,     1,     3,     7,     0,     0,     0,
      14,    51,    22,     0,     0,    13,     0,    20,     0,     0,
       9,     0,    18,     0,     0,    16,     0,     0,     0,    21,
       0,     0,     0,     0,    67,     0,    29,    23,    24,    26,
      25,    28,    27,     0,     0,     8,    22,     0,    17,     0,
      78,    71,    72,    73,    69,    40,    52,    54,    63,    70,
      66,    74,     0,     0,    43,    43,    19,    33,     0,     0,
       0,    15,     0,     0,    77,    47,     0,    55,    56,    57,
      58,    59,    60,     0,     0,     0,     0,     0,     0,     0,
       0,    42,     0,     0,     0,    35,     0,     0,    75,    67,
      45,     0,    76,    61,    62,    53,    64,    65,    38,    36,
      30,    41,     0,    32,     0,    68,    10,     0,    46,     0,
       0,     0,    34,    44,    22,    22,    31,     0,     0,    39,
      37
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -56,   -56,   -56,   138,   -56,   -56,   -56,   -56,   -56,   -56,
     -56,   -56,   -55,   -56,   -56,   -56,   -56,   -56,   -56,   -56,
     -56,   -56,   110,    69,    90,    51,   -38,   -56,   -56,    52,
     -18,   -33,    -7,   -56,   -56
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     3,     4,     5,     6,    19,     7,    36,    12,    24,
      25,    33,    28,    45,    46,    47,    48,    49,   130,    50,
     129,    51,    64,   100,   101,    13,    85,    66,    95,    67,
      68,    69,    70,    71,    86
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      54,    80,    65,    18,    18,    18,    18,     8,    18,    16,
      59,     9,     1,     9,    10,    11,    10,    11,    17,     2,
      59,    82,    32,    35,    58,    77,    60,    81,    23,    20,
      37,    78,    14,    59,    98,    99,    60,    18,    79,    22,
     105,   106,    83,    61,    62,    44,    63,    96,    97,    60,
     110,   104,    21,    61,    62,    44,    63,     9,    93,    94,
      10,    11,    59,    27,   122,   123,    61,    62,    44,    63,
      59,    26,    30,   128,    29,   113,   114,    31,    60,   137,
     138,    34,    38,    55,    53,    56,    60,    72,    57,   116,
     117,    73,    74,    75,   133,    61,    62,    44,    63,   103,
      79,    39,   127,    61,    62,    44,    63,    40,    41,    76,
      42,   107,     9,   108,   109,    10,    11,   111,    87,    88,
      89,    90,    91,    92,    93,    94,   118,    43,   112,   119,
     120,   121,    44,   124,   125,   131,   126,   132,   139,   134,
     135,   136,    15,    52,   102,   140,     0,   115,     0,     0,
      84
};

static const yytype_int16 yycheck[] =
{
      33,    56,    40,    20,    20,    20,    20,     4,    20,     5,
       4,    19,     3,    19,    22,    23,    22,    23,    39,    10,
       4,    59,    39,    39,    39,    39,    20,    39,    36,     4,
      36,    13,     0,     4,    72,    73,    20,    20,    20,     6,
      78,    79,    36,    37,    38,    39,    40,    32,    33,    20,
      83,    35,    21,    37,    38,    39,    40,    19,    30,    31,
      22,    23,     4,     7,    35,   103,    37,    38,    39,    40,
       4,    20,     5,   111,    23,    93,    94,    12,    20,   134,
     135,     9,    31,     6,    33,     6,    20,     4,    37,    96,
      97,     4,     4,     4,   127,    37,    38,    39,    40,    13,
      20,     8,    36,    37,    38,    39,    40,    14,    15,     6,
      17,    11,    19,     5,    39,    22,    23,    12,    24,    25,
      26,    27,    28,    29,    30,    31,     5,    34,    21,     5,
       5,     5,    39,     4,    21,     4,     6,     5,    16,     6,
       6,     5,     4,    33,    75,    18,    -1,    95,    -1,    -1,
      60
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    10,    42,    43,    44,    45,    47,     4,    19,
      22,    23,    49,    66,     0,    44,     5,    39,    20,    46,
       4,    21,     6,    36,    50,    51,    66,     7,    53,    66,
       5,    12,    39,    52,     9,    39,    48,    36,    66,     8,
      14,    15,    17,    34,    39,    54,    55,    56,    57,    58,
      60,    62,    63,    66,    72,     6,     6,    66,    39,     4,
      20,    37,    38,    40,    63,    67,    68,    70,    71,    72,
      73,    74,     4,     4,     4,     4,     6,    39,    13,    20,
      53,    39,    67,    36,    65,    67,    75,    24,    25,    26,
      27,    28,    29,    30,    31,    69,    32,    33,    67,    67,
      64,    65,    64,    13,    35,    67,    67,    11,     5,    39,
      72,    12,    21,    71,    71,    70,    73,    73,     5,     5,
       5,     5,    35,    67,     4,    21,     6,    36,    67,    61,
      59,     4,     5,    72,     6,     6,     5,    53,    53,    16,
      18
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    41,    42,    43,    43,    44,    44,    46,    45,    48,
      47,    49,    49,    50,    50,    51,    51,    51,    51,    52,
      52,    53,    53,    54,    54,    54,    54,    54,    54,    54,
      55,    56,    56,    56,    57,    57,    59,    58,    61,    60,
      62,    63,    64,    64,    65,    65,    65,    65,    66,    66,
      66,    66,    67,    68,    68,    69,    69,    69,    69,    69,
      69,    70,    70,    70,    71,    71,    71,    72,    72,    73,
      73,    73,    73,    73,    73,    73,    74,    75,    75
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     0,     8,     0,
      11,     1,     0,     1,     0,     5,     3,     4,     2,     3,
       0,     3,     0,     1,     1,     1,     1,     1,     1,     1,
       4,     6,     4,     2,     5,     3,     0,     8,     0,     8,
       2,     4,     1,     0,     4,     2,     3,     1,     1,     1,
       1,     3,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     1,     3,     3,     1,     1,     4,     1,
       1,     1,     1,     1,     1,     3,     3,     1,     0
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
#line 129 "parserTaPyoCa.y"
        {
            if (obter_erros_semanticos() == 0) {
                printf("Analise sintatica e semantica concluida com sucesso.\n");
            } else {
                fprintf(stderr, "Falha na compilacao: %d erro(s) semantico(s).\n", obter_erros_semanticos());
                YYABORT;
            }
        }
#line 1431 "y.tab.c"
    break;

  case 3: /* declaracoes_globais: declaracoes_globais declaracao  */
#line 140 "parserTaPyoCa.y"
                                     {}
#line 1437 "y.tab.c"
    break;

  case 4: /* declaracoes_globais: declaracao  */
#line 141 "parserTaPyoCa.y"
                                     {}
#line 1443 "y.tab.c"
    break;

  case 5: /* declaracao: subrotina  */
#line 145 "parserTaPyoCa.y"
                 {}
#line 1449 "y.tab.c"
    break;

  case 6: /* declaracao: main_block  */
#line 146 "parserTaPyoCa.y"
                 {}
#line 1455 "y.tab.c"
    break;

  case 7: /* $@1: %empty  */
#line 151 "parserTaPyoCa.y"
        {
            pr("[main]");
            emit_c("int main(void) %c", 123);
            depth++;
            retorno_esperado = criar_tipo(TIPO_VOID, 0);
            encontrou_return = 0;
            entrar_escopo();
        }
#line 1468 "y.tab.c"
    break;

  case 8: /* main_block: MAIN LPAREN RPAREN $@1 NEWLINE corpo_bloco ENDMAIN NEWLINE  */
#line 160 "parserTaPyoCa.y"
        {
            sair_escopo();
            emit_c("return 0;");
            depth--;
            emit_c("%c\n", 125);
            pr("[endmain]");
            printf("\n");
        }
#line 1481 "y.tab.c"
    break;

  case 9: /* $@2: %empty  */
#line 172 "parserTaPyoCa.y"
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
#line 1511 "y.tab.c"
    break;

  case 10: /* subrotina: SUB tipo_opt ID LPAREN parametros_opt RPAREN $@2 NEWLINE corpo_bloco ENDSUB NEWLINE  */
#line 198 "parserTaPyoCa.y"
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
#line 1527 "y.tab.c"
    break;

  case 11: /* tipo_opt: tipo  */
#line 212 "parserTaPyoCa.y"
                   { (yyval.sval) = (yyvsp[0].sval); }
#line 1533 "y.tab.c"
    break;

  case 12: /* tipo_opt: %empty  */
#line 213 "parserTaPyoCa.y"
                   { (yyval.sval) = strdup(""); }
#line 1539 "y.tab.c"
    break;

  case 13: /* parametros_opt: lista_parametros  */
#line 217 "parserTaPyoCa.y"
                       { (yyval.plist) = (yyvsp[0].plist); }
#line 1545 "y.tab.c"
    break;

  case 14: /* parametros_opt: %empty  */
#line 219 "parserTaPyoCa.y"
        {
            ListaParams *lp = calloc(1, sizeof(ListaParams));
            lp->count = 0;
            lp->texto = strdup("");
            lp->c_texto = strdup("");
            (yyval.plist) = lp;
        }
#line 1557 "y.tab.c"
    break;

  case 15: /* lista_parametros: lista_parametros COMMA REF tipo ID  */
#line 230 "parserTaPyoCa.y"
        {
            (yyval.plist) = (yyvsp[-4].plist);
            Tipo t = parse_tipo_string((yyvsp[-1].sval));
            char *t_c = tipo_para_c(t);

            if ((yyval.plist)->count < 32) {
                int idx = (yyval.plist)->count++;
                (yyval.plist)->nomes[idx] = (yyvsp[0].sval);
                (yyval.plist)->tipos[idx] = t;
                (yyval.plist)->is_ref[idx] = 1; /* Marca como parametro por referencia */
            } else {
                erro_semantico(yylineno, "Limite maximo de 32 parametros excedido.");
                free((yyvsp[0].sval));
            }

            char *novo = fmt("%s, ref %s %s", (yyval.plist)->texto, (yyvsp[-1].sval), (yyval.plist)->nomes[(yyval.plist)->count - 1]);
            char *novo_c = fmt("%s, %s *%s", (yyval.plist)->c_texto, t_c, (yyval.plist)->nomes[(yyval.plist)->count - 1]);
            free((yyval.plist)->texto); free((yyval.plist)->c_texto); free((yyvsp[-1].sval)); free(t_c);
            (yyval.plist)->texto = novo;
            (yyval.plist)->c_texto = novo_c;
        }
#line 1583 "y.tab.c"
    break;

  case 16: /* lista_parametros: REF tipo ID  */
#line 252 "parserTaPyoCa.y"
        {
            ListaParams *lp = calloc(1, sizeof(ListaParams));
            Tipo t = parse_tipo_string((yyvsp[-1].sval));
            char *t_c = tipo_para_c(t);

            lp->nomes[0] = (yyvsp[0].sval);
            lp->tipos[0] = t;
            lp->is_ref[0] = 1; /* Marca como parametro por referencia */
            lp->count = 1;
            lp->texto = fmt("ref %s %s", (yyvsp[-1].sval), (yyvsp[0].sval));
            lp->c_texto = fmt("%s *%s", t_c, (yyvsp[0].sval));
            free((yyvsp[-1].sval)); free(t_c);
            (yyval.plist) = lp;
        }
#line 1602 "y.tab.c"
    break;

  case 17: /* lista_parametros: lista_parametros COMMA tipo ID  */
#line 267 "parserTaPyoCa.y"
        {
            (yyval.plist) = (yyvsp[-3].plist);
            Tipo t = parse_tipo_string((yyvsp[-1].sval));
            char *t_c = tipo_para_c(t);

            if ((yyval.plist)->count < 32) {
                int idx = (yyval.plist)->count++;
                (yyval.plist)->nomes[idx] = (yyvsp[0].sval);
                (yyval.plist)->tipos[idx] = t;
                (yyval.plist)->is_ref[idx] = 0; /* Parametro normal (por valor) */
            } else {
                erro_semantico(yylineno, "Limite maximo de 32 parametros excedido.");
                free((yyvsp[0].sval));
            }

            char *novo = fmt("%s, %s %s", (yyval.plist)->texto, (yyvsp[-1].sval), (yyval.plist)->nomes[(yyval.plist)->count - 1]);
            char *novo_c = fmt("%s, %s %s", (yyval.plist)->c_texto, t_c, (yyval.plist)->nomes[(yyval.plist)->count - 1]);
            free((yyval.plist)->texto); free((yyval.plist)->c_texto); free((yyvsp[-1].sval)); free(t_c);
            (yyval.plist)->texto = novo;
            (yyval.plist)->c_texto = novo_c;
        }
#line 1628 "y.tab.c"
    break;

  case 18: /* lista_parametros: tipo ID  */
#line 289 "parserTaPyoCa.y"
        {
            ListaParams *lp = calloc(1, sizeof(ListaParams));
            Tipo t = parse_tipo_string((yyvsp[-1].sval));
            char *t_c = tipo_para_c(t);

            lp->nomes[0] = (yyvsp[0].sval);
            lp->tipos[0] = t;
            lp->is_ref[0] = 0; /* Parametro normal (por valor) */
            lp->count = 1;
            lp->texto = fmt("%s %s", (yyvsp[-1].sval), (yyvsp[0].sval));
            lp->c_texto = fmt("%s %s", t_c, (yyvsp[0].sval));
            free((yyvsp[-1].sval)); free(t_c);
            (yyval.plist) = lp;
        }
#line 1647 "y.tab.c"
    break;

  case 19: /* comandos: comandos comando NEWLINE  */
#line 306 "parserTaPyoCa.y"
                               {}
#line 1653 "y.tab.c"
    break;

  case 20: /* comandos: %empty  */
#line 307 "parserTaPyoCa.y"
                               {}
#line 1659 "y.tab.c"
    break;

  case 21: /* corpo_bloco: INDENT comandos DEDENT  */
#line 311 "parserTaPyoCa.y"
                             {}
#line 1665 "y.tab.c"
    break;

  case 22: /* corpo_bloco: %empty  */
#line 312 "parserTaPyoCa.y"
                             {}
#line 1671 "y.tab.c"
    break;

  case 23: /* comando: declaracaovar  */
#line 316 "parserTaPyoCa.y"
                    {}
#line 1677 "y.tab.c"
    break;

  case 24: /* comando: atribuicao  */
#line 317 "parserTaPyoCa.y"
                    {}
#line 1683 "y.tab.c"
    break;

  case 25: /* comando: comandoif  */
#line 318 "parserTaPyoCa.y"
                    {}
#line 1689 "y.tab.c"
    break;

  case 26: /* comando: comandowhile  */
#line 319 "parserTaPyoCa.y"
                    {}
#line 1695 "y.tab.c"
    break;

  case 27: /* comando: chamadafuncao  */
#line 320 "parserTaPyoCa.y"
                    { pr("[chamada] %s", (yyvsp[0].expr).texto); free((yyvsp[0].expr).texto); }
#line 1701 "y.tab.c"
    break;

  case 28: /* comando: retorno  */
#line 321 "parserTaPyoCa.y"
                    {}
#line 1707 "y.tab.c"
    break;

  case 29: /* comando: comandoprint  */
#line 322 "parserTaPyoCa.y"
                    {}
#line 1713 "y.tab.c"
    break;

  case 30: /* comandoprint: PRINT LPAREN argumentos_opt RPAREN  */
#line 327 "parserTaPyoCa.y"
        {
            pr("[print] print %s", (yyvsp[-1].alist)->texto);
            if ((yyvsp[-1].alist)->count == 0) {
                emit_c("printf(\"\\n\");");
            } else {
                /* Verifica o tipo do primeiro argumento para escolher o especificador C */
                const char *spec = "%d";
                if ((yyvsp[-1].alist)->tipos[0].base == TIPO_REAL)        spec = "%g";
                else if ((yyvsp[-1].alist)->tipos[0].base == TIPO_STRING) spec = "%s";

                emit_c("printf(\"%s\\n\", %s);", spec, (yyvsp[-1].alist)->texto);
            }
            free((yyvsp[-1].alist)->texto);
            free((yyvsp[-1].alist));
        }
#line 1733 "y.tab.c"
    break;

  case 31: /* declaracaovar: tipo ID ASSIGN READ LPAREN RPAREN  */
#line 346 "parserTaPyoCa.y"
        {
            pr("[decl] %s %s <- read()", (yyvsp[-5].sval), (yyvsp[-4].sval));
            Tipo t_var = parse_tipo_string((yyvsp[-5].sval));
            if (inserir_variavel((yyvsp[-4].sval), t_var, yylineno) != NULL) {
                if (t_var.dimensao > 0) {
                    erro_semantico(yylineno, "Nao e possivel usar 'read()' diretamente em um array ('%s').", (yyvsp[-4].sval));
                } else {
                    char *t_c = tipo_para_c(t_var);
                    if (t_var.base == TIPO_INT) {
                        emit_c("%s %s;", t_c, (yyvsp[-4].sval));
                        emit_c("if (scanf(\"%%d\", &%s) != 1) { fprintf(stderr, \"Erro em tempo de execucao: esperado 'int'.\\n\"); exit(1); }", (yyvsp[-4].sval));
                    } else if (t_var.base == TIPO_REAL) {
                        emit_c("%s %s;", t_c, (yyvsp[-4].sval));
                        emit_c("if (scanf(\"%%lf\", &%s) != 1) { fprintf(stderr, \"Erro em tempo de execucao: esperado 'real'.\\n\"); exit(1); }", (yyvsp[-4].sval));
                    } else if (t_var.base == TIPO_STRING) {
                        emit_c("char *%s = malloc(256);", (yyvsp[-4].sval));
                        emit_c("if (scanf(\"%%255s\", %s) != 1) { fprintf(stderr, \"Erro em tempo de execucao: esperado 'string'.\\n\"); exit(1); }", (yyvsp[-4].sval));
                    }
                    free(t_c);
                }
            }
            free((yyvsp[-5].sval)); free((yyvsp[-4].sval));
        }
#line 1761 "y.tab.c"
    break;

  case 32: /* declaracaovar: tipo ID ASSIGN expressao  */
#line 370 "parserTaPyoCa.y"
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
                        char *elem_c = tipo_para_c(t_elem);
                        char *init = strdup((yyvsp[0].expr).texto);
                        init[0] = '{';
                        init[strlen(init) - 1] = '}';
                        emit_c("%s %s[] = %s;", elem_c, (yyvsp[-2].sval), init);
                        free(elem_c); free(init);
                    } else {
                        emit_c("%s %s = %s;", t_c, (yyvsp[-2].sval), (yyvsp[0].expr).texto);
                    }
                    free(t_c);
                }
            }
            free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[0].expr).texto);
        }
#line 1791 "y.tab.c"
    break;

  case 33: /* declaracaovar: tipo ID  */
#line 396 "parserTaPyoCa.y"
        {
            pr("[decl] %s %s", (yyvsp[-1].sval), (yyvsp[0].sval));
            Tipo t_var = parse_tipo_string((yyvsp[-1].sval));
            if (inserir_variavel((yyvsp[0].sval), t_var, yylineno) != NULL) {
                char *t_c = tipo_para_c(t_var);
                emit_c("%s %s;", t_c, (yyvsp[0].sval));
                free(t_c);
            }
            free((yyvsp[-1].sval)); free((yyvsp[0].sval));
        }
#line 1806 "y.tab.c"
    break;

  case 34: /* atribuicao: l_value ASSIGN READ LPAREN RPAREN  */
#line 410 "parserTaPyoCa.y"
        {
            pr("[atrib] %s <- read()", (yyvsp[-4].expr).texto);
            if ((yyvsp[-4].expr).tipo.base != TIPO_ERRO) {
                if ((yyvsp[-4].expr).tipo.dimensao > 0) {
                    erro_semantico(yylineno, "Nao e possivel usar 'read()' diretamente em um array ('%s').", (yyvsp[-4].expr).texto);
                } else if ((yyvsp[-4].expr).tipo.base == TIPO_INT) {
                    emit_c("if (scanf(\"%%d\", &%s) != 1) { fprintf(stderr, \"Erro em tempo de execucao: esperado 'int'.\\n\"); exit(1); }", (yyvsp[-4].expr).texto);
                } else if ((yyvsp[-4].expr).tipo.base == TIPO_REAL) {
                    emit_c("if (scanf(\"%%lf\", &%s) != 1) { fprintf(stderr, \"Erro em tempo de execucao: esperado 'real'.\\n\"); exit(1); }", (yyvsp[-4].expr).texto);
                } else if ((yyvsp[-4].expr).tipo.base == TIPO_STRING) {
                    emit_c("if (scanf(\"%%255s\", %s) != 1) { fprintf(stderr, \"Erro em tempo de execucao: esperado 'string'.\\n\"); exit(1); }", (yyvsp[-4].expr).texto);
                }
            }
            free((yyvsp[-4].expr).texto);
        }
#line 1826 "y.tab.c"
    break;

  case 35: /* atribuicao: l_value ASSIGN expressao  */
#line 426 "parserTaPyoCa.y"
        {
            pr("[atrib] %s <- %s", (yyvsp[-2].expr).texto, (yyvsp[0].expr).texto);
            if (!tipos_compativeis((yyvsp[-2].expr).tipo, (yyvsp[0].expr).tipo)) {
                erro_semantico(yylineno, "Atribuicao incompativel: '%s' espera '%s', mas recebeu '%s'.",
                               (yyvsp[-2].expr).texto, tipo_para_string((yyvsp[-2].expr).tipo), tipo_para_string((yyvsp[0].expr).tipo));
            } else {
                emit_c("%s = %s;", (yyvsp[-2].expr).texto, (yyvsp[0].expr).texto);
            }
            free((yyvsp[-2].expr).texto); free((yyvsp[0].expr).texto);
        }
#line 1841 "y.tab.c"
    break;

  case 36: /* $@3: %empty  */
#line 440 "parserTaPyoCa.y"
        {
            pr("[while] while (%s)", (yyvsp[-1].expr).texto);
            depth++;
            if ((yyvsp[-1].expr).tipo.base != TIPO_ERRO && ((yyvsp[-1].expr).tipo.base != TIPO_INT || (yyvsp[-1].expr).tipo.dimensao != 0)) {
                erro_semantico(yylineno, "Condicao do 'while' deve ser logica/inteira, recebeu '%s'.",
                               tipo_para_string((yyvsp[-1].expr).tipo));
            }
            entrar_escopo();
            free((yyvsp[-1].expr).texto);
        }
#line 1856 "y.tab.c"
    break;

  case 37: /* comandowhile: WHILE LPAREN expressao RPAREN $@3 NEWLINE corpo_bloco ENDWHILE  */
#line 451 "parserTaPyoCa.y"
        {
            sair_escopo();
            depth--;
            pr("[endwhile]");
        }
#line 1866 "y.tab.c"
    break;

  case 38: /* $@4: %empty  */
#line 460 "parserTaPyoCa.y"
        {
            pr("[if] if (%s)", (yyvsp[-1].expr).texto);
            depth++;
            if ((yyvsp[-1].expr).tipo.base != TIPO_ERRO && ((yyvsp[-1].expr).tipo.base != TIPO_INT || (yyvsp[-1].expr).tipo.dimensao != 0)) {
                erro_semantico(yylineno, "Condicao do 'if' deve ser logica/inteira, recebeu '%s'.",
                               tipo_para_string((yyvsp[-1].expr).tipo));
            }
            entrar_escopo();
            free((yyvsp[-1].expr).texto);
        }
#line 1881 "y.tab.c"
    break;

  case 39: /* comandoif: IF LPAREN expressao RPAREN $@4 NEWLINE corpo_bloco ENDIF  */
#line 471 "parserTaPyoCa.y"
        {
            sair_escopo();
            depth--;
            pr("[endif]");
        }
#line 1891 "y.tab.c"
    break;

  case 40: /* retorno: RETURN expressao  */
#line 480 "parserTaPyoCa.y"
        {
            pr("[return] %s", (yyvsp[0].expr).texto);
            encontrou_return = 1;
            if (retorno_esperado.base == TIPO_VOID) {
                erro_semantico(yylineno, "Subrotina sem tipo de retorno (ou main) nao deve retornar valor.");
            } else if (!tipos_compativeis(retorno_esperado, (yyvsp[0].expr).tipo)) {
                erro_semantico(yylineno, "Tipo de retorno incompativel: esperado '%s', obtido '%s'.",
                               tipo_para_string(retorno_esperado), tipo_para_string((yyvsp[0].expr).tipo));
            }
            free((yyvsp[0].expr).texto);
        }
#line 1907 "y.tab.c"
    break;

  case 41: /* chamadafuncao: ID LPAREN argumentos_opt RPAREN  */
#line 495 "parserTaPyoCa.y"
        {
            /* Emite a chamada usando c_texto (que contem '&(...)' nos argumentos ref) */
            (yyval.expr).texto = fmt("%s(%s)", (yyvsp[-3].sval), (yyvsp[-1].alist)->c_texto);

            /* 1. Built-in: comprimento(array) -> int */
            if (strcmp((yyvsp[-3].sval), "comprimento") == 0) {
                if ((yyvsp[-1].alist)->count != 1) {
                    erro_semantico(yylineno, "'comprimento' espera 1 argumento, recebeu %d.", (yyvsp[-1].alist)->count);
                    (yyval.expr).tipo = criar_tipo(TIPO_ERRO, 0);
                } else if ((yyvsp[-1].alist)->is_ref[0]) {
                    erro_semantico(yylineno, "'comprimento' nao aceita argumento por referencia ('ref').");
                    (yyval.expr).tipo = criar_tipo(TIPO_ERRO, 0);
                } else if ((yyvsp[-1].alist)->tipos[0].dimensao <= 0 && (yyvsp[-1].alist)->tipos[0].base != TIPO_ERRO) {
                    erro_semantico(yylineno, "'comprimento' espera um array, recebeu '%s'.",
                                   tipo_para_string((yyvsp[-1].alist)->tipos[0]));
                    (yyval.expr).tipo = criar_tipo(TIPO_ERRO, 0);
                } else {
                    (yyval.expr).tipo = criar_tipo(TIPO_INT, 0);
                }
            }
            /* 2. Built-in: swap(a, b) -> void */
            else if (strcmp((yyvsp[-3].sval), "swap") == 0) {
                if ((yyvsp[-1].alist)->count != 2) {
                    erro_semantico(yylineno, "'swap' espera 2 argumentos, recebeu %d.", (yyvsp[-1].alist)->count);
                } else if ((yyvsp[-1].alist)->tipos[0].base != TIPO_ERRO && (yyvsp[-1].alist)->tipos[1].base != TIPO_ERRO &&
                           ((yyvsp[-1].alist)->tipos[0].base != (yyvsp[-1].alist)->tipos[1].base || (yyvsp[-1].alist)->tipos[0].dimensao != (yyvsp[-1].alist)->tipos[1].dimensao)) {
                    erro_semantico(yylineno, "'swap' requer argumentos exatamente do mesmo tipo, recebeu '%s' e '%s'.",
                                   tipo_para_string((yyvsp[-1].alist)->tipos[0]), tipo_para_string((yyvsp[-1].alist)->tipos[1]));
                }
                (yyval.expr).tipo = criar_tipo(TIPO_VOID, 0);
            }
            /* 3. Demais subrotinas definidas pelo usuario */
            else {
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
                                /* Ponteiros em C exigem tipo identico (nao permite passar int* para double*) */
                                if ((yyvsp[-1].alist)->tipos[i].base != TIPO_ERRO &&
                                    (s->params[i].base != (yyvsp[-1].alist)->tipos[i].base || s->params[i].dimensao != (yyvsp[-1].alist)->tipos[i].dimensao)) {
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
            free((yyvsp[-3].sval)); free((yyvsp[-1].alist)->texto); free((yyvsp[-1].alist)->c_texto); free((yyvsp[-1].alist));
        }
#line 1985 "y.tab.c"
    break;

  case 42: /* argumentos_opt: lista_argumentos  */
#line 571 "parserTaPyoCa.y"
                       { (yyval.alist) = (yyvsp[0].alist); }
#line 1991 "y.tab.c"
    break;

  case 43: /* argumentos_opt: %empty  */
#line 573 "parserTaPyoCa.y"
        {
            ListaArgs *la = calloc(1, sizeof(ListaArgs));
            la->count = 0;
            la->texto = strdup("");
            (yyval.alist) = la;
        }
#line 2002 "y.tab.c"
    break;

  case 44: /* lista_argumentos: lista_argumentos COMMA REF l_value  */
#line 583 "parserTaPyoCa.y"
        {
            int idx = (yyvsp[-3].alist)->count;
            (yyvsp[-3].alist)->tipos[idx] = (yyvsp[0].expr).tipo;
            (yyvsp[-3].alist)->is_ref[idx] = 1;
            (yyvsp[-3].alist)->count++;
            char *novo_t = fmt("%s, ref %s", (yyvsp[-3].alist)->texto, (yyvsp[0].expr).texto);
            char *novo_c = fmt("%s, &(%s)", (yyvsp[-3].alist)->c_texto, (yyvsp[0].expr).texto);
            free((yyvsp[-3].alist)->texto); free((yyvsp[-3].alist)->c_texto); free((yyvsp[0].expr).texto);
            (yyvsp[-3].alist)->texto = novo_t;
            (yyvsp[-3].alist)->c_texto = novo_c;
            (yyval.alist) = (yyvsp[-3].alist);
        }
#line 2019 "y.tab.c"
    break;

  case 45: /* lista_argumentos: REF l_value  */
#line 596 "parserTaPyoCa.y"
        {
            ListaArgs *la = calloc(1, sizeof(ListaArgs));
            la->tipos[0] = (yyvsp[0].expr).tipo;
            la->is_ref[0] = 1;
            la->count = 1;
            la->texto = fmt("ref %s", (yyvsp[0].expr).texto);
            la->c_texto = fmt("&(%s)", (yyvsp[0].expr).texto);
            free((yyvsp[0].expr).texto);
            (yyval.alist) = la;
        }
#line 2034 "y.tab.c"
    break;

  case 46: /* lista_argumentos: lista_argumentos COMMA expressao  */
#line 607 "parserTaPyoCa.y"
        {
            int idx = (yyvsp[-2].alist)->count;
            (yyvsp[-2].alist)->tipos[idx] = (yyvsp[0].expr).tipo;
            (yyvsp[-2].alist)->is_ref[idx] = 0;
            (yyvsp[-2].alist)->count++;
            char *novo_t = fmt("%s, %s", (yyvsp[-2].alist)->texto, (yyvsp[0].expr).texto);
            char *novo_c = fmt("%s, %s", (yyvsp[-2].alist)->c_texto, (yyvsp[0].expr).texto);
            free((yyvsp[-2].alist)->texto); free((yyvsp[-2].alist)->c_texto); free((yyvsp[0].expr).texto);
            (yyvsp[-2].alist)->texto = novo_t;
            (yyvsp[-2].alist)->c_texto = novo_c;
            (yyval.alist) = (yyvsp[-2].alist);
        }
#line 2051 "y.tab.c"
    break;

  case 47: /* lista_argumentos: expressao  */
#line 620 "parserTaPyoCa.y"
        {
            ListaArgs *la = calloc(1, sizeof(ListaArgs));
            la->tipos[0] = (yyvsp[0].expr).tipo;
            la->is_ref[0] = 0;
            la->count = 1;
            la->texto = strdup((yyvsp[0].expr).texto);
            la->c_texto = (yyvsp[0].expr).texto;
            (yyval.alist) = la;
        }
#line 2065 "y.tab.c"
    break;

  case 48: /* tipo: INT  */
#line 632 "parserTaPyoCa.y"
                               { (yyval.sval) = strdup("int"); }
#line 2071 "y.tab.c"
    break;

  case 49: /* tipo: REAL  */
#line 633 "parserTaPyoCa.y"
                               { (yyval.sval) = strdup("real"); }
#line 2077 "y.tab.c"
    break;

  case 50: /* tipo: STRING  */
#line 634 "parserTaPyoCa.y"
                               { (yyval.sval) = strdup("string"); }
#line 2083 "y.tab.c"
    break;

  case 51: /* tipo: tipo LBRACKET RBRACKET  */
#line 635 "parserTaPyoCa.y"
                               { (yyval.sval) = fmt("%s[]", (yyvsp[-2].sval)); free((yyvsp[-2].sval)); }
#line 2089 "y.tab.c"
    break;

  case 52: /* expressao: exprelacional  */
#line 639 "parserTaPyoCa.y"
                    { (yyval.expr) = (yyvsp[0].expr); }
#line 2095 "y.tab.c"
    break;

  case 53: /* exprelacional: exparitimetica op_relacional exparitimetica  */
#line 644 "parserTaPyoCa.y"
        {
            (yyval.expr).texto = fmt("%s %s %s", (yyvsp[-2].expr).texto, (yyvsp[-1].sval), (yyvsp[0].expr).texto);
            (yyval.expr).tipo = verificar_relacional((yyvsp[-2].expr).tipo, (yyvsp[0].expr).tipo, (yyvsp[-1].sval), yylineno);
            free((yyvsp[-2].expr).texto); free((yyvsp[-1].sval)); free((yyvsp[0].expr).texto);
        }
#line 2105 "y.tab.c"
    break;

  case 54: /* exprelacional: exparitimetica  */
#line 649 "parserTaPyoCa.y"
                     { (yyval.expr) = (yyvsp[0].expr); }
#line 2111 "y.tab.c"
    break;

  case 55: /* op_relacional: LT  */
#line 653 "parserTaPyoCa.y"
           { (yyval.sval) = strdup("<");  }
#line 2117 "y.tab.c"
    break;

  case 56: /* op_relacional: LEQ  */
#line 654 "parserTaPyoCa.y"
           { (yyval.sval) = strdup("<="); }
#line 2123 "y.tab.c"
    break;

  case 57: /* op_relacional: GT  */
#line 655 "parserTaPyoCa.y"
           { (yyval.sval) = strdup(">");  }
#line 2129 "y.tab.c"
    break;

  case 58: /* op_relacional: GEQ  */
#line 656 "parserTaPyoCa.y"
           { (yyval.sval) = strdup(">="); }
#line 2135 "y.tab.c"
    break;

  case 59: /* op_relacional: EQ  */
#line 657 "parserTaPyoCa.y"
           { (yyval.sval) = strdup("=="); }
#line 2141 "y.tab.c"
    break;

  case 60: /* op_relacional: NEQ  */
#line 658 "parserTaPyoCa.y"
           { (yyval.sval) = strdup("!="); }
#line 2147 "y.tab.c"
    break;

  case 61: /* exparitimetica: exparitimetica PLUS termo  */
#line 663 "parserTaPyoCa.y"
        {
            (yyval.expr).texto = fmt("%s + %s", (yyvsp[-2].expr).texto, (yyvsp[0].expr).texto);
            (yyval.expr).tipo = verificar_binario_aritmetico((yyvsp[-2].expr).tipo, (yyvsp[0].expr).tipo, "+", yylineno);
            free((yyvsp[-2].expr).texto); free((yyvsp[0].expr).texto);
        }
#line 2157 "y.tab.c"
    break;

  case 62: /* exparitimetica: exparitimetica MINUS termo  */
#line 669 "parserTaPyoCa.y"
        {
            (yyval.expr).texto = fmt("%s - %s", (yyvsp[-2].expr).texto, (yyvsp[0].expr).texto);
            (yyval.expr).tipo = verificar_binario_aritmetico((yyvsp[-2].expr).tipo, (yyvsp[0].expr).tipo, "-", yylineno);
            free((yyvsp[-2].expr).texto); free((yyvsp[0].expr).texto);
        }
#line 2167 "y.tab.c"
    break;

  case 63: /* exparitimetica: termo  */
#line 674 "parserTaPyoCa.y"
            { (yyval.expr) = (yyvsp[0].expr); }
#line 2173 "y.tab.c"
    break;

  case 64: /* termo: termo MUL fator  */
#line 679 "parserTaPyoCa.y"
        {
            (yyval.expr).texto = fmt("%s * %s", (yyvsp[-2].expr).texto, (yyvsp[0].expr).texto);
            (yyval.expr).tipo = verificar_binario_aritmetico((yyvsp[-2].expr).tipo, (yyvsp[0].expr).tipo, "*", yylineno);
            free((yyvsp[-2].expr).texto); free((yyvsp[0].expr).texto);
        }
#line 2183 "y.tab.c"
    break;

  case 65: /* termo: termo DIV fator  */
#line 685 "parserTaPyoCa.y"
        {
            (yyval.expr).texto = fmt("%s / %s", (yyvsp[-2].expr).texto, (yyvsp[0].expr).texto);
            (yyval.expr).tipo = verificar_binario_aritmetico((yyvsp[-2].expr).tipo, (yyvsp[0].expr).tipo, "/", yylineno);
            free((yyvsp[-2].expr).texto); free((yyvsp[0].expr).texto);
        }
#line 2193 "y.tab.c"
    break;

  case 66: /* termo: fator  */
#line 690 "parserTaPyoCa.y"
            { (yyval.expr) = (yyvsp[0].expr); }
#line 2199 "y.tab.c"
    break;

  case 67: /* l_value: ID  */
#line 695 "parserTaPyoCa.y"
        {
            Simbolo *s = buscar_simbolo((yyvsp[0].sval));
            if (!s) {
                erro_semantico(yylineno, "Variavel '%s' nao declarada.", (yyvsp[0].sval));
                (yyval.expr).texto = (yyvsp[0].sval);
                (yyval.expr).tipo = criar_tipo(TIPO_ERRO, 0);
            } else if (s->is_func) {
                erro_semantico(yylineno, "'%s' e uma subrotina e nao pode ser usada como variavel.", (yyvsp[0].sval));
                (yyval.expr).texto = (yyvsp[0].sval);
                (yyval.expr).tipo = criar_tipo(TIPO_ERRO, 0);
            } else {
                (yyval.expr).tipo = s->tipo;
                if (s->is_ref) {
                    (yyval.expr).texto = fmt("(*%s)", (yyvsp[0].sval));
                    free((yyvsp[0].sval));
                } else {
                    (yyval.expr).texto = (yyvsp[0].sval);
                }
            }
        }
#line 2224 "y.tab.c"
    break;

  case 68: /* l_value: l_value LBRACKET expressao RBRACKET  */
#line 716 "parserTaPyoCa.y"
        {
            (yyval.expr).texto = fmt("%s[%s]", (yyvsp[-3].expr).texto, (yyvsp[-1].expr).texto);
            if ((yyvsp[-1].expr).tipo.base != TIPO_ERRO && ((yyvsp[-1].expr).tipo.base != TIPO_INT || (yyvsp[-1].expr).tipo.dimensao != 0)) {
                erro_semantico(yylineno, "Indice de array deve ser 'int', recebeu '%s'.",
                               tipo_para_string((yyvsp[-1].expr).tipo));
            }
            if ((yyvsp[-3].expr).tipo.base != TIPO_ERRO) {
                if ((yyvsp[-3].expr).tipo.dimensao <= 0) {
                    erro_semantico(yylineno, "Tentativa de indexar tipo nao-array '%s'.",
                                   tipo_para_string((yyvsp[-3].expr).tipo));
                    (yyval.expr).tipo = criar_tipo(TIPO_ERRO, 0);
                } else {
                    (yyval.expr).tipo = criar_tipo((yyvsp[-3].expr).tipo.base, (yyvsp[-3].expr).tipo.dimensao - 1);
                }
            } else {
                (yyval.expr).tipo = criar_tipo(TIPO_ERRO, 0);
            }
            free((yyvsp[-3].expr).texto); free((yyvsp[-1].expr).texto);
        }
#line 2248 "y.tab.c"
    break;

  case 69: /* fator: chamadafuncao  */
#line 738 "parserTaPyoCa.y"
                              { (yyval.expr) = (yyvsp[0].expr); }
#line 2254 "y.tab.c"
    break;

  case 70: /* fator: l_value  */
#line 739 "parserTaPyoCa.y"
                              { (yyval.expr) = (yyvsp[0].expr); }
#line 2260 "y.tab.c"
    break;

  case 71: /* fator: INT_NUM  */
#line 740 "parserTaPyoCa.y"
                              { (yyval.expr).texto = fmt("%d", (yyvsp[0].ival)); (yyval.expr).tipo = criar_tipo(TIPO_INT, 0); }
#line 2266 "y.tab.c"
    break;

  case 72: /* fator: REAL_NUM  */
#line 741 "parserTaPyoCa.y"
                              { (yyval.expr).texto = fmt("%g", (yyvsp[0].fval)); (yyval.expr).tipo = criar_tipo(TIPO_REAL, 0); }
#line 2272 "y.tab.c"
    break;

  case 73: /* fator: STRING_LITERAL  */
#line 742 "parserTaPyoCa.y"
                              { (yyval.expr).texto = (yyvsp[0].sval); (yyval.expr).tipo = criar_tipo(TIPO_STRING, 0); }
#line 2278 "y.tab.c"
    break;

  case 74: /* fator: arrayLiteral  */
#line 743 "parserTaPyoCa.y"
                              { (yyval.expr) = (yyvsp[0].expr); }
#line 2284 "y.tab.c"
    break;

  case 75: /* fator: LPAREN expressao RPAREN  */
#line 744 "parserTaPyoCa.y"
                              { (yyval.expr).texto = fmt("(%s)", (yyvsp[-1].expr).texto); (yyval.expr).tipo = (yyvsp[-1].expr).tipo; free((yyvsp[-1].expr).texto); }
#line 2290 "y.tab.c"
    break;

  case 76: /* arrayLiteral: LBRACKET array_elements_opt RBRACKET  */
#line 749 "parserTaPyoCa.y"
        {
            (yyval.expr).texto = fmt("[%s]", (yyvsp[-1].alist)->texto);
            if ((yyvsp[-1].alist)->count == 0) {
                erro_semantico(yylineno, "Array literal vazio nao permitido sem inferencia de tipo.");
                (yyval.expr).tipo = criar_tipo(TIPO_ERRO, 0);
            } else {
                Tipo elem_tipo = (yyvsp[-1].alist)->tipos[0];
                int erro_elem = 0;
                for (int i = 1; i < (yyvsp[-1].alist)->count; i++) {
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
                } else {
                    (yyval.expr).tipo = criar_tipo(elem_tipo.base, elem_tipo.dimensao + 1);
                }
            }
            free((yyvsp[-1].alist)->texto);
            free((yyvsp[-1].alist));
        }
#line 2325 "y.tab.c"
    break;

  case 77: /* array_elements_opt: lista_argumentos  */
#line 782 "parserTaPyoCa.y"
                       { (yyval.alist) = (yyvsp[0].alist); }
#line 2331 "y.tab.c"
    break;

  case 78: /* array_elements_opt: %empty  */
#line 784 "parserTaPyoCa.y"
        {
            ListaArgs *la = calloc(1, sizeof(ListaArgs));
            la->count = 0;
            la->texto = strdup("");
            (yyval.alist) = la;
        }
#line 2342 "y.tab.c"
    break;


#line 2346 "y.tab.c"

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
      yyerror (YY_("syntax error"));
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

  return yyresult;
}

#line 792 "parserTaPyoCa.y"


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
