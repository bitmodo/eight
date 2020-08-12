#include "eight/ast/statement/expression.h"

#include "eight/ast/expression.h"

#include "eight/sema.h"
#include "eight/codegen/visitor.h"

#include <memory>

using namespace std;
using namespace Eight;

ExpressionStatement::ExpressionStatement(Expression *expression) : m_expression(std::move(expression)) {}

ExpressionStatement::~ExpressionStatement() {
    if (m_expression)
        delete m_expression;
}

ExpressionStatement::ExpressionStatement(ExpressionStatement &&other) : m_expression(std::move(other.m_expression)) {
    other.m_expression = nullptr;
}

ExpressionStatement &ExpressionStatement::operator=(ExpressionStatement &&other) {
    m_expression = std::move(other.m_expression);
    other.m_expression = nullptr;

    return *this;
}

void ExpressionStatement::analyze(Sema *sema) {
    sema->analyze(this);
}

llvmValue ExpressionStatement::accept(Visitor *visitor) const {
    return visitor->visit(this);
}
