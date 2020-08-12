#include "eight/ast/expression.h"

#include "eight/ast/expression/prefix.h"
#include "eight/ast/expression/binary.h"

#include "eight/sema.h"
#include "eight/codegen/visitor.h"

#include <memory>

using namespace std;
using namespace Eight;

Expression::Expression(PrefixExpression *expression) : m_expression(std::move(expression)) {}

Expression::~Expression() {
    if (m_expression)
        delete m_expression;
    
    for (const auto &exp : m_binaryExpressions)
        if (exp)
            delete exp;
}

Expression::Expression(Expression &&other) : m_expression(std::move(other.m_expression)) {
    other.m_expression = nullptr;

    for (const auto &exp : other.m_binaryExpressions)
        m_binaryExpressions.push_back(std::move(exp));
    
    other.m_binaryExpressions.clear();
}

Expression &Expression::operator=(Expression &&other) {
    m_expression = std::move(other.m_expression);
    other.m_expression = nullptr;

    for (const auto &exp : other.m_binaryExpressions)
        m_binaryExpressions.push_back(std::move(exp));
    
    other.m_binaryExpressions.clear();

    return *this;
}

void Expression::analyze(Sema *sema) {
    sema->analyze(this);
}

llvmValue Expression::accept(Visitor *visitor) const {
    return visitor->visit(this);
}
