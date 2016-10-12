#ifndef NUL_UI_APPLICATION_H
#define NUL_UI_APPLICATION_H

#include <glib-object.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define NUL_TYPE_UI_APPLICATION (nul_ui_application_get_type ())

G_DECLARE_FINAL_TYPE (
  NulUiApplication,
  nul_ui_application,
  NUL,
  UI_APPLICATION,
  GtkApplication
)

GtkApplication *
nul_ui_application_new (void);

G_END_DECLS

#endif /* NUL_UI_APPLICATION_H */
