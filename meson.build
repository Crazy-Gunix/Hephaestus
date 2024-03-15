project('hephaestus', 'c',
  version: '0.0.1',
  default_options: [
    'c_std=c17'
  ]
)

cmake = import('cmake')

raylib_dep = dependency('raylib', required: false)
if not raylib_dep.found()
  raylib_subproject = cmake.subproject('raylib')
  raylib_dep = raylib_subproject.dependency('raylib')
endif

cc = meson.get_compiler('c')
m_dep = cc.find_library('m', required: false)

src = [
  'src/main.c',
  'src/mem.c',
  'src/engine.c',
  'src/file_util.c',
  'src/lua_util.c',
  'src/json_util.c',
  'src/archive_util.c',
  'src/raygui_impl.c'
]

deps = [
  m_dep,
  raylib_dep,
  dependency('lua'),
  dependency('jansson'),
  dependency('libarchive')
]

incdirs = [
  include_directories('include'),
  include_directories('external/raygui/src'),
  include_directories('external/raygui/styles')
]

executable('hephaestus', src,
  dependencies: deps,
  include_directories: incdirs
)

