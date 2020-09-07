#include <eight/parser.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <eight/node.h>
#include <eight/token.h>
#include <eight/lexer.h>

struct lookaheadlist;

typedef struct lookaheadlist {
    struct token* value;
    struct lookaheadlist* next;
} lookaheadlist_t;

struct parser {
    lexer_t* lexer;
    lookaheadlist_t* lookahead;
};

// Parser consume functions

// TODO(BSFishy): refactor this stuff to work more nicely
HEDLEY_PRIVATE
void syncPrivate(parser_t* p, lookaheadlist_t* la, unsigned int index) {
    if (HEDLEY_UNLIKELY(la == HEDLEY_NULL)) return;

    if (HEDLEY_UNLIKELY(la->value == HEDLEY_NULL)) {
        la->value = nextToken(p->lexer);
    }

    if (HEDLEY_UNLIKELY(index == 0 || la->value->type == TEOF)) return;

    if (HEDLEY_UNLIKELY(la->next == HEDLEY_NULL)) {
        lookaheadlist_t* next = malloc(sizeof(lookaheadlist_t));
        if (HEDLEY_UNLIKELY(!next))
            return;
        
        memset(next, 0, sizeof(lookaheadlist_t));
        next->value = nextToken(p->lexer);
        la->next = next;
    }

    syncPrivate(p, la->next, index-1);
}

HEDLEY_PRIVATE
void sync(parser_t* p, unsigned int index) {
    syncPrivate(p, p->lookahead, index);
}

HEDLEY_PRIVATE
token_t* laPrivate(parser_t* p, lookaheadlist_t* la, unsigned long index) {
    if (HEDLEY_UNLIKELY(la == HEDLEY_NULL || la->value == HEDLEY_NULL || la->value->type == TEOF)) return HEDLEY_NULL;

    if (HEDLEY_UNLIKELY(index == 0)) return la->value;

    if (HEDLEY_UNLIKELY(la->next == HEDLEY_NULL)) return HEDLEY_NULL;

    return laPrivate(p, la->next, index-1);
}

HEDLEY_PRIVATE
token_t* la(parser_t* p, unsigned long index) {
    sync(p, index);

    return laPrivate(p, p->lookahead, index);
}

HEDLEY_PRIVATE
tokengroup_t lg(parser_t* p, unsigned long index) {
    token_t* t = la(p, index);
    if (HEDLEY_UNLIKELY(!t))
        return TNone;
    
    return t->group;
}

HEDLEY_PRIVATE
tokentype_t lt(parser_t* p, unsigned long index) {
    token_t* t = la(p, index);
    if (HEDLEY_UNLIKELY(!t))
        return TEOF;
    
    return t->type;
}

HEDLEY_PRIVATE
void consumeP(parser_t* p) {
    if (HEDLEY_UNLIKELY(!p->lookahead)) return;

    lookaheadlist_t* tmp = p->lookahead;
    p->lookahead = p->lookahead->next;

    if (HEDLEY_UNLIKELY(p->lookahead == HEDLEY_NULL)) {
        lookaheadlist_t* la = malloc(sizeof(lookaheadlist_t));

        if (HEDLEY_LIKELY(la != HEDLEY_NULL)) {
            memset(la, 0, sizeof(lookaheadlist_t));
            p->lookahead = la;
        }
    }

    if (HEDLEY_LIKELY(tmp->value))
        freeToken(&tmp->value);
    free(tmp);
}

HEDLEY_PRIVATE
token_t* consumeToken(parser_t* p) {
    if (HEDLEY_UNLIKELY(!p->lookahead)) return HEDLEY_NULL;

    lookaheadlist_t* tmp = p->lookahead;
    p->lookahead = p->lookahead->next;

    token_t* value = tmp->value;
    free(tmp);

    return value;
}

HEDLEY_PRIVATE
bool match(parser_t* p, tokentype_t t) {
    if (HEDLEY_UNLIKELY(lt(p, 0) != t)) return false;

    consumeP(p);
    return true;
}

// Regular parser functions

parser_t* newParser() {
    parser_t* p = malloc(sizeof(parser_t));
    if (HEDLEY_UNLIKELY(!p))
        return HEDLEY_NULL;
    
    memset(p, 0, sizeof(parser_t));

    lookaheadlist_t* la = malloc(sizeof(lookaheadlist_t));
    if (HEDLEY_UNLIKELY(!la)) {
        free(p);
        return HEDLEY_NULL;
    }

    memset(la, 0, sizeof(lookaheadlist_t));
    p->lookahead = la;

    lexer_t* l = newLexer();
    if (HEDLEY_UNLIKELY(!l)) {
        free(p);
        return HEDLEY_NULL;
    }
    
    p->lexer = l;
    return p;
}

void prepareParser(parser_t* p, const char* contents) {
    prepareLexer(p->lexer, contents);
}

struct node* parseRoot(parser_t* p) {
    nodelist_t* list = newNodeList();
    if (HEDLEY_UNLIKELY(list == HEDLEY_NULL)) return HEDLEY_NULL;

    while (HEDLEY_LIKELY(lt(p, 0) != TEOF)) {
        node_t* n = parseFunction(p);

        if (HEDLEY_LIKELY(n != HEDLEY_NULL)) {
            if (HEDLEY_UNLIKELY(addNodeToEnd(list, n) == false)) {
                freeNodeList(&list);
                return HEDLEY_NULL;
            }
        } else {
            break;
        }
    }

    return createNode(NNone, NRoot, HEDLEY_NULL, list);
}

struct node* parseParameter(parser_t* p) {
    if (HEDLEY_UNLIKELY(lt(p, 0) != TIdentifier)) return HEDLEY_NULL;

    token_t* name = consumeToken(p);
    if (HEDLEY_UNLIKELY(!match(p, TColon))) {
        freeToken(&name);
        return HEDLEY_NULL;
    }

    if (HEDLEY_UNLIKELY(lt(p, 0) != TIdentifier)) {
        freeToken(&name);
        return HEDLEY_NULL;
    }

    token_t* type = consumeToken(p);
    tokenlist_t* tokens = createTokenList(2, name, type);
    if (HEDLEY_UNLIKELY(tokens == HEDLEY_NULL)) {
        freeToken(&name);
        freeToken(&type);
        return HEDLEY_NULL;
    }

    return createNode(NNone, NParameter, tokens, HEDLEY_NULL);
}

struct node* parseParameters(parser_t* p) {
    if (HEDLEY_UNLIKELY(!match(p, TLParen))) return HEDLEY_NULL;

    nodelist_t* list = newNodeList();
    if (HEDLEY_UNLIKELY(list == HEDLEY_NULL)) return HEDLEY_NULL;

    while (HEDLEY_LIKELY(lt(p, 0) == TIdentifier)) {
        node_t* n = parseParameter(p);

        if (HEDLEY_UNLIKELY(addNodeToEnd(list, n) == false)) {
            freeNodeList(&list);
            return HEDLEY_NULL;
        }
    }

    if (HEDLEY_UNLIKELY(!match(p, TRParen))) {
        freeNodeList(&list);
        return HEDLEY_NULL;
    }

    return createNode(NNone, NParameter, HEDLEY_NULL, list);
}

struct node* parseCodeBlock(parser_t* p) {
    if (HEDLEY_UNLIKELY(!match(p, TLBrace))) return HEDLEY_NULL;

    return HEDLEY_NULL;
}

struct node* parseFunction(parser_t* p) {
    if (HEDLEY_UNLIKELY(!match(p, TFunc))) return HEDLEY_NULL;
    
    if (HEDLEY_UNLIKELY(lt(p, 0) != TIdentifier)) return HEDLEY_NULL;

    token_t* name = consumeToken(p);

    node_t* params = parseParameters(p);
    if (HEDLEY_UNLIKELY(params == HEDLEY_NULL)) {
        freeToken(&name);
        return HEDLEY_NULL;
    }

    node_t* codeBlock = parseCodeBlock(p);

    tokenlist_t* tokens = createTokenList(1, name);
    if (HEDLEY_UNLIKELY(tokens == HEDLEY_NULL)) {
        freeToken(&name);
        return HEDLEY_NULL;
    }

    nodelist_t* nodes = createNodeList(2, params, codeBlock);
    if (HEDLEY_UNLIKELY(nodes == HEDLEY_NULL)) {
        freeToken(&name);
        freeTokenList(&tokens);
        return HEDLEY_NULL;
    }
    
    return createNode(NDeclaration, NFunction, tokens, nodes);
}

struct node* parseExpression(parser_t* p) {
    if (HEDLEY_LIKELY(lt(p, 0) == TIdentifier)) return parseCall(p);

    return parseLiteral(p);
}

struct node* parseCall(parser_t* p) {
    if (HEDLEY_UNLIKELY(lt(p, 0) != TIdentifier)) return HEDLEY_NULL;

    token_t* name = consumeToken(p);

    if (HEDLEY_UNLIKELY(!match(p, TLParen))) {
        freeToken(&name);
        return HEDLEY_NULL;
    }

    nodelist_t* list = newNodeList();
    if (HEDLEY_UNLIKELY(list == HEDLEY_NULL)) {
        freeToken(&name);
        return HEDLEY_NULL;
    }

    if (HEDLEY_LIKELY(lt(p, 0) != TRParen)) {
        node_t* expression = parseExpression(p);
        if (HEDLEY_UNLIKELY(expression == HEDLEY_NULL || addNodeToEnd(list, expression) == false)) {
            freeToken(&name);
            freeNodeList(&list);
            return HEDLEY_NULL;
        }

        while (HEDLEY_LIKELY(match(p, TComma))) {
            node_t* expression = parseExpression(p);
            if (HEDLEY_UNLIKELY(expression == HEDLEY_NULL || addNodeToEnd(list, expression) == false)) {
                freeToken(&name);
                freeNodeList(&list);
                return HEDLEY_NULL;
            }
        }
    }

    return createNode(NExpression, NCall, createTokenList(1, name), list);
}

struct node* parseLiteral(parser_t* p) {
    return parseString(p);
}

struct node* parseString(parser_t* p) {
    if (HEDLEY_UNLIKELY(lt(p, 0) == TString)) return HEDLEY_NULL;

    token_t* t = consumeToken(p);
    if (HEDLEY_UNLIKELY(t == HEDLEY_NULL)) return HEDLEY_NULL;

    return createNode(NExpression | NValue, NString, createTokenList(1, t), HEDLEY_NULL);
}

void freeLookaheadlist(lookaheadlist_t* l) {
    if (HEDLEY_LIKELY(l->value))
        freeToken(&l->value);
    
    if (HEDLEY_LIKELY(l->next))
        freeLookaheadlist(l->next);
    
    free(l);
}

void freeParser(parser_t** p) {
    freeLexer(&((*p)->lexer));
    if (HEDLEY_LIKELY((*p)->lookahead))
        freeLookaheadlist((*p)->lookahead);

    free(*p);
    *p = HEDLEY_NULL;
}
