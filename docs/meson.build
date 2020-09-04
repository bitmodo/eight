# Get the commands and make sure the are the correct version
sphinx = find_program('sphinx-build', required: true, version: '>= 3.2.0')
breathe = find_program('breathe-apidoc', required: true, version: '>= 4.20.0')
doxygen = find_program('doxygen', required: true, version: '>= 1.8.0')

# Setup some variables with some common file names and paths
doxygen_folder_name = 'doxygen_doc'
doxygen_database = meson.current_build_dir() / doxygen_folder_name

sphinx_conf_input = 'conf.in.py'
sphinx_conf_output = 'conf.py'
sphinx_conf_input_path = meson.current_source_dir() / sphinx_conf_input
sphinx_conf_output_path = meson.current_build_dir() / sphinx_conf_output

doxy_conf_input = 'doxygen.in.ini'
doxy_conf_output = 'doxygen.ini'
doxy_conf_input_path = meson.current_source_dir() / doxy_conf_input
doxy_conf_output_path = meson.current_build_dir() / doxy_conf_output

# Common dependencies to trigger documentation builds
docs_common_deps = [eight_lib, eightc_exe]

# Configure the Sphinx config file with project settings
sphinx_conf_data = configuration_data()
sphinx_conf_data.set('SRC_ROOT', meson.current_source_dir())
sphinx_conf_data.set('BUILD_ROOT', doxygen_database)
sphinx_conf_data.set('VERSION', meson.project_version())
sphinx_conf = configure_file(
    input: sphinx_conf_input_path,
    output: sphinx_conf_output,
    configuration: sphinx_conf_data
)

# Configure the Doxygen config file with project settings
doxy_conf_data = configuration_data()
doxy_conf_data.set('SRC_ROOT', meson.source_root())
doxy_conf_data.set('OUTPUT_DIR', doxygen_database)
doxygen_conf = configure_file(
		input: doxy_conf_input_path,
		output: doxy_conf_output,
		configuration: doxy_conf_data
)

# Generate the Doxygen XML data for Breathe to use
doxygen_doc = custom_target('doxygen',
    command: [doxygen, doxy_conf_output_path],
    input: [doxygen_conf, docs_common_deps],
    output: doxygen_folder_name,
    build_by_default: true,
)

# Copy the Sphinx files to the build directory
sphinx_index = 'index.rst'
sphinx_files = [sphinx_index]
foreach file : sphinx_files
	configure_file(input: file, output: file, copy: true)
endforeach

# Generate the Sphinx docs
sphinx_doc = custom_target('docs',
    command: [sphinx, '-E', '-W', '-q', '-j', 'auto', meson.current_build_dir(), meson.current_build_dir() / '_build'],
    input: [sphinx_conf, sphinx_index, doxygen_doc, docs_common_deps],
    output: ['_build'],
    build_by_default: true,
    depends: [doxygen_doc],
)
