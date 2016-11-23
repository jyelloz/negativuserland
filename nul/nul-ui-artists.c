#include "nul-ui-artists.h"
#include "nul-music-service.h"

#include <gtk/gtk.h>

struct _NulUiArtists {
  GtkBox *box;
  GtkTreeView *tree;
  GtkListStore *store;
  GtkLabel *status;
  GtkButton *prev;
  GtkButton *next;
  NulMusicService *music;

  guint music_watcher;

  int page;
  int limit;

};

static void
prev_page_cb (GtkButton *const button, NulUiArtists *const self);
static void
next_page_cb (GtkButton *const button, NulUiArtists *const self);

NulUiArtists *
nul_ui_artists_new (GtkBox       *const box,
                    GtkTreeView  *const tree,
                    GtkListStore *const store,
                    GtkLabel     *const status,
                    GtkButton    *const prev,
                    GtkButton    *const next)
{

  g_return_val_if_fail (GTK_IS_BOX (box), NULL);
  g_return_val_if_fail (GTK_IS_TREE_VIEW (tree), NULL);
  g_return_val_if_fail (GTK_IS_LIST_STORE (store), NULL);
  g_return_val_if_fail (GTK_IS_BUTTON (prev), NULL);
  g_return_val_if_fail (GTK_IS_BUTTON (next), NULL);
  g_return_val_if_fail (GTK_IS_LABEL (status), NULL);

  NulUiArtists *const self = g_new0 (NulUiArtists, 1);

  self->box = g_object_ref (box);
  self->tree = g_object_ref (tree);
  self->store = g_object_ref (store);
  self->status = g_object_ref (status);
  self->prev = g_object_ref (prev);
  self->next = g_object_ref (next);

  self->page = 0;
  self->limit = 10;

  g_signal_connect (
    prev,
    "clicked",
    (GCallback) prev_page_cb,
    self
  );

  g_signal_connect (
    next,
    "clicked",
    (GCallback) next_page_cb,
    self
  );

  return self;

}

static void
update_stats (NulUiArtists     *const self,
              GParamSpec const *const pspec,
              gpointer          const user_data)
{

  guint64 const artists_count = nul_music_service_get_artists_count (self->music);
  guint64 const limit = self->limit;
  guint64 const offset = self->page * limit;
  guint64 const slice = MIN (artists_count - offset, limit);

  g_autofree gchar *status_text = g_strdup_printf (
    "artists %lu..%lu of %lu",
    offset,
    offset + slice - 1,
    artists_count
  );

  gtk_label_set_text (self->status, status_text);

}

void
nul_ui_artists_register (NulUiArtists    *const self,
                         NulMusicService *const proxy)

{

  g_return_if_fail (self->music_watcher == 0);

  self->music = g_object_ref (proxy);

  self->music_watcher = g_signal_connect_swapped (
    proxy,
    "notify",
    (GCallback) update_stats,
    self
  );

}

void
nul_ui_artists_unregister (NulUiArtists *const self)
{

  guint const music_watcher = self->music_watcher;
  NulMusicService *const music = self->music;

  g_return_if_fail (music_watcher > 0);
  g_return_if_fail (NUL_IS_MUSIC_SERVICE (music));

  g_signal_handler_disconnect (music, music_watcher);
  g_object_unref (music);

  self->music = NULL;

}

void
nul_ui_artists_update (NulUiArtists  *const self,
                       gchar        **const artists)
{

  GtkListStore *const store = self->store;

  gtk_list_store_clear (store);

  for (gchar **artist = artists; artist && *artist; artist++) {
    GtkTreeIter iter;
    gtk_list_store_append (store, &iter);
    gtk_list_store_set (
      store,
      &iter,
      0, *artist,
      -1
    );
  }

  update_stats (self, NULL, NULL);

}

void
nul_ui_artists_free (NulUiArtists *const self)
{

  g_object_unref (self->box);
  g_object_unref (self->tree);
  g_object_unref (self->store);
  g_object_unref (self->prev);
  g_object_unref (self->next);

  g_free (self);

}

static void
artists_ready_cb (NulMusicService *const proxy,
                  GAsyncResult    *const result,
                  NulUiArtists    *const self)
{

  g_autofree gchar **artists = NULL;

  nul_music_service_call_get_artists_finish (proxy, &artists, result, NULL);

  nul_ui_artists_update (self, artists);

}

static void
prev_page_cb (GtkButton *const button, NulUiArtists *const self)
{

  gint const limit = self->limit;
  gint const page = self->page = MAX (self->page - 1, 0);
  gint const offset = page * limit;

  nul_music_service_call_get_artists (
    self->music,
    offset,
    limit,
    NULL,
    (GAsyncReadyCallback) artists_ready_cb,
    self
  );

}

static void
next_page_cb (GtkButton *const button, NulUiArtists *const self)
{

  gint const artists_count = nul_music_service_get_artists_count (self->music);

  gint const limit = self->limit;
  gint const spillover = (artists_count % limit) > 0;
  gint const page_count = (artists_count / limit) + (spillover ? 1 : 0);
  gint const page = self->page = MIN (self->page + 1, page_count - 1);
  gint const offset = page * limit;

  nul_music_service_call_get_artists (
    self->music,
    offset,
    limit,
    NULL,
    (GAsyncReadyCallback) artists_ready_cb,
    self
  );

}
