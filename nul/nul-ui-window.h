#ifndef NUL_UI_WINDOW_H
#define NUL_UI_WINDOW_H

#include <glib-object.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define NUL_TYPE_UI_WINDOW (nul_ui_window_get_type ())

G_DECLARE_FINAL_TYPE (NulUiWindow, nul_ui_window, NUL, WINDOW, GtkWindow)

NulUiWindow *
nul_ui_window_new (void);

G_END_DECLS

#endif /* NUL_UI_WINDOW_H */
