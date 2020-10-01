# Flex & Bison stuff
flex = find_program('flex', required: true, version: '>= 2.6.4')
bison = find_program('bison', required: true, version: '>= 3.5.1')

parser_raw = files('parser.y')
parser_out_h = meson.current_build_dir() / 'parser.h'
parser_out_c = meson.current_build_dir() / 'parser.c'
parser_src = custom_target('parser',
    command: [bison, '--defines=' + parser_out_h, '--output=' + parser_out_c, parser_raw],
    input: [parser_raw],
    output: ['parser.h', 'parser.c']
)

lexer_raw = files('lexer.l')
lexer_out_h = meson.current_build_dir() / 'lexer.h'
lexer_out_c = meson.current_build_dir() / 'lexer.c'
lexer_src = custom_target('lexer',
    command: [flex, '--header-file=' + lexer_out_h, '--outfile=' + lexer_out_c, lexer_raw],
    input: [lexer_raw, parser_src[0]],
    output: ['lexer.h', 'lexer.c']
)

# Eight library
eight_src = files(
    'eight.c',
    'codegen.c',

    'ast/node.c',
    'ast/expression.c',
    'ast/statement.c',
    'ast/structure.c',
)

eight_lib = library('eight', [eight_src, lexer_src, parser_src], include_directories: [eight_inc], dependencies: [llvm_dep, thread_dep], c_args: ['-DEIGHT_COMPILATION'], install: true)
eight_dep = declare_dependency(include_directories: [eight_inc], link_with: [eight_lib], version: meson.project_version())
