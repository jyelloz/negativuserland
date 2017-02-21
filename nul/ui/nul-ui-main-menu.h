#ifndef NUL_UI_MAIN_MENU_H
#define NUL_UI_MAIN_MENU_H

#include <glib.h>
#include <gio/gio.h>
#include <gtk/gtk.h>

#include "nul-ui-artists.h"
#include "nul-ui-artist-albums.h"

G_BEGIN_DECLS

typedef struct _NulUiMainMenu NulUiMainMenu;

NulUiMainMenu *
nul_ui_main_menu_new              (GtkListBox        *const main_menu,
                                   GtkWidget         *const music_screen,
                                   GtkWidget         *const geolocation,
                                   GtkWidget         *const automotive,
                                   GtkWidget         *const settings,
                                   GtkStack          *const connected_state,
                                   NulUiArtists      *const artists,
                                   NulUiArtistAlbums *const artist_albums);

void
nul_ui_main_menu_free             (NulUiMainMenu     *const self);

void
nul_ui_main_menu_register_actions (NulUiMainMenu     *const self,
                                   GActionMap        *const map,
                                   GActionGroup      *const group);

G_END_DECLS

#endif /* NUL_UI_MAIN_MENU_H */
