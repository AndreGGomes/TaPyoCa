/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

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

#line 76 "y.tab.h"

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

#line 156 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
