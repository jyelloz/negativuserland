#ifndef NUL_UI_APPLICATION_H
#define NUL_UI_APPLICATION_H

#include <glib-object.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define NUL_UI_TYPE_APPLICATION (nul_ui_application_get_type ())

G_DECLARE_FINAL_TYPE (
  NulUiApplication,
  nul_ui_application,
  NUL_UI,
  APPLICATION,
  GtkApplication
)

GtkApplication *
nul_ui_application_new (void);

GtkBuilder *
nul_ui_application_get_builder (NulUiApplication *const self);

GtkBuilder *
nul_ui_application_get_builder_default (void);

G_END_DECLS

#endif /* NUL_UI_APPLICATION_H */
