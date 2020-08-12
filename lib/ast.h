#include "eight/ast.h"

#include "eight/ast/type.h"

#include "eight/ast/parameter.h"
#include "eight/ast/prototype.h"
#include "eight/ast/codeBlock.h"

#include "eight/ast/statement.h"
#include "eight/ast/statement/expression.h"
#include "eight/ast/statement/declaration.h"

#include "eight/ast/statement/controlTransfer.h"
#include "eight/ast/statement/controlTransfer/return.h"

#include "eight/ast/expression.h"

#include "eight/ast/expression/binary.h"
#include "eight/ast/expression/binary/assignment.h"

#include "eight/ast/expression/prefix.h"

#include "eight/ast/expression/primary.h"

#include "eight/ast/expression/primary/literal.h"
#include "eight/ast/expression/primary/literal/string.h"

#include "eight/ast/expression/primary/identifier.h"

#include "eight/ast/expression/postfix.h"
#include "eight/ast/expression/postfix/primary.h"
#include "eight/ast/expression/postfix/call.h"
#include "eight/ast/expression/postfix/explicitMember.h"

#include "eight/ast/declaration.h"
#include "eight/ast/declaration/variable.h"
#include "eight/ast/declaration/function.h"
#include "eight/ast/declaration/struct.h"
