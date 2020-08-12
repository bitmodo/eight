#include "eight/ast/expression/prefix.h"

#include "eight/ast/expression/postfix.h"

#include "eight/sema.h"
#include "eight/codegen/visitor.h"

#include <memory>

using namespace std;
using namespace Eight;

PrefixExpression::PrefixExpression(PostfixExpression *expression) : m_expression(std::move(expression)) {}

PrefixExpression::~PrefixExpression() {
    if (m_expression)
        delete m_expression;
}

PrefixExpression::PrefixExpression(PrefixExpression &&other) : m_expression(std::move(other.m_expression)) {
    other.m_expression = nullptr;
}

PrefixExpression &PrefixExpression::operator=(PrefixExpression &&other) {
    m_expression = std::move(other.m_expression);
    other.m_expression = nullptr;

    return *this;
}

void PrefixExpression::analyze(Sema *sema) {
    sema->analyze(this);
}

llvmValue PrefixExpression::accept(Visitor *visitor) const {
    return visitor->visit(this);
}
