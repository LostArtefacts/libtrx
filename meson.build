project(
  'libtrx',
  'c',
  default_options: [
    'c_std=c11',
    'warning_level=2',
  ],
)

staticdeps = get_option('staticdeps')
tr_version = get_option('tr_version')
gfx_gl_default_backend = get_option('gfx_gl_default_backend')

fs = import('fs')
c_compiler = meson.get_compiler('c')
relative_dir = fs.relative_to(meson.current_source_dir(), meson.global_build_root())
build_opts = [
  '-fmacro-prefix-map=@0@/src=libtrx'.format(relative_dir),
  '-Wno-unused',
  '-Wno-address-of-packed-member',
  '-DMESON_BUILD',
  '-DDWST_STATIC',
  '-DPCRE2_STATIC',
  '-DPCRE2_CODE_UNIT_WIDTH=8',
  '-DTR_VERSION=' + tr_version.to_string(),
  '-DGFX_GL_DEFAULT_BACKEND=' + gfx_gl_default_backend,
]
set_variable('defines', ['-DTR_VERSION=' + tr_version.to_string()])

add_project_arguments(build_opts, language: 'c')

# Always dynamically link on macOS
if host_machine.system() == 'darwin'
  staticdeps = false
endif

uthash = subproject('uthash', default_options: ['warning_level=0'])

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

if host_machine.system() == 'windows'
  dep_opengl = c_compiler.find_library('opengl32')
else
  dep_opengl = dependency('GL')
endif

sources = [
  'src/benchmark.c',
  'src/config/common.c',
  'src/config/file.c',
  'src/engine/audio.c',
  'src/engine/audio_sample.c',
  'src/engine/audio_stream.c',
  'src/engine/image.c',
  'src/enum_str.c',
  'src/event_manager.c',
  'src/filesystem.c',
  'src/game/backpack.c',
  'src/game/console/cmd/config.c',
  'src/game/console/cmd/die.c',
  'src/game/console/cmd/end_level.c',
  'src/game/console/cmd/exit_game.c',
  'src/game/console/cmd/exit_to_title.c',
  'src/game/console/cmd/fly.c',
  'src/game/console/cmd/give_item.c',
  'src/game/console/cmd/heal.c',
  'src/game/console/cmd/kill.c',
  'src/game/console/cmd/play_demo.c',
  'src/game/console/cmd/pos.c',
  'src/game/console/cmd/set_health.c',
  'src/game/console/cmd/sfx.c',
  'src/game/console/cmd/teleport.c',
  'src/game/console/common.c',
  'src/game/game_string.c',
  'src/game/items.c',
  'src/game/objects/names.c',
  'src/game/ui/common.c',
  'src/game/ui/events.c',
  'src/game/ui/widgets/console.c',
  'src/game/ui/widgets/prompt.c',
  'src/game/ui/widgets/spacer.c',
  'src/game/ui/widgets/stack.c',
  'src/gfx/2d/2d_renderer.c',
  'src/gfx/2d/2d_surface.c',
  'src/gfx/3d/3d_renderer.c',
  'src/gfx/3d/vertex_stream.c',
  'src/gfx/context.c',
  'src/gfx/gl/buffer.c',
  'src/gfx/gl/gl_core_3_3.c',
  'src/gfx/gl/program.c',
  'src/gfx/gl/sampler.c',
  'src/gfx/gl/texture.c',
  'src/gfx/gl/utils.c',
  'src/gfx/gl/vertex_array.c',
  'src/gfx/renderers/fbo_renderer.c',
  'src/gfx/renderers/legacy_renderer.c',
  'src/gfx/screenshot.c',
  'src/json/bson_parse.c',
  'src/json/bson_write.c',
  'src/json/json_base.c',
  'src/json/json_parse.c',
  'src/json/json_write.c',
  'src/log.c',
  'src/memory.c',
  'src/strings/common.c',
  'src/strings/fuzzy_match.c',
  'src/vector.c',
  'src/virtual_file.c',
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
  dep_opengl,
  uthash.get_variable('uthash_dep'),
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
