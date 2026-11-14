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
#line 17 "parserTaPyoCa.y"

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

/* ---------- estruturas auxiliares para o semantico ---------- */

static Tipo retorno_esperado;
static int encontrou_return = 0;


#line 117 "y.tab.c"

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
        int count;
        char *texto;
    } ListaParams;

    typedef struct {
        Tipo tipos[32];
        int count;
        char *texto;
    } ListaArgs;

#line 169 "y.tab.c"

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
    INT_NUM = 290,                 /* INT_NUM  */
    REAL_NUM = 291,                /* REAL_NUM  */
    ID = 292,                      /* ID  */
    STRING_LITERAL = 293           /* STRING_LITERAL  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 63 "parserTaPyoCa.y"

    int ival;
    double fval;
    char *sval;
    ExprInfo expr;
    ListaParams *plist;
    ListaArgs *alist;

#line 233 "y.tab.c"

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
  YYSYMBOL_INT_NUM = 35,                   /* INT_NUM  */
  YYSYMBOL_REAL_NUM = 36,                  /* REAL_NUM  */
  YYSYMBOL_ID = 37,                        /* ID  */
  YYSYMBOL_STRING_LITERAL = 38,            /* STRING_LITERAL  */
  YYSYMBOL_YYACCEPT = 39,                  /* $accept  */
  YYSYMBOL_programa = 40,                  /* programa  */
  YYSYMBOL_declaracoes_globais = 41,       /* declaracoes_globais  */
  YYSYMBOL_declaracao = 42,                /* declaracao  */
  YYSYMBOL_main_block = 43,                /* main_block  */
  YYSYMBOL_44_1 = 44,                      /* $@1  */
  YYSYMBOL_subrotina = 45,                 /* subrotina  */
  YYSYMBOL_46_2 = 46,                      /* $@2  */
  YYSYMBOL_tipo_opt = 47,                  /* tipo_opt  */
  YYSYMBOL_parametros_opt = 48,            /* parametros_opt  */
  YYSYMBOL_lista_parametros = 49,          /* lista_parametros  */
  YYSYMBOL_comandos = 50,                  /* comandos  */
  YYSYMBOL_corpo_bloco = 51,               /* corpo_bloco  */
  YYSYMBOL_comando = 52,                   /* comando  */
  YYSYMBOL_comandoprint = 53,              /* comandoprint  */
  YYSYMBOL_declaracaovar = 54,             /* declaracaovar  */
  YYSYMBOL_atribuicao = 55,                /* atribuicao  */
  YYSYMBOL_comandowhile = 56,              /* comandowhile  */
  YYSYMBOL_57_3 = 57,                      /* $@3  */
  YYSYMBOL_comandoif = 58,                 /* comandoif  */
  YYSYMBOL_59_4 = 59,                      /* $@4  */
  YYSYMBOL_retorno = 60,                   /* retorno  */
  YYSYMBOL_chamadafuncao = 61,             /* chamadafuncao  */
  YYSYMBOL_argumentos_opt = 62,            /* argumentos_opt  */
  YYSYMBOL_lista_argumentos = 63,          /* lista_argumentos  */
  YYSYMBOL_tipo = 64,                      /* tipo  */
  YYSYMBOL_expressao = 65,                 /* expressao  */
  YYSYMBOL_exprelacional = 66,             /* exprelacional  */
  YYSYMBOL_op_relacional = 67,             /* op_relacional  */
  YYSYMBOL_exparitimetica = 68,            /* exparitimetica  */
  YYSYMBOL_termo = 69,                     /* termo  */
  YYSYMBOL_l_value = 70,                   /* l_value  */
  YYSYMBOL_fator = 71,                     /* fator  */
  YYSYMBOL_arrayLiteral = 72,              /* arrayLiteral  */
  YYSYMBOL_array_elements_opt = 73         /* array_elements_opt  */
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
typedef yytype_int8 yy_state_t;

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
#define YYLAST   117

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  39
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  35
/* YYNRULES -- Number of rules.  */
#define YYNRULES  72
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  124

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   293


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
      35,    36,    37,    38
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    93,    93,   105,   106,   110,   111,   116,   115,   134,
     133,   175,   176,   180,   182,   191,   206,   219,   220,   224,
     225,   229,   230,   231,   232,   233,   234,   235,   239,   248,
     259,   269,   282,   281,   302,   301,   321,   336,   393,   395,
     404,   416,   427,   428,   429,   430,   434,   438,   444,   448,
     449,   450,   451,   452,   453,   457,   463,   469,   473,   479,
     485,   489,   503,   526,   527,   528,   529,   530,   531,   532,
     536,   570,   572
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
  "NEQ", "PLUS", "MINUS", "MUL", "DIV", "PRINT", "INT_NUM", "REAL_NUM",
  "ID", "STRING_LITERAL", "$accept", "programa", "declaracoes_globais",
  "declaracao", "main_block", "$@1", "subrotina", "$@2", "tipo_opt",
  "parametros_opt", "lista_parametros", "comandos", "corpo_bloco",
  "comando", "comandoprint", "declaracaovar", "atribuicao", "comandowhile",
  "$@3", "comandoif", "$@4", "retorno", "chamadafuncao", "argumentos_opt",
  "lista_argumentos", "tipo", "expressao", "exprelacional",
  "op_relacional", "exparitimetica", "termo", "l_value", "fator",
  "arrayLiteral", "array_elements_opt", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-71)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
       2,     6,    27,    20,     2,   -71,   -71,   -71,    19,   -71,
     -71,   -71,    -7,    12,   -71,   -71,   -71,    43,    39,    55,
      27,   -71,    57,    60,    50,   -19,   -71,    61,   -71,    27,
     -71,    14,    63,    65,   -18,   -71,     7,    68,    69,    70,
      71,    72,   -71,   -71,   -71,   -71,   -71,   -71,   -71,   -14,
      -6,   -71,    57,   -71,     7,     7,   -71,   -71,   -71,   -71,
     -71,   -71,    28,   -24,    56,   -71,   -71,     7,     7,     7,
       7,   -71,    64,     7,     7,    73,    74,    75,   -71,    59,
     -71,   -71,   -71,   -71,   -71,   -71,     7,     7,     7,     7,
       7,    76,    77,    78,    75,    80,     7,   -71,    67,    83,
     -71,     7,   -71,   -24,   -24,    -5,   -71,   -71,   -71,   -71,
     -71,   -71,   -71,   -71,   -71,   -71,    84,    85,    57,    57,
      79,    81,   -71,   -71
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,    12,     0,     2,     4,     6,     5,     0,    42,
      44,    43,     0,    11,     1,     3,     7,     0,     0,     0,
      14,    45,    20,     0,    13,     0,    18,     0,     9,     0,
      16,     0,     0,     0,     0,    19,     0,     0,     0,     0,
      61,     0,    27,    21,    22,    24,    23,    26,    25,     0,
       0,     8,    20,    15,     0,    72,    65,    66,    67,    63,
      36,    46,    48,    57,    64,    60,    68,     0,     0,    39,
      39,    17,    30,     0,     0,     0,     0,    71,    41,     0,
      49,    50,    51,    52,    53,    54,     0,     0,     0,     0,
       0,     0,     0,     0,    38,     0,     0,    31,     0,     0,
      69,     0,    70,    55,    56,    47,    58,    59,    34,    32,
      28,    37,    29,    62,    10,    40,     0,     0,    20,    20,
       0,     0,    35,    33
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -71,   -71,   -71,    82,   -71,   -71,   -71,   -71,   -71,   -71,
     -71,   -71,   -52,   -71,   -71,   -71,   -71,   -71,   -71,   -71,
     -71,   -71,    62,    22,    41,   -16,   -33,   -71,   -71,     9,
     -70,    86,   -51,   -71,   -71
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     3,     4,     5,     6,    19,     7,    33,    12,    23,
      24,    31,    27,    41,    42,    43,    44,    45,   117,    46,
     116,    47,    59,    93,    94,    13,    78,    61,    88,    62,
      63,    64,    65,    66,    79
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      75,    18,    18,    60,    25,     1,    18,    73,    89,    90,
       8,    54,     2,    34,    74,    49,   103,   104,    30,    53,
      14,    76,    35,    72,    16,    86,    87,    55,    36,    37,
      17,    38,    18,     9,    91,    92,    10,    11,   106,   107,
      97,    98,    56,    57,    40,    58,     9,    20,    39,    10,
      11,    40,    80,    81,    82,    83,    84,    85,    86,    87,
      21,    22,    29,   112,    26,    28,   120,   121,   115,    51,
      32,    52,    67,    68,    69,    70,    74,    96,    71,   100,
     102,   108,   109,   110,    99,   111,    15,   101,   113,   114,
     118,   119,    95,    48,     0,   122,    77,   105,     0,   123,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    50
};

static const yytype_int8 yycheck[] =
{
      52,    20,    20,    36,    20,     3,    20,    13,    32,    33,
       4,     4,    10,    29,    20,    31,    86,    87,    37,    37,
       0,    54,     8,    37,     5,    30,    31,    20,    14,    15,
      37,    17,    20,    19,    67,    68,    22,    23,    89,    90,
      73,    74,    35,    36,    37,    38,    19,     4,    34,    22,
      23,    37,    24,    25,    26,    27,    28,    29,    30,    31,
      21,     6,    12,    96,     7,     5,   118,   119,   101,     6,
       9,     6,     4,     4,     4,     4,    20,    13,     6,     5,
      21,     5,     5,     5,    11,     5,     4,    12,    21,     6,
       6,     6,    70,    31,    -1,    16,    55,    88,    -1,    18,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    10,    40,    41,    42,    43,    45,     4,    19,
      22,    23,    47,    64,     0,    42,     5,    37,    20,    44,
       4,    21,     6,    48,    49,    64,     7,    51,     5,    12,
      37,    50,     9,    46,    64,     8,    14,    15,    17,    34,
      37,    52,    53,    54,    55,    56,    58,    60,    61,    64,
      70,     6,     6,    37,     4,    20,    35,    36,    38,    61,
      65,    66,    68,    69,    70,    71,    72,     4,     4,     4,
       4,     6,    37,    13,    20,    51,    65,    63,    65,    73,
      24,    25,    26,    27,    28,    29,    30,    31,    67,    32,
      33,    65,    65,    62,    63,    62,    13,    65,    65,    11,
       5,    12,    21,    69,    69,    68,    71,    71,     5,     5,
       5,     5,    65,    21,     6,    65,    59,    57,     6,     6,
      51,    51,    16,    18
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    39,    40,    41,    41,    42,    42,    44,    43,    46,
      45,    47,    47,    48,    48,    49,    49,    50,    50,    51,
      51,    52,    52,    52,    52,    52,    52,    52,    53,    54,
      54,    55,    57,    56,    59,    58,    60,    61,    62,    62,
      63,    63,    64,    64,    64,    64,    65,    66,    66,    67,
      67,    67,    67,    67,    67,    68,    68,    68,    69,    69,
      69,    70,    70,    71,    71,    71,    71,    71,    71,    71,
      72,    73,    73
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     0,     8,     0,
      11,     1,     0,     1,     0,     4,     2,     3,     0,     3,
       0,     1,     1,     1,     1,     1,     1,     1,     4,     4,
       2,     3,     0,     8,     0,     8,     2,     4,     1,     0,
       3,     1,     1,     1,     1,     3,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     1,     3,     3,
       1,     1,     4,     1,     1,     1,     1,     1,     1,     3,
       3,     1,     0
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
#line 94 "parserTaPyoCa.y"
        {
            if (obter_erros_semanticos() == 0) {
                printf("Analise sintatica e semantica concluida com sucesso.\n");
            } else {
                fprintf(stderr, "Falha na compilacao: %d erro(s) semantico(s).\n", obter_erros_semanticos());
                YYABORT;
            }
        }
#line 1378 "y.tab.c"
    break;

  case 3: /* declaracoes_globais: declaracoes_globais declaracao  */
#line 105 "parserTaPyoCa.y"
                                     {}
#line 1384 "y.tab.c"
    break;

  case 4: /* declaracoes_globais: declaracao  */
#line 106 "parserTaPyoCa.y"
                                     {}
#line 1390 "y.tab.c"
    break;

  case 5: /* declaracao: subrotina  */
#line 110 "parserTaPyoCa.y"
                 {}
#line 1396 "y.tab.c"
    break;

  case 6: /* declaracao: main_block  */
#line 111 "parserTaPyoCa.y"
                 {}
#line 1402 "y.tab.c"
    break;

  case 7: /* $@1: %empty  */
#line 116 "parserTaPyoCa.y"
        {
            pr("[main]");
            depth++;
            retorno_esperado = criar_tipo(TIPO_VOID, 0);
            encontrou_return = 0;
            entrar_escopo();
        }
#line 1414 "y.tab.c"
    break;

  case 8: /* main_block: MAIN LPAREN RPAREN $@1 NEWLINE corpo_bloco ENDMAIN NEWLINE  */
#line 124 "parserTaPyoCa.y"
        {
            sair_escopo();
            depth--;
            pr("[endmain]");
            printf("\n");
        }
#line 1425 "y.tab.c"
    break;

  case 9: /* $@2: %empty  */
#line 134 "parserTaPyoCa.y"
        {
            if ((yyvsp[-4].sval)[0]) pr("[sub] %s %s(%s)", (yyvsp[-4].sval), (yyvsp[-3].sval), (yyvsp[-1].plist)->texto);
            else       pr("[sub] %s(%s)", (yyvsp[-3].sval), (yyvsp[-1].plist)->texto);
            depth++;

            Tipo ret = parse_tipo_string((yyvsp[-4].sval));
            Tipo *tipos_params = NULL;
            if ((yyvsp[-1].plist)->count > 0) {
                tipos_params = malloc(sizeof(Tipo) * (yyvsp[-1].plist)->count);
                for (int i = 0; i < (yyvsp[-1].plist)->count; i++) {
                    tipos_params[i] = (yyvsp[-1].plist)->tipos[i];
                }
            }
            /* Insere a subrotina no escopo global */
            inserir_funcao((yyvsp[-3].sval), ret, (yyvsp[-1].plist)->count, tipos_params, yylineno);

            /* Abre escopo local e declara os parametros */
            entrar_escopo();
            for (int i = 0; i < (yyvsp[-1].plist)->count; i++) {
                inserir_variavel((yyvsp[-1].plist)->nomes[i], (yyvsp[-1].plist)->tipos[i], yylineno);
                free((yyvsp[-1].plist)->nomes[i]);
            }
            retorno_esperado = ret;
            encontrou_return = 0;

            free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-1].plist)->texto); free((yyvsp[-1].plist));
        }
#line 1457 "y.tab.c"
    break;

  case 10: /* subrotina: SUB tipo_opt ID LPAREN parametros_opt RPAREN $@2 NEWLINE corpo_bloco ENDSUB NEWLINE  */
#line 162 "parserTaPyoCa.y"
        {
            if (retorno_esperado.base != TIPO_VOID && !encontrou_return) {
                erro_semantico(yylineno, "Subrotina com retorno '%s' deve possuir comando 'return'.",
                               tipo_para_string(retorno_esperado));
            }
            sair_escopo();
            depth--;
            pr("[endsub]");
            printf("\n");
        }
#line 1472 "y.tab.c"
    break;

  case 11: /* tipo_opt: tipo  */
#line 175 "parserTaPyoCa.y"
                   { (yyval.sval) = (yyvsp[0].sval); }
#line 1478 "y.tab.c"
    break;

  case 12: /* tipo_opt: %empty  */
#line 176 "parserTaPyoCa.y"
                   { (yyval.sval) = strdup(""); }
#line 1484 "y.tab.c"
    break;

  case 13: /* parametros_opt: lista_parametros  */
#line 180 "parserTaPyoCa.y"
                       { (yyval.plist) = (yyvsp[0].plist); }
#line 1490 "y.tab.c"
    break;

  case 14: /* parametros_opt: %empty  */
#line 182 "parserTaPyoCa.y"
        {
            ListaParams *lp = calloc(1, sizeof(ListaParams));
            lp->count = 0;
            lp->texto = strdup("");
            (yyval.plist) = lp;
        }
#line 1501 "y.tab.c"
    break;

  case 15: /* lista_parametros: lista_parametros COMMA tipo ID  */
#line 192 "parserTaPyoCa.y"
        {
            (yyval.plist) = (yyvsp[-3].plist);
            if ((yyval.plist)->count < 32) {
                int idx = (yyval.plist)->count++;
                (yyval.plist)->nomes[idx] = (yyvsp[0].sval);
                (yyval.plist)->tipos[idx] = parse_tipo_string((yyvsp[-1].sval));
            } else {
                erro_semantico(yylineno, "Limite maximo de 32 parametros excedido.");
                free((yyvsp[0].sval));
            }
            char *novo = fmt("%s, %s %s", (yyval.plist)->texto, (yyvsp[-1].sval), (yyval.plist)->nomes[(yyval.plist)->count - 1]);
            free((yyval.plist)->texto); free((yyvsp[-1].sval));
            (yyval.plist)->texto = novo;
        }
#line 1520 "y.tab.c"
    break;

  case 16: /* lista_parametros: tipo ID  */
#line 207 "parserTaPyoCa.y"
        {
            ListaParams *lp = calloc(1, sizeof(ListaParams));
            lp->nomes[0] = (yyvsp[0].sval);
            lp->tipos[0] = parse_tipo_string((yyvsp[-1].sval));
            lp->count = 1;
            lp->texto = fmt("%s %s", (yyvsp[-1].sval), (yyvsp[0].sval));
            free((yyvsp[-1].sval));
            (yyval.plist) = lp;
        }
#line 1534 "y.tab.c"
    break;

  case 17: /* comandos: comandos comando NEWLINE  */
#line 219 "parserTaPyoCa.y"
                               {}
#line 1540 "y.tab.c"
    break;

  case 18: /* comandos: %empty  */
#line 220 "parserTaPyoCa.y"
                               {}
#line 1546 "y.tab.c"
    break;

  case 19: /* corpo_bloco: INDENT comandos DEDENT  */
#line 224 "parserTaPyoCa.y"
                             {}
#line 1552 "y.tab.c"
    break;

  case 20: /* corpo_bloco: %empty  */
#line 225 "parserTaPyoCa.y"
                             {}
#line 1558 "y.tab.c"
    break;

  case 21: /* comando: declaracaovar  */
#line 229 "parserTaPyoCa.y"
                    {}
#line 1564 "y.tab.c"
    break;

  case 22: /* comando: atribuicao  */
#line 230 "parserTaPyoCa.y"
                    {}
#line 1570 "y.tab.c"
    break;

  case 23: /* comando: comandoif  */
#line 231 "parserTaPyoCa.y"
                    {}
#line 1576 "y.tab.c"
    break;

  case 24: /* comando: comandowhile  */
#line 232 "parserTaPyoCa.y"
                    {}
#line 1582 "y.tab.c"
    break;

  case 25: /* comando: chamadafuncao  */
#line 233 "parserTaPyoCa.y"
                    { pr("[chamada] %s", (yyvsp[0].expr).texto); free((yyvsp[0].expr).texto); }
#line 1588 "y.tab.c"
    break;

  case 26: /* comando: retorno  */
#line 234 "parserTaPyoCa.y"
                    {}
#line 1594 "y.tab.c"
    break;

  case 27: /* comando: comandoprint  */
#line 235 "parserTaPyoCa.y"
                    {}
#line 1600 "y.tab.c"
    break;

  case 28: /* comandoprint: PRINT LPAREN argumentos_opt RPAREN  */
#line 240 "parserTaPyoCa.y"
        {
            pr("[print] print %s", (yyvsp[-1].alist)->texto);
            free((yyvsp[-1].alist)->texto);
            free((yyvsp[-1].alist));
        }
#line 1610 "y.tab.c"
    break;

  case 29: /* declaracaovar: tipo ID ASSIGN expressao  */
#line 249 "parserTaPyoCa.y"
        {
            pr("[decl] %s %s <- %s", (yyvsp[-3].sval), (yyvsp[-2].sval), (yyvsp[0].expr).texto);
            Tipo t_var = parse_tipo_string((yyvsp[-3].sval));
            inserir_variavel((yyvsp[-2].sval), t_var, yylineno);
            if (!tipos_compativeis(t_var, (yyvsp[0].expr).tipo)) {
                erro_semantico(yylineno, "Nao e possivel inicializar '%s' (%s) com tipo '%s'.",
                               (yyvsp[-2].sval), tipo_para_string(t_var), tipo_para_string((yyvsp[0].expr).tipo));
            }
            free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[0].expr).texto);
        }
#line 1625 "y.tab.c"
    break;

  case 30: /* declaracaovar: tipo ID  */
#line 260 "parserTaPyoCa.y"
        {
            pr("[decl] %s %s", (yyvsp[-1].sval), (yyvsp[0].sval));
            Tipo t_var = parse_tipo_string((yyvsp[-1].sval));
            inserir_variavel((yyvsp[0].sval), t_var, yylineno);
            free((yyvsp[-1].sval)); free((yyvsp[0].sval));
        }
#line 1636 "y.tab.c"
    break;

  case 31: /* atribuicao: l_value ASSIGN expressao  */
#line 270 "parserTaPyoCa.y"
        {
            pr("[atrib] %s <- %s", (yyvsp[-2].expr).texto, (yyvsp[0].expr).texto);
            if (!tipos_compativeis((yyvsp[-2].expr).tipo, (yyvsp[0].expr).tipo)) {
                erro_semantico(yylineno, "Atribuicao incompativel: '%s' espera '%s', mas recebeu '%s'.",
                               (yyvsp[-2].expr).texto, tipo_para_string((yyvsp[-2].expr).tipo), tipo_para_string((yyvsp[0].expr).tipo));
            }
            free((yyvsp[-2].expr).texto); free((yyvsp[0].expr).texto);
        }
#line 1649 "y.tab.c"
    break;

  case 32: /* $@3: %empty  */
#line 282 "parserTaPyoCa.y"
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
#line 1664 "y.tab.c"
    break;

  case 33: /* comandowhile: WHILE LPAREN expressao RPAREN $@3 NEWLINE corpo_bloco ENDWHILE  */
#line 293 "parserTaPyoCa.y"
        {
            sair_escopo();
            depth--;
            pr("[endwhile]");
        }
#line 1674 "y.tab.c"
    break;

  case 34: /* $@4: %empty  */
#line 302 "parserTaPyoCa.y"
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
#line 1689 "y.tab.c"
    break;

  case 35: /* comandoif: IF LPAREN expressao RPAREN $@4 NEWLINE corpo_bloco ENDIF  */
#line 313 "parserTaPyoCa.y"
        {
            sair_escopo();
            depth--;
            pr("[endif]");
        }
#line 1699 "y.tab.c"
    break;

  case 36: /* retorno: RETURN expressao  */
#line 322 "parserTaPyoCa.y"
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
#line 1715 "y.tab.c"
    break;

  case 37: /* chamadafuncao: ID LPAREN argumentos_opt RPAREN  */
#line 337 "parserTaPyoCa.y"
        {
            (yyval.expr).texto = fmt("%s(%s)", (yyvsp[-3].sval), (yyvsp[-1].alist)->texto);

            /* 1. Built-in: comprimento(array) -> int */
            if (strcmp((yyvsp[-3].sval), "comprimento") == 0) {
                if ((yyvsp[-1].alist)->count != 1) {
                    erro_semantico(yylineno, "'comprimento' espera 1 argumento, recebeu %d.", (yyvsp[-1].alist)->count);
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
                } else if (!tipos_compativeis((yyvsp[-1].alist)->tipos[0], (yyvsp[-1].alist)->tipos[1]) ||
                           !tipos_compativeis((yyvsp[-1].alist)->tipos[1], (yyvsp[-1].alist)->tipos[0])) {
                    erro_semantico(yylineno, "'swap' requer argumentos do mesmo tipo, recebeu '%s' e '%s'.",
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
                            if (!tipos_compativeis(s->params[i], (yyvsp[-1].alist)->tipos[i])) {
                                erro_semantico(yylineno, "Argumento %d de '%s' invalido: esperado '%s', recebido '%s'.",
                                               i + 1, (yyvsp[-3].sval), tipo_para_string(s->params[i]), tipo_para_string((yyvsp[-1].alist)->tipos[i]));
                            }
                        }
                    }
                    (yyval.expr).tipo = s->tipo;
                }
            }
            free((yyvsp[-3].sval)); free((yyvsp[-1].alist)->texto); free((yyvsp[-1].alist));
        }
#line 1773 "y.tab.c"
    break;

  case 38: /* argumentos_opt: lista_argumentos  */
#line 393 "parserTaPyoCa.y"
                       { (yyval.alist) = (yyvsp[0].alist); }
#line 1779 "y.tab.c"
    break;

  case 39: /* argumentos_opt: %empty  */
#line 395 "parserTaPyoCa.y"
        {
            ListaArgs *la = calloc(1, sizeof(ListaArgs));
            la->count = 0;
            la->texto = strdup("");
            (yyval.alist) = la;
        }
#line 1790 "y.tab.c"
    break;

  case 40: /* lista_argumentos: lista_argumentos COMMA expressao  */
#line 405 "parserTaPyoCa.y"
        {
            (yyval.alist) = (yyvsp[-2].alist);
            if ((yyval.alist)->count < 32) {
                (yyval.alist)->tipos[(yyval.alist)->count++] = (yyvsp[0].expr).tipo;
            } else {
                erro_semantico(yylineno, "Limite maximo de 32 argumentos/elementos excedido.");
            }
            char *novo = fmt("%s, %s", (yyval.alist)->texto, (yyvsp[0].expr).texto);
            free((yyval.alist)->texto); free((yyvsp[0].expr).texto);
            (yyval.alist)->texto = novo;
        }
#line 1806 "y.tab.c"
    break;

  case 41: /* lista_argumentos: expressao  */
#line 417 "parserTaPyoCa.y"
        {
            ListaArgs *la = calloc(1, sizeof(ListaArgs));
            la->tipos[0] = (yyvsp[0].expr).tipo;
            la->count = 1;
            la->texto = (yyvsp[0].expr).texto;
            (yyval.alist) = la;
        }
#line 1818 "y.tab.c"
    break;

  case 42: /* tipo: INT  */
#line 427 "parserTaPyoCa.y"
                               { (yyval.sval) = strdup("int"); }
#line 1824 "y.tab.c"
    break;

  case 43: /* tipo: REAL  */
#line 428 "parserTaPyoCa.y"
                               { (yyval.sval) = strdup("real"); }
#line 1830 "y.tab.c"
    break;

  case 44: /* tipo: STRING  */
#line 429 "parserTaPyoCa.y"
                               { (yyval.sval) = strdup("string"); }
#line 1836 "y.tab.c"
    break;

  case 45: /* tipo: tipo LBRACKET RBRACKET  */
#line 430 "parserTaPyoCa.y"
                               { (yyval.sval) = fmt("%s[]", (yyvsp[-2].sval)); free((yyvsp[-2].sval)); }
#line 1842 "y.tab.c"
    break;

  case 46: /* expressao: exprelacional  */
#line 434 "parserTaPyoCa.y"
                    { (yyval.expr) = (yyvsp[0].expr); }
#line 1848 "y.tab.c"
    break;

  case 47: /* exprelacional: exparitimetica op_relacional exparitimetica  */
#line 439 "parserTaPyoCa.y"
        {
            (yyval.expr).texto = fmt("%s %s %s", (yyvsp[-2].expr).texto, (yyvsp[-1].sval), (yyvsp[0].expr).texto);
            (yyval.expr).tipo = verificar_relacional((yyvsp[-2].expr).tipo, (yyvsp[0].expr).tipo, (yyvsp[-1].sval), yylineno);
            free((yyvsp[-2].expr).texto); free((yyvsp[-1].sval)); free((yyvsp[0].expr).texto);
        }
#line 1858 "y.tab.c"
    break;

  case 48: /* exprelacional: exparitimetica  */
#line 444 "parserTaPyoCa.y"
                     { (yyval.expr) = (yyvsp[0].expr); }
#line 1864 "y.tab.c"
    break;

  case 49: /* op_relacional: LT  */
#line 448 "parserTaPyoCa.y"
           { (yyval.sval) = strdup("<");  }
#line 1870 "y.tab.c"
    break;

  case 50: /* op_relacional: LEQ  */
#line 449 "parserTaPyoCa.y"
           { (yyval.sval) = strdup("<="); }
#line 1876 "y.tab.c"
    break;

  case 51: /* op_relacional: GT  */
#line 450 "parserTaPyoCa.y"
           { (yyval.sval) = strdup(">");  }
#line 1882 "y.tab.c"
    break;

  case 52: /* op_relacional: GEQ  */
#line 451 "parserTaPyoCa.y"
           { (yyval.sval) = strdup(">="); }
#line 1888 "y.tab.c"
    break;

  case 53: /* op_relacional: EQ  */
#line 452 "parserTaPyoCa.y"
           { (yyval.sval) = strdup("=="); }
#line 1894 "y.tab.c"
    break;

  case 54: /* op_relacional: NEQ  */
#line 453 "parserTaPyoCa.y"
           { (yyval.sval) = strdup("!="); }
#line 1900 "y.tab.c"
    break;

  case 55: /* exparitimetica: exparitimetica PLUS termo  */
#line 458 "parserTaPyoCa.y"
        {
            (yyval.expr).texto = fmt("%s + %s", (yyvsp[-2].expr).texto, (yyvsp[0].expr).texto);
            (yyval.expr).tipo = verificar_binario_aritmetico((yyvsp[-2].expr).tipo, (yyvsp[0].expr).tipo, "+", yylineno);
            free((yyvsp[-2].expr).texto); free((yyvsp[0].expr).texto);
        }
#line 1910 "y.tab.c"
    break;

  case 56: /* exparitimetica: exparitimetica MINUS termo  */
#line 464 "parserTaPyoCa.y"
        {
            (yyval.expr).texto = fmt("%s - %s", (yyvsp[-2].expr).texto, (yyvsp[0].expr).texto);
            (yyval.expr).tipo = verificar_binario_aritmetico((yyvsp[-2].expr).tipo, (yyvsp[0].expr).tipo, "-", yylineno);
            free((yyvsp[-2].expr).texto); free((yyvsp[0].expr).texto);
        }
#line 1920 "y.tab.c"
    break;

  case 57: /* exparitimetica: termo  */
#line 469 "parserTaPyoCa.y"
            { (yyval.expr) = (yyvsp[0].expr); }
#line 1926 "y.tab.c"
    break;

  case 58: /* termo: termo MUL fator  */
#line 474 "parserTaPyoCa.y"
        {
            (yyval.expr).texto = fmt("%s * %s", (yyvsp[-2].expr).texto, (yyvsp[0].expr).texto);
            (yyval.expr).tipo = verificar_binario_aritmetico((yyvsp[-2].expr).tipo, (yyvsp[0].expr).tipo, "*", yylineno);
            free((yyvsp[-2].expr).texto); free((yyvsp[0].expr).texto);
        }
#line 1936 "y.tab.c"
    break;

  case 59: /* termo: termo DIV fator  */
#line 480 "parserTaPyoCa.y"
        {
            (yyval.expr).texto = fmt("%s / %s", (yyvsp[-2].expr).texto, (yyvsp[0].expr).texto);
            (yyval.expr).tipo = verificar_binario_aritmetico((yyvsp[-2].expr).tipo, (yyvsp[0].expr).tipo, "/", yylineno);
            free((yyvsp[-2].expr).texto); free((yyvsp[0].expr).texto);
        }
#line 1946 "y.tab.c"
    break;

  case 60: /* termo: fator  */
#line 485 "parserTaPyoCa.y"
            { (yyval.expr) = (yyvsp[0].expr); }
#line 1952 "y.tab.c"
    break;

  case 61: /* l_value: ID  */
#line 490 "parserTaPyoCa.y"
        {
            Simbolo *s = buscar_simbolo((yyvsp[0].sval));
            (yyval.expr).texto = (yyvsp[0].sval);
            if (!s) {
                erro_semantico(yylineno, "Variavel '%s' nao declarada.", (yyvsp[0].sval));
                (yyval.expr).tipo = criar_tipo(TIPO_ERRO, 0);
            } else if (s->is_func) {
                erro_semantico(yylineno, "'%s' e uma subrotina e nao pode ser usada como variavel.", (yyvsp[0].sval));
                (yyval.expr).tipo = criar_tipo(TIPO_ERRO, 0);
            } else {
                (yyval.expr).tipo = s->tipo;
            }
        }
#line 1970 "y.tab.c"
    break;

  case 62: /* l_value: l_value LBRACKET expressao RBRACKET  */
#line 504 "parserTaPyoCa.y"
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
#line 1994 "y.tab.c"
    break;

  case 63: /* fator: chamadafuncao  */
#line 526 "parserTaPyoCa.y"
                              { (yyval.expr) = (yyvsp[0].expr); }
#line 2000 "y.tab.c"
    break;

  case 64: /* fator: l_value  */
#line 527 "parserTaPyoCa.y"
                              { (yyval.expr) = (yyvsp[0].expr); }
#line 2006 "y.tab.c"
    break;

  case 65: /* fator: INT_NUM  */
#line 528 "parserTaPyoCa.y"
                              { (yyval.expr).texto = fmt("%d", (yyvsp[0].ival)); (yyval.expr).tipo = criar_tipo(TIPO_INT, 0); }
#line 2012 "y.tab.c"
    break;

  case 66: /* fator: REAL_NUM  */
#line 529 "parserTaPyoCa.y"
                              { (yyval.expr).texto = fmt("%g", (yyvsp[0].fval)); (yyval.expr).tipo = criar_tipo(TIPO_REAL, 0); }
#line 2018 "y.tab.c"
    break;

  case 67: /* fator: STRING_LITERAL  */
#line 530 "parserTaPyoCa.y"
                              { (yyval.expr).texto = (yyvsp[0].sval); (yyval.expr).tipo = criar_tipo(TIPO_STRING, 0); }
#line 2024 "y.tab.c"
    break;

  case 68: /* fator: arrayLiteral  */
#line 531 "parserTaPyoCa.y"
                              { (yyval.expr) = (yyvsp[0].expr); }
#line 2030 "y.tab.c"
    break;

  case 69: /* fator: LPAREN expressao RPAREN  */
#line 532 "parserTaPyoCa.y"
                              { (yyval.expr).texto = fmt("(%s)", (yyvsp[-1].expr).texto); (yyval.expr).tipo = (yyvsp[-1].expr).tipo; free((yyvsp[-1].expr).texto); }
#line 2036 "y.tab.c"
    break;

  case 70: /* arrayLiteral: LBRACKET array_elements_opt RBRACKET  */
#line 537 "parserTaPyoCa.y"
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
#line 2071 "y.tab.c"
    break;

  case 71: /* array_elements_opt: lista_argumentos  */
#line 570 "parserTaPyoCa.y"
                       { (yyval.alist) = (yyvsp[0].alist); }
#line 2077 "y.tab.c"
    break;

  case 72: /* array_elements_opt: %empty  */
#line 572 "parserTaPyoCa.y"
        {
            ListaArgs *la = calloc(1, sizeof(ListaArgs));
            la->count = 0;
            la->texto = strdup("");
            (yyval.alist) = la;
        }
#line 2088 "y.tab.c"
    break;


#line 2092 "y.tab.c"

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

#line 580 "parserTaPyoCa.y"


int main(void) {
    iniciar_semantico();
    return yyparse();
}

int yyerror(char *msg) {
    fprintf(stderr, "Linha %d: %s at '%s'\n", yylineno, msg, yytext);
    return 0;
}
