#include "nul-ui-main-menu.h"

#include "nul-ui-application.h"

#include "nul-ui-artists.h"
#include "nul-ui-artist-albums.h"

#include <gio/gio.h>
#include <gtk/gtk.h>

struct _NulUiMainMenu {

  GtkListBox *main_menu;

  GtkWidget *music_screen;
  GtkWidget *geolocation;
  GtkWidget *automotive;
  GtkWidget *settings;

  GtkStack *connected_state;

  NulUiArtists *artists;
  NulUiArtistAlbums *artist_albums;

};

static void
activate_main_menu (GSimpleAction *const action,
                    GVariant      *const parameter,
                    gpointer       const user_data)
{
  NulUiMainMenu *const self = (NulUiMainMenu *) user_data;
  gtk_stack_set_visible_child (
    self->connected_state,
    GTK_WIDGET (self->main_menu)
  );
}

static void
activate_music (GSimpleAction *const action,
                GVariant      *const parameter,
                gpointer       const user_data)
{
  NulUiMainMenu *const self = (NulUiMainMenu *) user_data;
  gtk_stack_set_visible_child (
    self->connected_state,
    self->music_screen
  );
}

static inline GtkStack *
get_music_screen_stack (NulUiApplication *const app)
{

  GtkBuilder *const bld = nul_ui_application_get_builder (app);

  return GTK_STACK (gtk_builder_get_object (bld, "music-screen-stack"));

}

static void
artists_ready_cb (NulMusicService *const music,
                  GAsyncResult    *const result,
                  NulUiMainMenu   *const self)
{

  nul_debug ("artists are ready");

  NulUiApplication *const app = nul_ui_application_get_default ();
  GtkStack *const stack = get_music_screen_stack (app);

  g_autoptr(GError) error = NULL;
  g_autoptr(GVariant) artists = NULL;

  nul_music_service_call_get_artists_finish (
    music,
    &artists,
    result,
    &error
    );

  if (error) {
    nul_error ("failed to get artists: %s", error->message);
  }

  nul_ui_artists_update (self->artists, artists);

  gtk_stack_set_visible_child_name (
    stack,
    "artists-list-box"
  );

}

static void
activate_music_artists (GSimpleAction *const action,
                        GVariant      *const parameter,
                        gpointer       const user_data)
{

  nul_debug ("activating artists");

  NulUiApplication *const app = nul_ui_application_get_default ();
  NulMusicService *const music = nul_ui_application_get_music (app);

  nul_music_service_call_get_artists (
    music,
    0,
    10,
    NULL,
    (GAsyncReadyCallback) artists_ready_cb,
    user_data
  );

}

static void
artist_albums_ready_cb (NulMusicService *const music,
                        GAsyncResult    *const result,
                        NulUiMainMenu   *const self)
{

  nul_debug ("artist albums are ready");

  NulUiApplication *const app = nul_ui_application_get_default ();
  GtkStack *const stack = get_music_screen_stack (app);

  g_autoptr(GError) error = NULL;
  g_autoptr(GVariant) albums = NULL;

  nul_music_service_call_get_albums_for_artist_finish (
    music,
    &albums,
    result,
    &error
  );

  if (error) {
    nul_error ("failed to get albums for artist: %s", error->message);
  }

  nul_ui_artist_albums_update (self->artist_albums, albums);

  gtk_stack_set_visible_child_name (
    GTK_STACK (stack),
    "artist-albums-list-box"
  );

}

static void
activate_music_artist_albums (GSimpleAction *const action,
                              GVariant      *const parameter,
                              gpointer       const user_data)
{

  guint64 const artist_id = g_variant_get_uint64 (parameter);
  nul_debug ("activating albums for artist#%" G_GUINT64_FORMAT, artist_id);

  NulUiApplication *const app = nul_ui_application_get_default ();
  NulMusicService *const music = nul_ui_application_get_music (app);

  nul_music_service_call_get_albums_for_artist (
    music,
    artist_id,
    0,
    10,
    NULL,
    (GAsyncReadyCallback) artist_albums_ready_cb,
    user_data
  );

}

static void
activate_music_album_tracks (GSimpleAction *const action,
                             GVariant      *const parameter,
                             gpointer       const user_data)
{
  guint64 const album_id = g_variant_get_uint64 (parameter);
  nul_debug ("activating album#%" G_GUINT64_FORMAT, album_id);
}

static void
activate_geolocation (GSimpleAction *const action,
                      GVariant      *const parameter,
                      gpointer       const user_data)
{
  NulUiMainMenu *const self = (NulUiMainMenu *) user_data;
  gtk_stack_set_visible_child (
    self->connected_state,
    self->geolocation
  );
}

static void
activate_automotive (GSimpleAction *const action,
                     GVariant      *const parameter,
                     gpointer       const user_data)
{
  NulUiMainMenu *const self = (NulUiMainMenu *) user_data;
  gtk_stack_set_visible_child (
    self->connected_state,
    self->automotive
  );
}

static void
activate_settings (GSimpleAction *const action,
                   GVariant      *const parameter,
                   gpointer       const user_data)
{
  NulUiMainMenu *const self = (NulUiMainMenu *) user_data;
  gtk_stack_set_visible_child (
    self->connected_state,
    self->settings
  );
}

static GActionEntry const entries[] = {
  {"main-menu", activate_main_menu, NULL, NULL, NULL},
  {"music", activate_music, NULL, NULL, NULL},
  {"music-artists", activate_music_artists, NULL, NULL, NULL},
  {"music-artist-albums", activate_music_artist_albums, "t", NULL, NULL},
  {"music-album-tracks", activate_music_album_tracks, "t", NULL, NULL},
  {"geolocation", activate_geolocation, NULL, NULL, NULL},
  {"automotive", activate_automotive, NULL, NULL, NULL},
  {"settings", activate_settings, NULL, NULL, NULL},
};

NulUiMainMenu *
nul_ui_main_menu_new (GtkListBox        *const main_menu,
                      GtkWidget         *const music_screen,
                      GtkWidget         *const geolocation,
                      GtkWidget         *const automotive,
                      GtkWidget         *const settings,
                      GtkStack          *const connected_state,
                      NulUiArtists      *const artists,
                      NulUiArtistAlbums *const artist_albums)
{

  NulUiMainMenu *const self = g_new0 (NulUiMainMenu, 1);

  self->connected_state = g_object_ref (connected_state);
  self->music_screen = g_object_ref (music_screen);
  self->geolocation = g_object_ref (geolocation);
  self->automotive = g_object_ref (automotive);
  self->settings = g_object_ref (settings);
  self->main_menu = g_object_ref (main_menu);
  self->artists = g_object_ref (artists);
  self->artist_albums = g_object_ref (artist_albums);

  return self;

}

static void
row_activated_cb (GtkListBox    *const box,
                  GtkListBoxRow *const row,
                  GActionGroup  *const group)
{

  GtkWidget *const child = gtk_bin_get_child (GTK_BIN (row));
  gchar const *const name = gtk_widget_get_name (child);

  g_action_group_activate_action (group, name, NULL);

}

void
nul_ui_main_menu_free (NulUiMainMenu *const self)
{
  g_clear_object (&self->connected_state);
  g_clear_object (&self->music_screen);
  g_clear_object (&self->geolocation);
  g_clear_object (&self->automotive);
  g_clear_object (&self->settings);
  g_clear_object (&self->main_menu);
  g_clear_object (&self->artists);
  g_clear_object (&self->artist_albums);

  g_free (self);
}

void
nul_ui_main_menu_register_actions (NulUiMainMenu *const self,
                                   GActionMap    *const map,
                                   GActionGroup  *const group)
{

  g_action_map_add_action_entries (
    map,
    entries,
    G_N_ELEMENTS (entries),
    self
  );

  g_signal_connect (
    self->main_menu,
    "row-activated",
    G_CALLBACK (row_activated_cb),
    group
  );

}
