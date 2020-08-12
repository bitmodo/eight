#include "eight/ast/parameter.h"

#include "eight/ast/type.h"

#include "eight/sema.h"
#include "eight/codegen/visitor.h"

#include <memory>

using namespace std;
using namespace Eight;

Parameter::Parameter(const std::string &name, Type *type) : m_name(name), m_type(std::move(type)) {}

Parameter::Parameter(Parameter &&other) : m_name(other.m_name), m_type(std::move(other.m_type)) {
    other.m_type = nullptr;
}

Parameter &Parameter::operator=(Parameter &&other) {
    m_name = other.m_name;
    m_type = std::move(other.m_type);

    other.m_type = nullptr;

    return *this;
}

Parameter::~Parameter() {
    if (m_type)
        delete m_type;
}

void Parameter::analyze(Sema *sema) {
    sema->analyze(this);
}

llvmValue Parameter::accept(Visitor *visitor) const {
    return visitor->visit(this);
}
