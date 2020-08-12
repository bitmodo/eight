#include "eight/ast/codeBlock.h"

#include "eight/ast/statement.h"

#include "eight/sema.h"
#include "eight/codegen/visitor.h"

#include <memory>

using namespace std;
using namespace Eight;

CodeBlock::~CodeBlock() {
    for (const auto &statement : m_statements)
        if (statement)
            delete statement;
}

CodeBlock::CodeBlock(CodeBlock &&other) {
    for (const auto &statement : other.m_statements)
        m_statements.push_back(std::move(statement));
    
    other.m_statements.clear();
}

CodeBlock &CodeBlock::operator=(CodeBlock &&other) {
    for (const auto &statement : other.m_statements)
        m_statements.push_back(std::move(statement));
    
    other.m_statements.clear();

    return *this;
}

void CodeBlock::analyze(Sema *sema) {
    sema->analyze(this);
}

llvmValue CodeBlock::accept(Visitor *visitor) const {
    return visitor->visit(this);
}
