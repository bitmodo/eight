#include "eight/ast/expression/primary/identifier.h"

#include "eight/sema.h"
#include "eight/codegen/visitor.h"

using namespace std;
using namespace Eight;

Identifier::Identifier(const std::string &name) : m_name(name) {}

void Identifier::analyze(Sema *sema) {
    sema->analyze(this);
}

llvmValue Identifier::accept(Visitor *visitor) const {
    return visitor->visit(this);
}
