#ifndef EIGHT_PARSER_H
#define EIGHT_PARSER_H

#include "api.h"

#include "_ast_classes.h"

#include <istream>
#include <string>
#include <vector>

namespace Eight {
    class Token;
    class Lexer;

    struct Parse {
        const std::string m_name;
        Lexer *m_lexer;

        PostfixExpression *postfixExpressionPrime();
    public:
        Parse(const std::string, std::istream*);
        ~Parse();

        inline const std::string &name() const {
            return m_name;
        }

        std::vector<AST*> root();

        Type *type();

        Parameter *parameter();
        Prototype *prototype();
        CodeBlock *codeBlock();

        Statement *statement();
        std::vector<Statement*> statements();
        ExpressionStatement *expressionStatement();
        DeclarationStatement *declarationStatement();

        ControlTransfer *controlTransfer();
        Return *ret();

        Expression *expression();

        BinaryExpression *binaryExpression();
        Assignment *assignment();

        PrefixExpression *prefixExpression();

        PrimaryExpression *primaryExpression();

        Literal *literal();
        String *string();

        Identifier *identifier();

        PostfixExpression *postfixExpression();
        PrimaryPostfixExpression *primaryPostfixExpression();
        Call *call();
        ExplicitMember *explicitMember();

        Declaration *declaration();
        std::vector<Declaration*> declarations();
        Variable *variable();
        Function *function();
        Struct *structure();
    };

    Parse *parse(const std::string, std::istream*);
    Parse *parseFile(const std::string);
}

#endif
