#ifndef EIGHT_AST_EXPRESSION_POSTFIX_EXPLICITMEMBER_H
#define EIGHT_AST_EXPRESSION_POSTFIX_EXPLICITMEMBER_H

#include "eight/ast/expression/postfix.h"

#include <string>

namespace Eight {
    class ExplicitMember : public PostfixExpression {
        PostfixExpression *m_parent;
        std::string m_member;
    public:
        ExplicitMember(const std::string&);
        ~ExplicitMember() override;

        ExplicitMember(const ExplicitMember&) = delete;
        ExplicitMember &operator=(const ExplicitMember&) = delete;
        ExplicitMember(ExplicitMember&&);
        ExplicitMember &operator=(ExplicitMember&&);

        virtual void analyze(Sema*) override;
        virtual llvmValue accept(Visitor*) const override;

        virtual void setHead(PostfixExpression*) override;

        inline const PostfixExpression *const parent() const {
            return m_parent;
        }

        inline const std::string &member() const {
            return m_member;
        }
    };
}

#endif
