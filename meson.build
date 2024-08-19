project(
  'libtrx',
  'c',
  default_options: [
    'c_std=c11',
    'warning_level=2',
  ],
)

fs = import('fs')

c_compiler = meson.get_compiler('c')
relative_dir = fs.relative_to(meson.current_source_dir(), meson.global_build_root())
build_opts = [
  '-fmacro-prefix-map=@0@/src=libtrx'.format(relative_dir),
  '-Wno-unused',
  '-DMESON_BUILD',
  '-DDWST_STATIC',
  '-DPCRE2_STATIC',
  '-DPCRE2_CODE_UNIT_WIDTH=8',
]
add_project_arguments(build_opts, language: 'c')

staticdeps = get_option('staticdeps')

# Always dynamically link on macOS
if host_machine.system() == 'darwin'
  staticdeps = false
endif

null_dep = dependency('', required: false)
dep_avcodec = dependency('libavcodec', static: staticdeps)
dep_avformat = dependency('libavformat', static: staticdeps)
dep_avutil = dependency('libavutil', static: staticdeps)
dep_sdl2 = dependency('SDL2', static: staticdeps)
dep_pcre2 = dependency('libpcre2-8', static: staticdeps)
dep_backtrace = c_compiler.find_library('backtrace', static: true, required: false)
dep_swscale = dependency('libswscale', static: staticdeps)
dep_swresample = dependency('libswresample', static: staticdeps)

dep_zlib = null_dep

if not staticdeps
  dep_zlib = dependency('zlib', static: staticdeps)
endif

sources = [
  'src/benchmark.c',
  'src/filesystem.c',
  'src/engine/audio.c',
  'src/engine/audio_sample.c',
  'src/engine/audio_stream.c',
  'src/json/bson_parse.c',
  'src/json/bson_write.c',
  'src/json/json_base.c',
  'src/json/json_parse.c',
  'src/json/json_write.c',
  'src/log.c',
  'src/memory.c',
  'src/strings.c',
  'src/vector.c',
]

dependencies = [
  dep_avcodec,
  dep_avformat,
  dep_avutil,
  dep_sdl2,
  dep_pcre2,
  dep_backtrace,
  dep_swresample,
  dep_swscale,
  dep_zlib,
]

if dep_backtrace.found() and host_machine.system() == 'linux'
  sources += ['src/log_linux.c']
elif host_machine.system() == 'windows'
  sources += ['src/log_windows.c']
  dwarfstack = subproject('dwarfstack', default_options: ['warning_level=0'])
  dep_dwarfstack = dwarfstack.get_variable('dep_dwarfstack')
  dep_dbghelp = c_compiler.find_library('dbghelp')
  dependencies += [dep_dbghelp, dep_dwarfstack]
else
  sources += ['src/log_unknown.c']
endif

libtrx = static_library(
  'libtrx',
  sources,
  dependencies: dependencies,
  include_directories: [
    'include/libtrx/',
    'src/',
  ]
)

dep_trx = declare_dependency(
  link_whole: libtrx,
  include_directories: [
    include_directories('include', is_system: true)
  ]
)
