#include "eight/ast/expression/binary/assignment.h"

#include "eight/ast/expression/prefix.h"

#include "eight/sema.h"
#include "eight/codegen/visitor.h"

#include <memory>

using namespace std;
using namespace Eight;

Assignment::Assignment(PrefixExpression *expression) : m_expression(std::move(expression)) {}

Assignment::~Assignment() {
    if (m_expression)
        delete m_expression;
}

Assignment::Assignment(Assignment &&other) : m_expression(std::move(other.m_expression)) {
    other.m_expression = nullptr;
}

Assignment &Assignment::operator=(Assignment &&other) {
    return *this;
}

void Assignment::analyze(Sema *sema) {
    sema->analyze(this);
}

llvmValue Assignment::accept(Visitor *visitor) const {
    return visitor->visit(this);
}
