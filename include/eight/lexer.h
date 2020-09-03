#ifndef EIGHT_LEXER_H
#define EIGHT_LEXER_H

#include <eight/api.h>

HEDLEY_BEGIN_C_DECLS

struct token;

typedef struct lexer lexer_t;

EIGHT_API
lexer_t* newLexer();

EIGHT_API
void prepareLexer(lexer_t*, const char*);

EIGHT_API
struct token* nextToken(lexer_t*);

EIGHT_API
void freeLexer(lexer_t**);

HEDLEY_END_C_DECLS

#endif
