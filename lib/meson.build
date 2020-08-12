# # Parser library
# parser_src = files(
#     'util/lexer.cc',

#     'lexer.cc',
#     'parser.cc',

#     'parser/token.cc',

#     'ast/type.cc',
#     'ast/codeBlock.cc',
#     'ast/parameter.cc',
#     'ast/prototype.cc',
#     'ast/expression.cc',
#     'ast/expression/postfix.cc',
#     'ast/expression/postfix/call.cc',
#     'ast/expression/postfix/primary.cc',
#     'ast/expression/primary/identifier.cc',
#     'ast/expression/primary/literal/string.cc',

#     'ast/statement/expression.cc',
#     'ast/statement/declaration.cc',
#     'ast/statement/controlTransfer/return.cc',

#     'ast/declaration/variable.cc',
#     'ast/declaration/function.cc',
#     'ast/declaration/struct.cc',
    
#     'codegen/visitor.cc'
# )

# parser_lib = library('parser', parser_src, include_directories: [eight_inc], dependencies: [llvm_dep], cpp_args: ['-DEIGHT_COMPILATION'], install: true)
# parser_dep = declare_dependency(include_directories: [eight_inc], link_with: [parser_lib], version: meson.project_version())

# Eight library
eight_src = files(
    'util/lexer.cc',

    'lexer.cc',
    'parser.cc',
    'sema.cc',

    'ast.cc',

    'parser/token.cc',

    'ast/type.cc',
    'ast/codeBlock.cc',
    'ast/parameter.cc',
    'ast/prototype.cc',

    'ast/expression.cc',

    'ast/expression/binary/assignment.cc',

    'ast/expression/prefix.cc',

    'ast/expression/postfix.cc',
    'ast/expression/postfix/call.cc',
    'ast/expression/postfix/explicitMember.cc',
    'ast/expression/postfix/primary.cc',

    'ast/expression/primary/identifier.cc',
    'ast/expression/primary/literal/string.cc',

    'ast/statement/expression.cc',
    'ast/statement/declaration.cc',
    'ast/statement/controlTransfer/return.cc',

    'ast/declaration.cc',
    'ast/declaration/variable.cc',
    'ast/declaration/function.cc',
    'ast/declaration/struct.cc',
    
    'codegen/visitor.cc'
)

eight_lib = library('eight', eight_src, include_directories: [eight_inc], dependencies: [llvm_dep], c_args: ['-DEIGHT_COMPILATION'], install: true)
eight_dep = declare_dependency(include_directories: [eight_inc], link_with: [eight_lib], version: meson.project_version())
