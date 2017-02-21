#ifndef NUL_UI_ARTIST_ALBUMS_H
#define NUL_UI_ARTIST_ALBUMS_H

#include <glib.h>
#include <glib-object.h>
#include <gio/gio.h>
#include <gtk/gtk.h>

#include "nul-music-service.h"

G_BEGIN_DECLS

#define NUL_UI_TYPE_ARTIST_ALBUMS (nul_ui_artist_albums_get_type ())

G_DECLARE_FINAL_TYPE (
  NulUiArtistAlbums,
  nul_ui_artist_albums,
  NUL_UI,
  ARTIST_ALBUMS,
  GObject
)

NulUiArtistAlbums *
nul_ui_artist_albums_new              (GtkBox            *const box,
                                       GtkTreeView       *const tree,
                                       GtkListStore      *const store,
                                       GtkLabel          *const status,
                                       GtkButton         *const prev,
                                       GtkButton         *const next);

void
nul_ui_artist_albums_register         (NulUiArtistAlbums *const self,
                                       NulMusicService   *const proxy);

void
nul_ui_artist_albums_unregister       (NulUiArtistAlbums *const self);

void
nul_ui_artist_albums_register_actions (NulUiArtistAlbums *const self,
                                       GActionMap        *const map,
                                       GActionGroup      *const group);

void
nul_ui_artist_albums_update           (NulUiArtistAlbums *const self,
                                       GVariant          *const artists);

G_END_DECLS

#endif /* NUL_UI_ARTIST_ALBUMS_H */
