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
nul_api_version = '0.1'

gnome = import('gnome')
pkgconfig = import('pkgconfig')

deps = [
    dependency('glib-2.0'),
    dependency('gio-2.0'),
    dependency('gio-unix-2.0'),
    dependency('gobject-2.0'),
    dependency('gobject-2.0'),
    dependency('gee-0.8'),
]
nul_inc = include_directories(['.', './nul'])

subdir('data')
subdir('dbus')
subdir('nul')
# subdir('tests')
