#include "eight/ast/statement/controlTransfer/return.h"

#include "eight/ast/expression.h"

#include "eight/sema.h"
#include "eight/codegen/visitor.h"

#include <memory>

using namespace std;
using namespace Eight;

Return::Return(Expression *expression) : m_expression(std::move(expression)) {}

Return::~Return() {
    if (m_expression)
        delete m_expression;
}

Return::Return(Return &&other) : m_expression(std::move(other.m_expression)) {
    other.m_expression = nullptr;
}

Return &Return::operator=(Return &&other) {
    m_expression = std::move(other.m_expression);
    other.m_expression = nullptr;

    return *this;
}

void Return::analyze(Sema *sema) {
    sema->analyze(this);
}

llvmValue Return::accept(Visitor *visitor) const {
    return visitor->visit(this);
}
