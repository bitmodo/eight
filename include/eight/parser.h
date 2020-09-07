#ifndef EIGHT_PARSER_H
#define EIGHT_PARSER_H

#include <eight/api.h>

HEDLEY_BEGIN_C_DECLS

struct node;

typedef struct parser parser_t;

EIGHT_API
parser_t* newParser();

EIGHT_API
void prepareParser(parser_t*, const char*);

EIGHT_API
struct node* parseRoot(parser_t*);

EIGHT_API
struct node* parseFunction(parser_t*);

EIGHT_API
struct node* parseParameters(parser_t*);

EIGHT_API
struct node* parseParameter(parser_t*);

EIGHT_API
struct node* parseCodeBlock(parser_t*);

EIGHT_API
struct node* parseExpression(parser_t*);

EIGHT_API
struct node* parseCall(parser_t*);

EIGHT_API
struct node* parseLiteral(parser_t*);

EIGHT_API
struct node* parseString(parser_t*);

EIGHT_API
void freeParser(parser_t**);

HEDLEY_END_C_DECLS

#endif
