#include "eight/sema.h"

#include "ast.h"

using namespace std;
using namespace Eight;

#ifdef ANALYZE_FUNC_IMPL
#  undef ANALYZE_FUNC_IMPL
#endif
#define ANALYZE_FUNC_IMPL(type) void Sema::analyze(type *node)

ANALYZE_FUNC_IMPL(AST) {
    node->analyze(this);
}
        
ANALYZE_FUNC_IMPL(Type) {

}

ANALYZE_FUNC_IMPL(Parameter) {

}

ANALYZE_FUNC_IMPL(Prototype) {

}

ANALYZE_FUNC_IMPL(CodeBlock) {

}

ANALYZE_FUNC_IMPL(Statement) {

}

ANALYZE_FUNC_IMPL(ExpressionStatement) {

}

ANALYZE_FUNC_IMPL(DeclarationStatement) {

}

ANALYZE_FUNC_IMPL(ControlTransfer) {

}

ANALYZE_FUNC_IMPL(Return) {

}

ANALYZE_FUNC_IMPL(Expression) {
    analyze(node->expression());
}

ANALYZE_FUNC_IMPL(BinaryExpression) {

}

ANALYZE_FUNC_IMPL(Assignment) {

}

ANALYZE_FUNC_IMPL(PrefixExpression) {

}

ANALYZE_FUNC_IMPL(PrimaryExpression) {

}

ANALYZE_FUNC_IMPL(Literal) {

}

ANALYZE_FUNC_IMPL(String) {

}

ANALYZE_FUNC_IMPL(Identifier) {

}

ANALYZE_FUNC_IMPL(PostfixExpression) {

}

ANALYZE_FUNC_IMPL(PrimaryPostfixExpression) {

}

ANALYZE_FUNC_IMPL(Call) {

}

ANALYZE_FUNC_IMPL(ExplicitMember) {

}

ANALYZE_FUNC_IMPL(Declaration) {

}

ANALYZE_FUNC_IMPL(Variable) {
    node->expression()->name() = node->name();
}

ANALYZE_FUNC_IMPL(Function) {

}

ANALYZE_FUNC_IMPL(Struct) {

}
