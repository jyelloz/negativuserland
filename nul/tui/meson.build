if get_option('enable_stfl')
  nul_tui_stfl_deps = [
    deps,
    dependency('stfl'),
    libnul_dbus_gen_dep,
  ]

  executable(
    'nul-tui-stfl',
    'nul-tui-stfl-main.c',
    'nul-tui-stfl-application.c',
    nul_tui_resources,
    c_args: ['-D_POSIX_SOURCE'],
    dependencies: nul_tui_stfl_deps,
    include_directories: [nul_inc],
  )
endif
