#include "eight/parser.h"
#include "eight/lexer.h"
#include "eight/parser/token.h"

#include "eight/exception.h"

#include "ast.h"

#include <fstream>
#include <string>

#include <iostream>

using namespace std;
using namespace Eight;

Parse *Eight::parseFile(const string file) {
    ifstream *stream = new ifstream(file);
    Parse *result = parse(file, stream);
    stream->close();
    
    return result;
}

Parse *Eight::parse(const string filename, istream *in) {
    return new Parse(filename, in);
}

Parse::Parse(const std::string filename, istream *in) : m_name(filename) {
    m_lexer = new Lexer(in);
}

Parse::~Parse() {
    delete m_lexer;
}

std::vector<AST*> Parse::root() {
    std::vector<AST*> nodes;

    AST *node;
    while ((node = declaration()) != nullptr) {
        nodes.push_back(std::move(node));
    }

    if (!m_lexer->eof()) {
        throw syntax_error(m_lexer->position().line(), m_lexer->position().column(), "Unexpected token");
    }

    return nodes;
}

Type *Parse::type() {
    m_lexer->push();

    Token *identifier = m_lexer->identifier();
    if (!identifier) {
        m_lexer->popSeek();
        return nullptr;
    }

    Type *t = new Type(identifier->text());
    delete identifier;

    m_lexer->pop();

    return t;
}

Parameter *Parse::parameter() {
    m_lexer->push();

    Token *identifier = m_lexer->identifier();
    if (!identifier) {
        m_lexer->popSeek();
        return nullptr;
    }

    if (!m_lexer->colon()) {
        delete identifier;
        throw syntax_error(m_lexer->position().line(), m_lexer->position().column(), "Unexpected token");
    }

    Type *t = type();
    if (!t) {
        delete identifier;
        throw syntax_error(m_lexer->position().line(), m_lexer->position().column(), "Unexpected token");
    }

    Parameter *param = new Parameter(identifier->text(), t);
    delete identifier;

    m_lexer->pop();

    return param;
}

Prototype *Parse::prototype() {
    m_lexer->push();

    if (!m_lexer->func()) {
        m_lexer->popSeek();
        return nullptr;
    }

    Token *identifier = m_lexer->identifier();
    if (!identifier) {
        throw syntax_error(m_lexer->position().line(), m_lexer->position().column(), "Unexpected token");
    }

    if (!m_lexer->lparen()) {
        delete identifier;
        throw syntax_error(m_lexer->position().line(), m_lexer->position().column(), "Unexpected token");
    }

    bool variadic = false;
    std::vector<Parameter> parameters;

    Parameter *param;
    if ((param = parameter()) != nullptr) {
        parameters.push_back(std::move(*param));
        delete param;

        if (m_lexer->elipses()) {
            variadic = true;
        }
    }

    if (!m_lexer->rparen()) {
        delete identifier;
        throw syntax_error(m_lexer->position().line(), m_lexer->position().column(), "Unexpected token");
    }

    Prototype *pt = new Prototype(identifier->text(), variadic);
    delete identifier;

    pt->parameters().swap(parameters);

    m_lexer->pop();

    return pt;
}

CodeBlock *Parse::codeBlock() {
    m_lexer->push();

    if (!m_lexer->lbrace()) {
        m_lexer->popSeek();
        return nullptr;
    }

    std::vector<Statement*> states = statements();

    if (!m_lexer->rbrace()) {
        for (const auto &st : states)
            delete st;
        
        throw syntax_error(m_lexer->position().line(), m_lexer->position().column(), "Unexpected token when finishing code block");
    }

    CodeBlock *cb = new CodeBlock();
    cb->statements().swap(states);

    m_lexer->pop();

    return cb;
}

Statement *Parse::statement() {
    Statement *st;
    if ((st = controlTransfer()) != nullptr) {
        m_lexer->push();

        if (!m_lexer->semicolon())
            m_lexer->popSeek();
        else
            m_lexer->pop();
        
        return st;
    }

    if ((st = declarationStatement()) != nullptr) {
        m_lexer->push();

        if (!m_lexer->semicolon())
            m_lexer->popSeek();
        else
            m_lexer->pop();
        
        return st;
    }
    
    if ((st = expressionStatement()) != nullptr) {
        m_lexer->push();

        if (!m_lexer->semicolon())
            m_lexer->popSeek();
        else
            m_lexer->pop();
        
        return st;
    }

    return nullptr;
}

std::vector<Statement*> Parse::statements() {
    std::vector<Statement*> statements;

    Statement *st;
    while ((st = statement()) != nullptr) {
        statements.push_back(std::move(st));
    }

    return statements;
}

ExpressionStatement *Parse::expressionStatement() {
    Expression *e = expression();
    if (!e) {
        return nullptr;
    }

    return new ExpressionStatement(e);
}

DeclarationStatement *Parse::declarationStatement() {
    Declaration *d = declaration();
    if (!d) {
        return nullptr;
    }

    return new DeclarationStatement(d);
}

ControlTransfer *Parse::controlTransfer() {
    ControlTransfer *ct;
    if ((ct = ret()) != nullptr) {
        return ct;
    }

    return nullptr;
}

Return *Parse::ret() {
    m_lexer->push();

    if (!m_lexer->ret()) {
        m_lexer->popSeek();
        return nullptr;
    }

    Return *r = new Return(expression());

    m_lexer->pop();

    return r;
}

Expression *Parse::expression() {
    PrefixExpression *pe = prefixExpression();
    if (!pe) {
        return nullptr;
    }

    std::vector<BinaryExpression*> es;
    BinaryExpression *binary;
    while ((binary = binaryExpression()) != nullptr) {
        es.push_back(std::move(binary));
    }

    Expression *e = new Expression(pe);
    e->binaryExpressions().swap(es);

    return e;
}

BinaryExpression *Parse::binaryExpression() {
    BinaryExpression *be;
    if ((be = assignment()) != nullptr) {
        return be;
    }

    return nullptr;
}

Assignment *Parse::assignment() {
    m_lexer->push();

    if (!m_lexer->equals()) {
        m_lexer->popSeek();
        return nullptr;
    }

    PrefixExpression *pe = prefixExpression();
    if (!pe) {
        throw syntax_error(m_lexer->position().line(), m_lexer->position().column(), "Unexpected token");
    }

    m_lexer->pop();

    return new Assignment(pe);
}

PrefixExpression *Parse::prefixExpression() {
    PostfixExpression *pe = postfixExpression();
    if (!pe) {
        return nullptr;
    }

    return new PrefixExpression(pe);
}

PrimaryExpression *Parse::primaryExpression() {
    PrimaryExpression *pe;
    if ((pe = literal()) != nullptr) {
        return pe;
    }
    
    if ((pe = identifier()) != nullptr) {
        return pe;
    }

    return nullptr;
}

Literal *Parse::literal() {
    String *str = string();
    if (str) {
        return str;
    }

    return nullptr;
}

String *Parse::string() {
    m_lexer->push();

    Token *str = m_lexer->string();
    if (!str) {
        m_lexer->popSeek();
        return nullptr;
    }

    String *s = new String(str->text());
    delete str;

    m_lexer->pop();

    return s;
}

Identifier *Parse::identifier() {
    m_lexer->push();

    Token *id = m_lexer->identifier();
    if (!id) {
        m_lexer->popSeek();
        return nullptr;
    }

    Identifier *i = new Identifier(id->text());
    delete id;

    m_lexer->pop();

    return i;
}

PostfixExpression *Parse::postfixExpression() {
    PostfixExpression *output = primaryPostfixExpression();
    if (!output) {
        return nullptr;
    }

    PostfixExpression *prime;
    if ((prime = postfixExpressionPrime()) != nullptr) {
        prime->setHead(output);
        output = prime;
    }

    return output;
}

PostfixExpression *Parse::postfixExpressionPrime() {
    PostfixExpression *output = call();

    if (!output) {
        output = explicitMember();
    }

    if (!output) {
        return nullptr;
    }

    PostfixExpression *prime;
    if ((prime = postfixExpressionPrime()) != nullptr) {
        prime->setHead(output);
        output = prime;
    }

    return output;
}

PrimaryPostfixExpression *Parse::primaryPostfixExpression() {
    PrimaryExpression *pe = primaryExpression();
    if (!pe) {
        return nullptr;
    }

    return new PrimaryPostfixExpression(pe);
}

Call *Parse::call() {
    m_lexer->push();

    if (!m_lexer->lparen()) {
        m_lexer->popSeek();
        return nullptr;
    }

    Expression *arg;
    std::vector<Expression*> args;
    if ((arg = expression()) != nullptr) {
        args.push_back(arg);
    }

    if (!m_lexer->rparen()) {
        for (const auto &arg : args)
            delete arg;
        
        throw syntax_error(m_lexer->position().line(), m_lexer->position().column(), "Unexpected token inside of call parenthesis");
    }

    Call *c = new Call();
    c->arguments().swap(args);

    m_lexer->pop();

    return c;
}

ExplicitMember *Parse::explicitMember() {
    m_lexer->push();

    if (!m_lexer->dot()) {
        m_lexer->popSeek();
        return nullptr;
    }

    Token *identifier = m_lexer->identifier();
    if (!identifier) {
        m_lexer->popSeek();
        return nullptr;
    }

    ExplicitMember *em = new ExplicitMember(identifier->text());
    delete identifier;

    m_lexer->pop();

    return em;
}

Declaration *Parse::declaration() {
    Declaration *decl;
    if ((decl = variable()) != nullptr) {
        return decl;
    }
    
    if ((decl = function()) != nullptr) {
        return decl;
    }
    
    if ((decl = structure()) != nullptr) {
        return decl;
    }

    return nullptr;
}

std::vector<Declaration*> Parse::declarations() {
    std::vector<Declaration*> decls;

    Declaration *d;
    while ((d = declaration()) != nullptr) {
        decls.push_back(std::move(d));
    }

    return decls;
}

Variable *Parse::variable() {
    m_lexer->push();

    bool mutate;
    if (m_lexer->var()) {
        mutate = true;
    } else if (m_lexer->let()) {
        mutate = false;
    } else {
        m_lexer->popSeek();
        return nullptr;
    }

    Token *identifier = m_lexer->identifier();
    if (!identifier) {
        throw syntax_error(m_lexer->position().line(), m_lexer->position().column(), "Unexpected token");
    }

    if (!m_lexer->colon()) {
        delete identifier;
        throw syntax_error(m_lexer->position().line(), m_lexer->position().column(), "Unexpected token");
    }

    Type *t = type();
    if (!t) {
        delete identifier;
        throw syntax_error(m_lexer->position().line(), m_lexer->position().column(), "Unexpected token");
    }

    Expression *e = nullptr;
    if (m_lexer->equals()) {
        e = expression();

        if (!e) {
            delete identifier;
            delete t;
            throw syntax_error(m_lexer->position().line(), m_lexer->position().column(), "Unexpected token");
        }
    }

    Variable *v = new Variable(identifier->text(), t, e, mutate);
    delete identifier;

    m_lexer->pop();

    return v;
}

Function *Parse::function() {
    Prototype *pt = prototype();
    if (!pt) {
        return nullptr;
    }

    Function *f = new Function(pt, codeBlock());

    return f;
}

Struct *Parse::structure() {
    m_lexer->push();

    if (!m_lexer->structure()) {
        m_lexer->popSeek();
        return nullptr;
    }

    Token *identifier = m_lexer->identifier();
    if (!identifier) {
        throw syntax_error(m_lexer->position().line(), m_lexer->position().column(), "Unexpected token");
    }

    if (!m_lexer->lbrace()) {
        delete identifier;
        throw syntax_error(m_lexer->position().line(), m_lexer->position().column(), "Unexpected token");
    }

    std::vector<Declaration*> body = declarations();

    if (!m_lexer->rbrace()) {
        delete identifier;
        for (const auto &member : body)
            delete member;

        throw syntax_error(m_lexer->position().line(), m_lexer->position().column(), "Unexpected token");
    }

    Struct *s = new Struct(identifier->text());
    delete identifier;
    s->body().swap(body);

    m_lexer->pop();

    return s;
}
