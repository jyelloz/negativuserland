libnul_dbus_gen_music_src = gnome.gdbus_codegen(
  'nul-music-service',
  'music-service.xml',
  namespace: 'Nul',
  interface_prefix: 'org.negativuserland.',
)

libnul_dbus_gen_geolocation_src = gnome.gdbus_codegen(
  'nul-geolocation-service',
  'geolocation-service.xml',
  namespace: 'Nul',
  interface_prefix: 'org.negativuserland.',
)

libnul_dbus_gen_deps = [
  dependency('glib-2.0'),
  dependency('gio-2.0'),
  dependency('gio-unix-2.0'),
]

libnul_dbus_gen_inc = include_directories(['..', '.'])

libnul_dbus_gen_dep = declare_dependency(
  include_directories: libnul_dbus_gen_inc,
  dependencies: libnul_dbus_gen_deps,
  sources: [libnul_dbus_gen_music_src, libnul_dbus_gen_geolocation_src],
)
