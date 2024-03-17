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

enginel_src = [
  'src/enginel/enginel.c',
  'src/enginel/graphics.c',
  'src/enginel/text.c',
  'src/enginel/input.c',
  'src/enginel/window.c'
]

engine_src = [
  'src/engine/mem.c',
  'src/engine/engine.c',
  'src/engine/file_util.c',
  'src/engine/lua_util.c',
  'src/engine/json_util.c',
  'src/engine/archive_util.c',
  'src/engine/raygui_impl.c'
]

enginel = library('enginel', enginel_src,
  dependencies: deps,
  include_directories: incdirs
)
engine = library('engine', engine_src,
  dependencies: deps,
  include_directories: incdirs,
  link_with: enginel
)

executable('hephaestus', 'src/main.c',
  dependencies: deps,
  include_directories: incdirs,
  link_with: engine
)

