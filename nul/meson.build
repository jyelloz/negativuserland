nul_service_deps = [
  deps,
  dependency('tracker-sparql-1.0'),
  libnul_dbus_gen_dep,
]

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

subdir('ui')
subdir('tui')
subdir('player')
