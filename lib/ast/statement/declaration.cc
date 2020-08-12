#include "eight/ast/statement/declaration.h"

#include "eight/codegen/visitor.h"

#include "eight/sema.h"
#include "eight/ast/declaration.h"

#include <memory>

using namespace std;
using namespace Eight;

DeclarationStatement::DeclarationStatement(Declaration *declaration) : m_declaration(std::move(declaration)) {}

DeclarationStatement::~DeclarationStatement() {
    if (m_declaration)
        delete m_declaration;
}

DeclarationStatement::DeclarationStatement(DeclarationStatement &&other) : m_declaration(std::move(other.m_declaration)) {
    other.m_declaration = nullptr;
}

DeclarationStatement &DeclarationStatement::operator=(DeclarationStatement &&other) {
    m_declaration = std::move(other.m_declaration);
    other.m_declaration = nullptr;

    return *this;
}

void DeclarationStatement::analyze(Sema *sema) {
    sema->analyze(this);
}

llvmValue DeclarationStatement::accept(Visitor *visitor) const {
    return visitor->visit(this);
}
