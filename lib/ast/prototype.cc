#include "eight/ast/prototype.h"

#include "eight/sema.h"
#include "eight/codegen/visitor.h"

using namespace std;
using namespace Eight;

Prototype::Prototype(const std::string &name, bool variadic) : m_name(name), m_variadic(variadic) {}

void Prototype::analyze(Sema *sema) {
    sema->analyze(this);
}

llvmValue Prototype::accept(Visitor *visitor) const {
    return visitor->visit(this);
}
