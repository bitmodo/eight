#include "eight/ast/expression/postfix/call.h"

#include "eight/ast/expression.h"

#include "eight/sema.h"
#include "eight/codegen/visitor.h"

using namespace std;
using namespace Eight;

// Call::Call() {}

Call::~Call() {
    if (m_func)
        delete m_func;

    for (const auto &arg : m_arguments)
        if (arg)
            delete arg;
}

Call::Call(Call &&other) : m_func(std::move(other.m_func)) {
    other.m_name = nullptr;

    for (const auto &arg : other.m_arguments)
        m_arguments.push_back(std::move(arg));
    
    other.m_arguments.clear();
}

Call &Call::operator=(Call &&other) {
    m_name = std::move(other.m_name);
    other.m_name = nullptr;

    for (const auto &arg : other.m_arguments)
        m_arguments.push_back(std::move(arg));
    
    other.m_arguments.clear();

    return *this;
}

void Call::analyze(Sema *sema) {
    sema->analyze(this);
}

llvmValue Call::accept(Visitor *visitor) const {
    return visitor->visit(this);
}

void Call::setHead(PostfixExpression *head) {
    m_func = std::move(head);
}
