project(
  'negativuserland',
  'c',
  version: '0.1.0',
  default_options: [
    'warning_level=1',
    'c_std=c11',
  ],
)

nul_version = meson.project_version()

gnome = import('gnome')
pkgconfig = import('pkgconfig')

prefix = get_option('prefix')
bindir = get_option('bindir')
localedir = get_option('localedir')

cdata = configuration_data()

cdata.set_quoted('BINDIR', '@0@/@1@'.format(prefix, bindir))
cdata.set_quoted('GETTEXT_PACKAGE', 'negativuserland')
cdata.set_quoted('G_LOG_DOMAIN', 'Nul')
cdata.set_quoted('LOCALEDIR', '@0@/@1@'.format(prefix, localedir))
cdata.set_quoted('PACKAGE', 'negativuserland')
cdata.set_quoted('PACKAGE_BUGREPORT', 'http://negativuserland.org/bugs')
cdata.set_quoted('PACKAGE_NAME', 'negatıvuserland')
cdata.set_quoted('PACKAGE_TARNAME', 'negativuserland')
cdata.set_quoted('PACKAGE_URL', 'http://negativuserland.org/')
cdata.set_quoted('PACKAGE_VERSION', '@0@'.format(nul_version))

cdata.set('BINDIR_UNQUOTED', '@0@/@1@'.format(prefix, bindir))
cdata.set('LOCALEDIR_UNQUOTED', '@0@/@1@'.format(prefix, localedir))
cdata.set('PACKAGE_NAME_UNQUOTED', 'negatıvuserland')

configure_file(
  input: 'config.h.meson',
  output: 'config.h',
  configuration: cdata,
)

add_global_arguments(
  '-include', 'config.h',
  language: 'c',
)

deps = [
  dependency('glib-2.0', version: '>= 2.44'),
  dependency('gio-2.0'),
  dependency('gio-unix-2.0'),
  dependency('gobject-2.0'),
  dependency('gee-0.8'),
]

subdir('data')
subdir('dbus')
subdir('nul')
