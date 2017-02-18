#include "nul-ui-artists.h"
#include "nul-music-service.h"

#include <gio/gio.h>
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
  guint row_activated_watcher;

  int page;
  int limit;

};

static void
prev_page_cb (GtkButton *const button, NulUiArtists *const self);
static void
next_page_cb (GtkButton *const button, NulUiArtists *const self);

static void
activate_music_artist_albums (GSimpleAction *const action,
                              GVariant      *const parameter,
                              gpointer       const user_data);

static GActionEntry const entries[] = {
  {"music-artist-albums", activate_music_artist_albums, "t", NULL, NULL},
};

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
  g_return_val_if_fail (GTK_IS_LABEL (status), NULL);
  g_return_val_if_fail (GTK_IS_BUTTON (prev), NULL);
  g_return_val_if_fail (GTK_IS_BUTTON (next), NULL);

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

  gtk_widget_set_sensitive (GTK_WIDGET (prev), FALSE);
  gtk_widget_set_sensitive (GTK_WIDGET (next), FALSE);

  return self;

}

static void
update_stats (NulUiArtists     *const self,
              GParamSpec const *const pspec,
              gpointer          const user_data)
{

  guint64 const artists_count = nul_music_service_get_artists_count (
    self->music
  );
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

  gtk_widget_set_sensitive (
    GTK_WIDGET (self->prev),
    offset > 0
  );

  gtk_widget_set_sensitive (
    GTK_WIDGET (self->next),
    (offset + limit) < artists_count
  );

}

static inline gchar *
get_row_artist_name (GtkTreeModel *const model,
                     GtkTreeIter  *const iter)
{

  gchar const *artist_name;

  gtk_tree_model_get (
    model,
    iter,
    2, &artist_name,
    -1
  );

  return g_strdup_printf ("%s", artist_name);

}

static inline guint64
get_row_artist_id (GtkTreeModel *const model,
                   GtkTreeIter  *const iter)
{

  gint64 artist_id;

  gtk_tree_model_get (
    model,
    iter,
    0, &artist_id,
    -1
  );

  return artist_id;

}

static void
activate_music_artist_albums (GSimpleAction *const action,
                              GVariant      *const parameter,
                              gpointer       const user_data)
{
  guint64 const artist_id = g_variant_get_uint64 (parameter);
  nul_debug ("activating artist#%" G_GUINT64_FORMAT, artist_id);
}

static void
row_activated_cb (GtkTreeView       *const view,
                  GtkTreePath       *const path,
                  GtkTreeViewColumn *const column,
                  NulUiArtists      *const self)
{

  gint *const indices = gtk_tree_path_get_indices (path);
  g_autofree gchar const *path_string = gtk_tree_path_to_string (path);
  const gint selected_index = indices == NULL ? -1 : *indices;
  GtkTreeModel *const model = gtk_tree_view_get_model (view);
  GtkWindow *const win = GTK_WINDOW (
    gtk_widget_get_toplevel (GTK_WIDGET (view))
  );

  nul_debug ("row activated: %d", selected_index);

  GtkTreeIter iter;

  if (!gtk_tree_model_get_iter (model, &iter, path)) {
    nul_critical ("could not get iterator for path %s", path_string);
    return;
  }

  g_autofree gchar *artist_name = get_row_artist_name (model, &iter);
  guint64 const artist_id = get_row_artist_id (model, &iter);

  nul_debug (
    "artist=`%s', artist_id=%" G_GUINT64_FORMAT,
    artist_name,
    artist_id
  );

  g_action_group_activate_action (
    G_ACTION_GROUP (win),
    "win.music-artist-albums",
    g_variant_new_uint64 (artist_id)
  );

}

void
nul_ui_artists_register (NulUiArtists    *const self,
                         NulMusicService *const proxy)

{

  g_return_if_fail (self->music_watcher == 0);
  g_return_if_fail (self->music == NULL);
  g_return_if_fail (NUL_IS_MUSIC_SERVICE (proxy));

  self->music = g_object_ref (proxy);

  self->music_watcher = g_signal_connect_swapped (
    proxy,
    "notify",
    (GCallback) update_stats,
    self
  );

  self->row_activated_watcher = g_signal_connect (
    self->tree,
    "row-activated",
    G_CALLBACK (row_activated_cb),
    self
  );

}

void
nul_ui_artists_unregister (NulUiArtists *const self)
{

  guint const music_watcher = self->music_watcher;
  guint const row_activated_watcher = self->row_activated_watcher;
  NulMusicService *const music = self->music;
  GtkTreeView *const tree = self->tree;

  if (!NUL_IS_MUSIC_SERVICE (music)) {
    g_return_if_fail (music_watcher == 0);
    return;
  }
  g_return_if_fail (music_watcher > 0);

  g_signal_handler_disconnect (music, music_watcher);
  g_clear_object (&self->music);
  g_object_unref (music);

  g_signal_handler_disconnect (tree, row_activated_watcher);

  self->music_watcher = 0;
  self->row_activated_watcher = 0;

}

void
nul_ui_artists_update (NulUiArtists *const self,
                       GVariant     *const artists)
{

  GtkListStore *const store = self->store;

  gtk_list_store_clear (store);

  g_autoptr(GVariantIter) artists_iter = g_variant_iter_new (artists);
  g_autoptr(GVariant) artist;

  gint64 artist_id;
  gchar const *artist_name;
  gchar const *artist_urn;

  while ((artist = g_variant_iter_next_value (artists_iter))) {

    g_auto(GVariantDict) artist_dict;
    GtkTreeIter iter;

    g_variant_dict_init (&artist_dict, artist);
    g_variant_dict_lookup (
      &artist_dict,
      "name",
      "s",
      &artist_name
    );
    g_variant_dict_lookup (
      &artist_dict,
      "urn",
      "s",
      &artist_urn
    );
    g_variant_dict_lookup (
      &artist_dict,
      "id",
      "x",
      &artist_id
    );

    gtk_list_store_append (store, &iter);
    gtk_list_store_set (
      store,
      &iter,
      0, artist_id,
      1, artist_urn,
      2, artist_name,
      -1
    );

  }

  update_stats (self, NULL, NULL);

}

void
nul_ui_artists_register_actions (NulUiArtists *const self,
                                 GActionMap   *const map,
                                 GActionGroup *const group)
{

  g_action_map_add_action_entries (
    map,
    entries,
    G_N_ELEMENTS (entries),
    self
  );

}

void
nul_ui_artists_free (NulUiArtists *const self)
{

  nul_debug ("freeing NulUiArtists#%p", self);

  g_clear_object (&self->box);
  g_clear_object (&self->tree);
  g_clear_object (&self->store);
  g_clear_object (&self->prev);
  g_clear_object (&self->next);

  g_free (self);

}

static void
artists_ready_cb (NulMusicService *const proxy,
                  GAsyncResult    *const result,
                  NulUiArtists    *const self)
{

  g_autoptr(GVariant) artists;

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
