#ifndef NUL_UI_APPLICATION_H
#define NUL_UI_APPLICATION_H

#include <glib-object.h>
#include <gtk/gtk.h>

#include "nul-music-service.h"

G_BEGIN_DECLS

#define NUL_UI_TYPE_APPLICATION (nul_ui_application_get_type ())

#define nul_ui_application_get_default() \
  NUL_UI_APPLICATION (g_application_get_default ())

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

NulMusicService *
nul_ui_application_get_music (NulUiApplication *const self);

G_END_DECLS

#endif /* NUL_UI_APPLICATION_H */
