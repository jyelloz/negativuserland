#ifndef NUL_UI_ARTISTS_H
#define NUL_UI_ARTISTS_H

#include <glib.h>
#include <gio/gio.h>
#include <gtk/gtk.h>

#include "nul-music-service.h"

G_BEGIN_DECLS

#define NUL_UI_TYPE_ARTISTS (nul_ui_artists_get_type ())

G_DECLARE_FINAL_TYPE (
  NulUiArtists,
  nul_ui_artists,
  NUL_UI,
  ARTISTS,
  GObject
)

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
nul_ui_artists_register_actions (NulUiArtists *const self,
                                 GActionMap   *const map,
                                 GActionGroup *const group);

void
nul_ui_artists_update (NulUiArtists *const self,
                       GVariant     *const artists);

G_END_DECLS

#endif /* NUL_UI_ARTISTS_H */
