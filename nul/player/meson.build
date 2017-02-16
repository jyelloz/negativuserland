nul_player_deps = [
  deps,
  dependency('gstreamer-1.0'),
  libnul_dbus_gen_dep,
]
executable(
  'nul-player',
  'nul-player-gst.c',
  'nul-player-application.c',
  'nul-player-main.c',
  include_directories: include_directories(['.']),
  c_args: ['-DG_LOG_DOMAIN="NulPlayer"'],
  dependencies: nul_player_deps,
)
