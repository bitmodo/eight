#include "eight/ast/type.h"

#include "eight/sema.h"
#include "eight/codegen/visitor.h"

using namespace std;
using namespace Eight;

Type::Type(const std::string &name) : m_name(name) {}

void Type::analyze(Sema *sema) {
    sema->analyze(this);
}

llvmValue Type::accept(Visitor *visitor) const {
    return visitor->visit(this);
}
