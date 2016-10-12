nul_ui_deps = [
  deps,
  dependency('gtk+-3.0'),
  libnul_dbus_gen_dep,
]

nul_tui_stfl_deps = [
  deps,
  dependency('stfl'),
  libnul_dbus_gen_dep,
]

nul_service_deps = [
  deps,
  libnul_dbus_gen_dep,
]

executable(
  'nul-ui',
  'nul-window.c',
  'nul-ui-main.c',
  dependencies: nul_ui_deps,
)

executable(
  'nul-tui-stfl',
  'nul-tui-stfl-main.c',
  dependencies: nul_tui_stfl_deps,
)

executable(
  'nul-service',
  'nul-media-player.c',
  'nul-service-main.c',
  'nul-service-application.c',
  'nul-music-service-util.c',
  'nul-geolocation-service-util.c',
  dependencies: nul_service_deps,
  include_directories: [demo_data_inc],
)
