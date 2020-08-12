#include "eight/ast/expression/primary/literal/string.h"

#include "eight/sema.h"
#include "eight/codegen/visitor.h"

using namespace std;
using namespace Eight;

String::String(const std::string &content) : m_content(content) {}

void String::analyze(Sema *sema) {
    sema->analyze(this);
}

llvmValue String::accept(Visitor *visitor) const {
    return visitor->visit(this);
}
