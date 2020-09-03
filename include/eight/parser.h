#ifndef EIGHT_PARSER_H
#define EIGHT_PARSER_H

#include <eight/api.h>

HEDLEY_BEGIN_C_DECLS

struct nodelist;

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
void freeParser(parser_t**);

HEDLEY_END_C_DECLS

#endif
