#include "eight/ast/declaration/variable.h"

#include "eight/sema.h"
#include "eight/codegen/visitor.h"

#include "eight/ast/type.h"
#include "eight/ast/expression.h"

#include <memory>

using namespace std;
using namespace Eight;

Variable::Variable(std::string name, Type *type, Expression *expression, bool mutate) : Declaration(name), m_mutable(mutate), m_type(std::move(type)), m_expression(std::move(expression)) {}

Variable::~Variable() {
    if (m_type)
        delete m_type;
    
    if (m_expression)
        delete m_expression;
}

Variable::Variable(Variable &&other) : Declaration(other.m_name), m_mutable(other.m_mutable), m_type(std::move(other.m_type)), m_expression(std::move(other.m_expression)) {
    other.m_type = nullptr;
    other.m_expression = nullptr;
}

Variable &Variable::operator=(Variable &&other) {
    m_mutable = other.m_mutable;
    m_name = other.m_name;
    m_type = std::move(other.m_type);
    m_expression = std::move(other.m_expression);

    other.m_type = nullptr;
    other.m_expression = nullptr;

    return *this;
}

void Variable::analyze(Sema *sema) {
    sema->analyze(this);
}

llvmValue Variable::accept(Visitor *visitor) const {
    return visitor->visit(this);
}
