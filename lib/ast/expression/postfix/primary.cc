#include "eight/ast/expression/postfix/primary.h"

#include "eight/ast/expression/primary.h"

#include "eight/sema.h"
#include "eight/codegen/visitor.h"

#include <memory>

using namespace std;
using namespace Eight;

PrimaryPostfixExpression::PrimaryPostfixExpression(PrimaryExpression *expression) : m_expression(std::move(expression)) {}

PrimaryPostfixExpression::~PrimaryPostfixExpression() {
    if (m_expression)
        delete m_expression;
}

PrimaryPostfixExpression::PrimaryPostfixExpression(PrimaryPostfixExpression &&other) : m_expression(std::move(other.m_expression)) {
    other.m_expression = nullptr;
}

PrimaryPostfixExpression &PrimaryPostfixExpression::operator=(PrimaryPostfixExpression &&other) {
    m_expression = std::move(other.m_expression);
    other.m_expression = nullptr;

    return *this;
}

void PrimaryPostfixExpression::analyze(Sema *sema) {
    sema->analyze(this);
}

llvmValue PrimaryPostfixExpression::accept(Visitor *visitor) const {
    return visitor->visit(this);
}
