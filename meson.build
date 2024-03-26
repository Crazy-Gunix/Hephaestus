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

conf_data = configuration_data()

conf_data.set('version', '0.0.1')
if cc.has_header('syslog.h')
  conf_data.set('syslog', true)
endif
conf_data.set('color', true)

configure_file(input : 'config.h.in',
               output : 'config.h',
               configuration : conf_data)

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

util_src = [
  'src/util/file.c',
  'src/util/lua_helper.c',
  'src/util/json_helper.c',
  'src/util/archive_helper.c',
  'src/util/mem.c',
  'src/util/log.c'
]

enginel = library('enginel', enginel_src,
  dependencies: deps,
  include_directories: incdirs,
)
util = library('util', util_src,
  dependencies: deps,
  include_directories: incdirs,
  link_with: enginel
)
raygui_impl = static_library('raygui', 'src/raygui_impl.c',
  dependencies: deps,
  include_directories: incdirs
)
engine = library('engine', 'src/engine/engine.c',
  dependencies: deps,
  include_directories: incdirs,
  link_with: [enginel, util, raygui_impl]
)

executable('hephaestus', 'src/main.c',
  dependencies: deps,
  include_directories: incdirs,
  link_with: engine
)

