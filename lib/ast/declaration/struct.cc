#include "eight/ast/declaration/struct.h"

#include "eight/sema.h"
#include "eight/codegen/visitor.h"

#include <memory>

using namespace std;
using namespace Eight;

Struct::Struct(const std::string &name) : m_name(name) {}

Struct::~Struct() {
    for (const auto &member : m_body)
        if (member)
            delete member;
}

Struct::Struct(Struct &&other) {
    for (const auto &member : other.m_body)
        m_body.push_back(std::move(member));
    
    other.m_body.clear();
}

Struct &Struct::operator=(Struct &&other) {
    for (const auto &member : other.m_body)
        m_body.push_back(std::move(member));

    other.m_body.clear();

    return *this;
}

void Struct::analyze(Sema *sema) {
    sema->analyze(this);
}

llvmValue Struct::accept(Visitor *visitor) const {
    return visitor->visit(this);
}
