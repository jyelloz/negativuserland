#ifndef NUL_WINDOW_H
#define NUL_WINDOW_H

#include <glib-object.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define NUL_TYPE_WINDOW (nul_window_get_type ())

G_DECLARE_FINAL_TYPE (NulWindow, nul_window, NUL, WINDOW, GtkWindow)

NulWindow *
nul_window_new (void);

G_END_DECLS

#endif /* NUL_WINDOW_H */
