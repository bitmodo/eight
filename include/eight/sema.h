#ifndef EIGHT_SEMA_H
#define EIGHT_SEMA_H

#include "eight/api.h"

#include "_ast_classes.h"

#ifdef ANALYZE_FUNC
#  undef ANALYZE_FUNC
#endif
#define ANALYZE_FUNC(type) void analyze(type*)

namespace Eight {
    class AST;

    struct Sema {
    public:
        ANALYZE_FUNC(AST);
        
        ANALYZE_FUNC(Type);

        ANALYZE_FUNC(Parameter);
        ANALYZE_FUNC(Prototype);
        ANALYZE_FUNC(CodeBlock);

        ANALYZE_FUNC(Statement);
        ANALYZE_FUNC(ExpressionStatement);
        ANALYZE_FUNC(DeclarationStatement);
        
        ANALYZE_FUNC(ControlTransfer);
        ANALYZE_FUNC(Return);

        ANALYZE_FUNC(Expression);
        
        ANALYZE_FUNC(BinaryExpression);
        ANALYZE_FUNC(Assignment);
        
        ANALYZE_FUNC(PrefixExpression);

        ANALYZE_FUNC(PrimaryExpression);

        ANALYZE_FUNC(Literal);
        ANALYZE_FUNC(String);

        ANALYZE_FUNC(Identifier);

        ANALYZE_FUNC(PostfixExpression);
        ANALYZE_FUNC(PrimaryPostfixExpression);
        ANALYZE_FUNC(Call);
        ANALYZE_FUNC(ExplicitMember);
        
        ANALYZE_FUNC(Declaration);
        ANALYZE_FUNC(Variable);
        ANALYZE_FUNC(Function);
        ANALYZE_FUNC(Struct);
    };
}

#endif
