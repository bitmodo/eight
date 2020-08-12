#include "eight/ast/expression/postfix/explicitMember.h"

#include "eight/sema.h"
#include "eight/codegen/visitor.h"

#include <memory>

using namespace std;
using namespace Eight;

ExplicitMember::ExplicitMember(const std::string &member) : m_member(member) {}

ExplicitMember::~ExplicitMember() {
    if (m_parent)
        delete m_parent;
}

ExplicitMember::ExplicitMember(ExplicitMember &&other) : m_parent(std::move(other.m_parent)), m_member(other.m_member) {
    other.m_parent = nullptr;
}

ExplicitMember &ExplicitMember::operator=(ExplicitMember &&other) {
    m_member = other.m_member;
    m_parent = std::move(other.m_parent);
    other.m_parent = nullptr;

    return *this;
}

void ExplicitMember::analyze(Sema *sema) {
    sema->analyze(this);
}

llvmValue ExplicitMember::accept(Visitor *visitor) const {
    return visitor->visit(this);
}

void ExplicitMember::setHead(PostfixExpression *head) {
    m_parent = std::move(head);
}
