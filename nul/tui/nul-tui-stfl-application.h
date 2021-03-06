#ifndef NUL_TUI_STFL_APPLICATION_H
#define NUL_TUI_STFL_APPLICATION_H

#include <glib-object.h>
#include <gio/gio.h>

G_BEGIN_DECLS

#define NUL_TUI_TYPE_STFL_APPLICATION (nul_tui_stfl_application_get_type ())

G_DECLARE_FINAL_TYPE (
  NulTuiStflApplication,
  nul_tui_stfl_application,
  NUL_TUI,
  STFL_APPLICATION,
  GApplication
)

GApplication *
nul_tui_stfl_application_new (void);

void
nul_tui_stfl_application_resize (NulTuiStflApplication *const self);

void
nul_tui_stfl_application_input (NulTuiStflApplication *const self);

G_END_DECLS

#endif /* NUL_TUI_STFL_APPLICATION_H */
