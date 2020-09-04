#ifndef EIGHT_AST_FUNCTION_H
#define EIGHT_AST_FUNCTION_H

#include <eight/api.h>

HEDLEY_BEGIN_C_DECLS

struct token;
struct node;
struct nodelist;

EIGHT_API
struct token* getFunctionName(struct node*);

EIGHT_API
struct nodelist* getFunctionParameters(struct node*);

EIGHT_API
struct node* getFunctionCodeBlock(struct node*);

HEDLEY_END_C_DECLS

#endif
