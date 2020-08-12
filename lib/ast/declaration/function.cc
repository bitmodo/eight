#include "eight/ast/declaration/function.h"

#include "eight/ast/prototype.h"
#include "eight/ast/codeBlock.h"

#include "eight/sema.h"
#include "eight/codegen/visitor.h"

#include <memory>

using namespace std;
using namespace Eight;

Function::Function(Prototype *prototype, CodeBlock *codeBlock) : m_prototype(std::move(prototype)), m_codeBlock(std::move(codeBlock)) {}

Function::~Function() {
    if (m_prototype)
        delete m_prototype;
    if (m_codeBlock)
        delete m_codeBlock;
}

Function::Function(Function &&other) : m_prototype(std::move(other.m_prototype)), m_codeBlock(std::move(other.m_codeBlock)) {
    other.m_prototype = nullptr;
    other.m_codeBlock = nullptr;
}

Function &Function::operator=(Function &&other) {
    m_prototype = std::move(other.m_prototype);
    m_codeBlock = std::move(other.m_codeBlock);

    other.m_prototype = nullptr;
    other.m_codeBlock = nullptr;

    return *this;
}

void Function::analyze(Sema *sema) {
    sema->analyze(this);
}

llvmValue Function::accept(Visitor *visitor) const {
    return visitor->visit(this);
}
