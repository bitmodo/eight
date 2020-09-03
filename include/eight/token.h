#ifndef EIGHT_TOKEN_H
#define EIGHT_TOKEN_H

#include <eight/api.h>

#include <stddef.h>

HEDLEY_BEGIN_C_DECLS

enum tokengroup {
    TNone = (1 << 0),

    TKeyword = (1 << 1),
    TOperator = (1 << 2),
} HEDLEY_FLAGS;

typedef enum tokengroup tokengroup_t;

typedef enum tokentype {
    TEOF = -1,

    TPeriod,
    TComma,
    TColon,

    TLParen,
    TRParen,
    TLBrace,
    TRBrace,

    TFunc,

    TString,
    TIdentifier,
} tokentype_t;

typedef struct token {
    enum tokengroup group;
    enum tokentype type;

    unsigned line;
    unsigned column;

    char* text;
} token_t;

typedef struct tokenlist tokenlist_t;

struct tokenlist {
    struct token* value;
    struct tokenlist* next;
};

EIGHT_API
struct token* createToken(tokengroup_t, tokentype_t, unsigned, unsigned, char*);

EIGHT_API
struct tokenlist* createTokenList(size_t, ...);

EIGHT_API
void freeToken(token_t**);

EIGHT_API
void freeTokenList(tokenlist_t**);

HEDLEY_END_C_DECLS

#endif
