nul_ui_deps = [
  deps,
  dependency('gtk+-3.0'),
  libnul_dbus_gen_dep,
]

executable(
  'nul-ui',
  'nul-ui-application.c',
  'nul-ui-pagination-util.c',
  'nul-ui-service-state-stack.c',
  'nul-ui-artists.c',
  'nul-ui-artist-albums.c',
  'nul-ui-main-menu.c',
  'nul-ui-page-browser.c',
  'nul-ui-main.c',
  nul_ui_resources,
  include_directories: [nul_inc, include_directories('.')],
  dependencies: nul_ui_deps,
)
