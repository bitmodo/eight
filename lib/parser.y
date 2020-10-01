%{

#include "parser.h"
#include "lexer.h"

#include <eight/ast/expression.h>
#include <eight/ast/statement.h>
#include <eight/ast/structure.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void yyerror(YYLTYPE *locp, void* scan, struct array** output, const char *s);

#define UNIMPLEMENTED(feature) yyerror(&yyloc, yyscanner, output, feature " is not implemented yet"); YYERROR

#define ARRAY_ALLOC(type, arr, val) \
    arr = malloc(sizeof(struct array)); \
    memset(arr, 0, sizeof(struct array)); \
    arr->count = 1; \
    arr->value = malloc(sizeof(type *)); \
    arr->value[0] = val

#define ARRAY_APPEND(type, arr, old, new) \
    arr = old; \
    arr->value = realloc(arr->value, sizeof(type *) * (arr->count+1)); \
    arr->value[arr->count++] = new

%}

%locations
%define api.pure full
%define parse.error verbose
%start file

// %define parse.lac full
// %define parse.trace
// %verbose

%param {void* yyscanner}
%parse-param {struct array** output}

%union {
    _Bool* boolean;
    char* string;
    void* pointer;
    struct array* array;
    struct expression* expression;
    struct statement* statement;
    struct structure* structure;
}

%token <string> STRING
%token <string> IDENTIFIER

%token PERIOD "."
%token COMMA ","
%token COLON ":"

%token LPAREN "("
%token RPAREN ")"

%token LBRACE "{"
%token RBRACE "}"

%token ARROW "->"

%token FUNC "func"
%token THROWS "throws"

%%

file: root
    {
        *output = $<array>1;
    } ;

root: function
        {
            $<array>$ = malloc(sizeof(struct array));
            memset($<array>$, 0, sizeof(struct array));
            $<array>$->count = 1;
            $<array>$->value = malloc(sizeof(structure_t*));
            $<array>$->value[0] = $<statement>1;
        }
    | root function
        {
            $<array>$ = $<array>1;
            $<array>$->value = realloc($<array>$->value, sizeof(structure_t*) * ($<array>$->count+1));
            $<array>$->value[$<array>$->count++] = $<structure>2;
        } ;

codeblock: LBRACE statements RBRACE { $<array>$ = $<array>2; } ;

type: IDENTIFIER
    {
        $<pointer>$ = malloc(sizeof(type_t));
        ((type_t*) $<pointer>$)->name = $1;
    } ;

function: functionHead
        {
            $<structure>$ = $<structure>1;
        }
    | functionHead codeblock
        {
            $<structure>$ = $<structure>1;
            $<structure>$->data.function->statementCount = $<array>2->count;
            $<structure>$->data.function->statements = (statement_t**) $<array>2->value;
            free($<array>2);
        } ;
functionHead: FUNC IDENTIFIER functionSignature
    {
        $<structure>$ = allocStructure(StctFunction);
        $<structure>$->name = $2;
        $<structure>$->data.function->params = (functionparameters_t*) $<pointer>3;
        // $<structure>$->data.function->parameterCount = $<array>3->count;
        // $<structure>$->data.function->parameters = (parameter_t**) $<array>3->value;
        // free($<array>3);
    } ;
functionSignature: functionParameters functionOutput
        {
            UNIMPLEMENTED("Function output");
        }
    | functionParameters
        {
            $<pointer>$ = $<pointer>1;
        } ;
functionParameters: LPAREN parameters elipses RPAREN
        {
            $<pointer>$ = malloc(sizeof(functionparameters_t));
            memset($<pointer>$, 0, sizeof(functionparameters_t));
            ((functionparameters_t*) $<pointer>$)->variadic = *$<boolean>3;
            free($<boolean>3);
            ((functionparameters_t*) $<pointer>$)->count = $<array>2->count;
            ((functionparameters_t*) $<pointer>$)->array = (parameter_t**) $<array>2->value;
            free($<array>2);
            // $<array>$ = $<array>2;
        }
    | LPAREN RPAREN
        {
            $<pointer>$ = malloc(sizeof(functionparameters_t));
            memset($<pointer>$, 0, sizeof(functionparameters_t));
            // $<array>$ = malloc(sizeof(struct array));
            // memset($<array>$, 0, sizeof(struct array));
        } ;
functionOutput: functionThrow functionReturn | functionReturn | functionThrow ;
functionReturn: ARROW type { free($<string>2); UNIMPLEMENTED("Return type"); } ;
functionThrow: THROWS type { free($<string>2); UNIMPLEMENTED("Throwing"); } ;

parameter: IDENTIFIER COLON type
    { 
        $<pointer>$ = malloc(sizeof(parameter_t));
        ((parameter_t*) $<pointer>$)->type = *((type_t*) $<pointer>3);
        free($<pointer>3);
        ((parameter_t*) $<pointer>$)->name = $1;
    } ;
parameters: parameter
        {
            ARRAY_ALLOC(parameter_t, $<array>$, $<pointer>1);
        }
    | parameter COMMA parameters
        {
            ARRAY_APPEND(parameter_t, $<array>$, $<array>1, $<pointer>3);
        } ;
elipses: %empty
        {
            $<boolean>$ = malloc(sizeof(bool));
            memset($<boolean>$, 0, sizeof(bool));
            *$<boolean>$ = false;
        }
    | PERIOD PERIOD PERIOD
        {
            $<boolean>$ = malloc(sizeof(bool));
            memset($<boolean>$, 0, sizeof(bool));
            *$<boolean>$ = true;
        } ;

statement: expression { $<statement>$ = allocStatement(StmtExpression); $<statement>$->data.exp = $<expression>1; } ;
statements: statement
        {
            ARRAY_ALLOC(statement_t, $<array>$, $<statement>1);
        }
    | statements statement
        {
            ARRAY_APPEND(statement_t, $<array>$, $<array>1, $<statement>2);
        } ;

expression: callExpression | literal | symbolExpression ;

symbolExpression: IDENTIFIER { $<expression>$ = allocExpression(ExpSymbol); $<expression>$->data.symbol = $1; } ;

callExpression: expression LPAREN arguments RPAREN
    {
        $<expression>$ = allocExpression(ExpCall);
        $<expression>$->data.call->head = $<expression>1;
        $<expression>$->data.call->argc = $<array>3->count;
        $<expression>$->data.call->argv = (expression_t**) $<array>3->value;
        free($<array>3);
    } ;
argument: expression ;
arguments: argument
        {
            $<array>$ = malloc(sizeof(struct array));
            memset($<array>$, 0, sizeof(struct array));
            $<array>$->count = 1;
            $<array>$->value = malloc(sizeof(expression_t*));
            $<array>$->value[0] = $<expression>1;
        }
    | arguments COMMA argument
        {
            $<array>$ = $<array>1;
            $<array>$->value = realloc($<array>$->value, sizeof(expression_t*) * ($<array>$->count+1));
            $<array>$->value[$<array>$->count++] = $<expression>3;
        } ;

literal: STRING { $<expression>$ = allocLiteral(LitString); $<expression>$->data.literal->data.string = $1; } ;

%%

void yyerror(YYLTYPE *locp, void* scan, struct array** output, const char* s) {
    fprintf(stderr, "Error at %d:%d-%d:%d: %s\n", locp->first_line, locp->first_column, locp->last_line, locp->last_column, s);
}
