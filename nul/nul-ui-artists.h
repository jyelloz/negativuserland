#ifndef NUL_UI_ARTISTS_H
#define NUL_UI_ARTISTS_H

#include <glib.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

typedef struct _NulUiArtists NulUiArtists;

NulUiArtists *
nul_ui_artists_new (GtkBox      *const box,
                    GtkTreeView *const tree);

void
nul_ui_artists_free (NulUiArtists *const self);

G_END_DECLS

#endif /* NUL_UI_ARTISTS_H */
