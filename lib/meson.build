# Eight library
eight_src = files(
    'eight.c',

    'token.c',
    'node.c',

    'lexer.c',
    'parser.c',
    'codegen.c',

    'ast/function.c',
)

eight_lib = library('eight', eight_src, include_directories: [eight_inc], dependencies: [llvm_dep, thread_dep], c_args: ['-DEIGHT_COMPILATION'], install: true)
eight_dep = declare_dependency(include_directories: [eight_inc], link_with: [eight_lib], version: meson.project_version())
