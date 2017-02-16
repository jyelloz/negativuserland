nul_ui_deps = [
  deps,
  dependency('gtk+-3.0'),
  libnul_dbus_gen_dep,
]

nul_service_deps = [
  deps,
  dependency('tracker-sparql-1.0'),
  libnul_dbus_gen_dep,
]

executable(
  'nul-ui',
  'nul-ui-window.c',
  'nul-ui-application.c',
  'nul-ui-pagination-util.c',
  'nul-ui-service-state-stack.c',
  'nul-ui-artists.c',
  'nul-ui-main-menu.c',
  'nul-ui-main.c',
  nul_ui_resources,
  include_directories: include_directories(['.']),
  dependencies: nul_ui_deps,
)

executable(
  'nul-service',
  'nul-service-main.c',
  'nul-service-application.c',
  'nul-music-service-util.c',
  'nul-sparql-query-util.c',
  'nul-geolocation-service-util.c',
  dependencies: nul_service_deps,
  include_directories: [demo_data_inc],
)

subdir('tui')
subdir('player')
