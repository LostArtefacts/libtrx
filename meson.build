project(
  'libtrx',
  'c',
  default_options: [
    'c_std=c11',
    'warning_level=2',
  ],
)

libtrx_sources = [
  'src/memory.c',
]

libtrx = static_library(
  'libtrx',
  libtrx_sources,
  include_directories: ['src/'],
)
