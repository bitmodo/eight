project('eight', 'c', 'cpp',
    license: 'MIT',
    meson_version: '>= 0.54.1',
    default_options : ['c_std=c11', 'cpp_std=c++17'],
    version: '0.1.0')

# Prepare dependencies
llvm_dep = dependency('LLVM', version: '>= 10.0', modules: ['core'])

# Initialize subdirectories
subdir('include')
subdir('lib')
subdir('tools')
