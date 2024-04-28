project(
  'libtrx',
  'c',
  default_options: [
    'c_std=c11',
    'warning_level=2',
  ],
)

c_compiler = meson.get_compiler('c')
build_opts = [
  '-Wno-unused',
  '-DMESON_BUILD',
]
add_project_arguments(build_opts, language: 'c')

staticdeps = get_option('staticdeps')

# Always dynamically link on macOS
if host_machine.system() == 'darwin'
  staticdeps = false
endif

dep_sdl2 = dependency('SDL2', static: staticdeps)
dep_backtrace = c_compiler.find_library('backtrace', static: true, required: false)

sources = [
  'src/filesystem.c',
  'src/log.c',
  'src/memory.c',
  'src/strings.c',
]

dependencies = [
  dep_sdl2,
  dep_backtrace,
]

if dep_backtrace.found() and host_machine.system() == 'linux'
  sources += ['src/log_linux.c']
elif host_machine.system() == 'windows'
  sources += ['src/log_windows.c']
  dep_dbghelp = c_compiler.find_library('dbghelp')
  dependencies += [dep_dbghelp]
else
  sources += ['src/log_unknown.c']
endif

libtrx = static_library(
  'libtrx',
  sources,
  dependencies: dependencies,
  include_directories: ['src/'],
)
