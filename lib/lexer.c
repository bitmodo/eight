#include <eight/lexer.h>

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <eight/token.h>

struct lexer {
    unsigned long p;
    char c;

    unsigned long line;
    unsigned long column;

    size_t contentLength;
    const char* contents;
};

HEDLEY_PRIVATE
char* strmem(const char* str) {
    size_t size = strlen(str) + 1;
    char* dest = malloc(size);
    memset(dest, 0, size);

    return strcpy(dest, str);
}

lexer_t* newLexer() {
    lexer_t* l = malloc(sizeof(lexer_t));
    if (HEDLEY_UNLIKELY(!l))
        return HEDLEY_NULL;

    memset(l, 0, sizeof(lexer_t));
    return l;
}

void prepareLexer(lexer_t* l, const char* contents) {
    l->contents = contents;
    l->contentLength = strlen(l->contents);

    if (l->contentLength > 0) l->c = l->contents[0];
}

HEDLEY_PRIVATE
void updateCharacter(lexer_t* l) {
    if (l->p >= l->contentLength) l->c = 0;
    else l->c = l->contents[l->p];
}

HEDLEY_PRIVATE
void consume(lexer_t* l) {
    l->p++;
    l->column++;

    updateCharacter(l);
}

HEDLEY_PRIVATE
char* consumeCount(lexer_t* l, unsigned int amount) {
    size_t size = sizeof(char) * amount + 1;
    char* text = malloc(size);
    memset(text, 0, size);
    strncpy(text, l->contents + l->p, amount);

    l->p += amount;
    l->column += amount;
    updateCharacter(l);

    return text;
}

HEDLEY_PRIVATE
bool canLex(lexer_t* l, const char* text) {
    size_t len = strlen(text);
    return strncmp(l->contents + l->p, text, len) == 0;
}

HEDLEY_PRIVATE
void wsToken(lexer_t* l) {
    // TODO(BSFishy): Add some Headley macros

    while (l->c == ' ' || l->c == '\t' || l->c == '\n' || l->c == '\r') {
        if (l->c == '\n' || l->c == '\r') {
            l->line++;
            l->column = 0;
        }

        consume(l);
    }
}

HEDLEY_PRIVATE
struct token* newToken(lexer_t* l, enum tokengroup group, enum tokentype type, char* text) {
    struct token* t = malloc(sizeof(struct token));
    if (HEDLEY_UNLIKELY(!t))
        return HEDLEY_NULL;
    
    memset(t, 0, sizeof(struct token));
    t->group = group;
    t->type = type;

    t->line = l->line;
    t->column = l->column;

    t->text = text;

    return t;
}

HEDLEY_PRIVATE
bool identifierHeadChar(char c) {
    return isalpha(c);
}

HEDLEY_PRIVATE
bool identifierChar(char c) {
    return isalnum(c);
}

struct token* nextToken(lexer_t* l) {
    while (l->c != 0) {
        switch (l->c) {
            case ' ': case '\t': case '\n': case '\r': wsToken(l); continue;

            case '.': consume(l); return newToken(l, TNone, TPeriod, strmem("."));
            case ',': consume(l); return newToken(l, TNone, TComma, strmem(","));
            case ':': consume(l); return newToken(l, TNone, TColon, strmem(":"));
            
            case '(': consume(l); return newToken(l, TNone, TLParen, strmem("("));
            case ')': consume(l); return newToken(l, TNone, TRParen, strmem(")"));
            case '{': consume(l); return newToken(l, TNone, TLBrace, strmem("{"));
            case '}': consume(l); return newToken(l, TNone, TRBrace, strmem("}"));

            default:
                // TODO(BSFishy): refactor this to make it easier to read

                if (canLex(l, "func")) {
                    return newToken(l, TKeyword, TFunc, consumeCount(l, 4));
                }

                if (l->c == '"') {
                    unsigned long size = 0;

                    while (l->contents[l->p + size + 1] != '"') {
                        size++;
                    }

                    consume(l);
                    char* text = consumeCount(l, size);
                    consume(l);

                    return newToken(l, TNone, TString, text);
                }

                // Identifier
                if (identifierHeadChar(l->c)) {
                    unsigned long size = 0;

                    do {
                        size++;
                    } while (identifierChar(l->contents[l->p + size]));

                    return newToken(l, TNone, TIdentifier, consumeCount(l, size));
                }

                return HEDLEY_NULL;
        }
    }

    return newToken(l, TNone, TEOF, "\0");
}

void freeLexer(lexer_t** l) {
    free(*l);
    *l = HEDLEY_NULL;
}
