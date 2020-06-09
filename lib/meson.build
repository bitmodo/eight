# Parser library
parser_src = files(
    'lexer.cc',
    'parser.cc'
)

parser_lib = library('parser', parser_src, include_directories: [eight_inc], cpp_args: ['-DEIGHT_COMPILATION'], install: true)
parser_dep = declare_dependency(include_directories: [eight_inc], link_with: [parser_lib], version: meson.project_version())

# Eight library
eight_src = files(
)

eight_lib = library('eight', eight_src, include_directories: [eight_inc], dependencies: [parser_dep, llvm_dep], c_args: ['-DEIGHT_COMPILATION'], install: true)
eight_dep = declare_dependency(include_directories: [eight_inc], link_with: [eight_lib], version: meson.project_version())
