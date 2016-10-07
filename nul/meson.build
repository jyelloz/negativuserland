nul_ui_deps = [
  deps,
  dependency('gtk+-3.0'),
]

nul_tui_stfl_deps = [
  deps,
  dependency('stfl'),
]

nul_service_deps = deps

nul_service_sources = [
  'nul-media-player.c',
  'nul-service-main.c',
  'nul-service-application.c',
  'nul-music-service-util.c',
  'nul-geolocation-service-util.c',
]

executable(
  'nul-ui',
  sources: [
    'nul-window.c',
    'nul-ui-main.c',
  ],
  dependencies: nul_ui_deps,
  include_directories: [libnul_dbus_gen_inc],
  link_with: [libnul_dbus_gen],
)

executable(
  'nul-tui-stfl',
  sources: [
    'nul-tui-stfl-main.c',
  ],
  dependencies: nul_tui_stfl_deps,
  include_directories: [libnul_dbus_gen_inc],
  link_with: [libnul_dbus_gen],
)

executable(
  'nul-service',
  sources: nul_service_sources,
  dependencies: nul_service_deps,
  include_directories: [libnul_dbus_gen_inc],
  link_with: [libnul_dbus_gen],
)
