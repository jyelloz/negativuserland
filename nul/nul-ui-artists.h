#ifndef NUL_UI_ARTISTS_H
#define NUL_UI_ARTISTS_H

#include <glib.h>
#include <gtk/gtk.h>

#include "nul-music-service.h"

G_BEGIN_DECLS

typedef struct _NulUiArtists NulUiArtists;

NulUiArtists *
nul_ui_artists_new (GtkBox       *const box,
                    GtkTreeView  *const tree,
                    GtkListStore *const store,
                    GtkLabel     *const status,
                    GtkButton    *const prev,
                    GtkButton    *const next);

void
nul_ui_artists_register (NulUiArtists    *const self,
                         NulMusicService *const proxy);

void
nul_ui_artists_unregister (NulUiArtists *const self);

void
nul_ui_artists_update (NulUiArtists  *const self,
                       gchar        **const artists);

void
nul_ui_artists_free (NulUiArtists *const self);

G_END_DECLS

#endif /* NUL_UI_ARTISTS_H */
