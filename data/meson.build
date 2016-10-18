configure_file(
  input: 'negativuserland.desktop.in',
  output: 'negativuserland.desktop',
  configuration: cdata,
)

nul_tui_resources = gnome.compile_resources(
  'nul-tui-resources',
  'nul-tui.gresource.xml',
)

nul_ui_resources = gnome.compile_resources(
  'nul-ui-resources',
  'nul-ui.gresource.xml',
)

subdir('demo')
