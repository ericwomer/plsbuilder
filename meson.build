project(
    'MPV Playlist Builder',
    'cpp',
    version: '0.0.0', # This is overridden in the conf_data section
    default_options: ['cpp_std=c++20', 'warning_level=1'],
    meson_version: '>=1.2.1',
)

executable_name = 'plsbuilder'
pkgconfig = import('pkgconfig')
cpp = meson.get_compiler('cpp')
cpp_family = cpp.get_id()

conf_data = configuration_data()
conf_data.set('name', meson.project_name())

conf_data.set('compiler_name', cpp.get_id())
conf_data.set('compiler_version', cpp.version())
conf_data.set('host_machine', host_machine.system())
conf_data.set('cpu_family', build_machine.cpu_family())

conf_data.set('major', '0')
conf_data.set('minor', '0')
conf_data.set('patch', '1')
conf_data.set('compile', '0')

configure_file(
    input: 'src/config.h.in',
    output: 'config.h',
    configuration: conf_data,
)

prefix = get_option('prefix')
bindir = prefix / get_option('bindir')
localedir = prefix / get_option('localedir')

datadir = prefix / get_option('datadir')
pkgdatadir = datadir / meson.project_name()
iconsdir = datadir / 'icons'
podir = meson.project_source_root() / 'po'
gettext_package = meson.project_name()

skeleton_src = [
    'src/main.cpp',
    'src/builder/builder.cpp',
    'src/utility/utility.cpp'
]

skeleton_include_directories = [
    include_directories('src/'),
    include_directories('build/'),
]

skeleton_cxxflags = [ '-Wall', '-Wextra', '-Werror', '-O3']
skeleton_ldflags = []
math = cpp.find_library('m', required: false)

skeleton_deps = [math]

if get_option('use-mold') and find_program('mold', required : false).found()
    message('Linking with mold')
    skeleton_ldflags += ['-fuse-ld=mold']
else
    message('Using the default selected linker')
endif

if cpp_family == 'gcc'
    skeleton_cxxflags += ['-fdiagnostics-color=always']
    skeleton_include_directories += [include_directories('/usr/include/')]
elif cpp_family == 'clang'
    skeleton_cxxflags += ['-fcolor-diagnostics', '-stdlib=libc++', '-Wnon-gcc']

    skeleton_include_directories += [
        include_directories('/usr/include/c++/v1/'),
    ]

    skeleton_ldflags += [
        '-lc++',
        '-lc++abi',
    ]

else
    # Do something else for MSVC?
endif

skeleton = executable(
    executable_name,
    skeleton_src,
    install: true,
    dependencies: skeleton_deps,
    include_directories: skeleton_include_directories,
    cpp_args: skeleton_cxxflags,
    link_args: skeleton_ldflags,
)

install_data('LICENSE', install_dir: join_paths('share/doc', executable_name))
