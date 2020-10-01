%{

#include "parser.h"

#include <stdlib.h>
#include <string.h>

/* Initialize LOC. */
# define LOCATION_RESET(Loc)                  \
  (Loc).first_column = (Loc).first_line = 1;  \
  (Loc).last_column =  (Loc).last_line = 1;

/* Advance of NUM lines. */
# define LOCATION_LINES(Loc, Num)             \
  (Loc).last_column = 1;                      \
  (Loc).last_line += Num;

/* Restart: move the first cursor to the last position. */
# define LOCATION_STEP(Loc)                   \
  (Loc).first_column = (Loc).last_column;     \
  (Loc).first_line = (Loc).last_line;

/* Each time we match a string, move the end cursor to its end. */
#define YY_USER_ACTION  yylloc->last_column += yyleng;

void yyerror(YYLTYPE*, void*, const char*);

#ifndef strdup
char* strdup(const char* str) {
    char* ret = malloc(sizeof(char) * (strlen(str)+1));
    if (!ret) return NULL;

    const char* i = str;
    char* d = ret;
    do {
        *(d++) = *i;
    } while (*(i++) != '\0');

    return ret;
}
#endif

%}

%option reentrant batch
%option nounistd noyywrap nounput noinput never-interactive
%option bison-bridge bison-locations

IDENTIFIER_HEAD [a-zA-Z_]

%%

%{
  /* At each yylex invocation, mark the current position as the
     start of the next token.  */
  LOCATION_STEP (*yylloc);
%}

[ \t]+ { LOCATION_STEP(*yylloc); }
\r+ ;
\n+ { LOCATION_LINES(*yylloc, yyleng); LOCATION_STEP(*yylloc); }

"." { return PERIOD; }
"," { return COMMA; }
":" { return COLON; }

"(" { return LPAREN; }
")" { return RPAREN; }

"{" { return LBRACE; }
"}" { return RBRACE; }

"->" { return ARROW; }

"func" { return FUNC; }
"throws" { return THROWS; }

\"[^"]*\" { yylval->string = strdup(yytext); return STRING; }
{IDENTIFIER_HEAD}({IDENTIFIER_HEAD}|[0-9])* { yylval->string = strdup(yytext); return IDENTIFIER; }

. { yyerror(yylloc, yyscanner, "Unexpected token"); return YYerror; }

%%
