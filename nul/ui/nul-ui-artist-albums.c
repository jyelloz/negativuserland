#include "nul-ui-artist-albums.h"

#define gobj_class G_OBJECT_CLASS (nul_ui_artist_albums_parent_class)

struct _NulUiArtistAlbums
{

  GObject parent_instance;

  GtkBox *box;
  GtkTreeView *tree;
  GtkListStore *store;
  GtkLabel *status;
  GtkButton *prev;
  GtkButton *next;

  guint row_activated_watcher;

};

G_DEFINE_TYPE (
  NulUiArtistAlbums,
  nul_ui_artist_albums,
  G_TYPE_OBJECT
)

#if 0
static void
row_activated_cb (GtkTreeView       *const view,
                  GtkTreePath       *const path,
                  GtkTreeViewColumn *const column,
                  NulUiArtistAlbums *const self)
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
    "music-artist-albums",
    g_variant_new_uint64 (artist_id)
  );

}
#endif

static void
nul_ui_artist_albums_finalize (GObject *const object)
{

  NulUiArtistAlbums *const self = NUL_UI_ARTIST_ALBUMS (object);

  g_clear_object (&self->box);
  g_clear_object (&self->tree);
  g_clear_object (&self->store);
  g_clear_object (&self->prev);
  g_clear_object (&self->next);

  gobj_class->finalize (object);

}

static void
nul_ui_artist_albums_class_init (NulUiArtistAlbumsClass *const cls)
{

  GObjectClass *const object_class = G_OBJECT_CLASS (cls);

  object_class->finalize = nul_ui_artist_albums_finalize;

}

static void
nul_ui_artist_albums_init (NulUiArtistAlbums *const self)
{
}

NulUiArtistAlbums *
nul_ui_artist_albums_new (GtkBox       *const box,
                          GtkTreeView  *const tree,
                          GtkListStore *const store,
                          GtkLabel     *const status,
                          GtkButton    *const prev,
                          GtkButton    *const next)
{

  g_return_val_if_fail (box && GTK_IS_BOX (box), NULL);
  g_return_val_if_fail (tree && GTK_IS_TREE_VIEW (tree), NULL);
  g_return_val_if_fail (store && GTK_IS_LIST_STORE (store), NULL);
  g_return_val_if_fail (status && GTK_IS_LABEL (status), NULL);
  g_return_val_if_fail (prev && GTK_IS_BUTTON (prev), NULL);
  g_return_val_if_fail (next && GTK_IS_BUTTON (next), NULL);

  NulUiArtistAlbums *const self = g_object_new (
    NUL_UI_TYPE_ARTIST_ALBUMS,
    NULL
  );

  self->box = g_object_ref (box);
  self->tree = g_object_ref (tree);
  self->store = g_object_ref (store);
  self->status = g_object_ref (status);
  self->prev = g_object_ref (prev);
  self->next = g_object_ref (next);

  return self;

}

void
nul_ui_artist_albums_register (NulUiArtistAlbums *const self,
                               NulMusicService   *const proxy)
{

#if 0
  self->row_activated_watcher = g_signal_connect (
    self->tree,
    "row-activated",
    G_CALLBACK (row_activated_cb),
    self
  );
#endif

}

void
nul_ui_artist_albums_unregister (NulUiArtistAlbums *const self)
{
}

void
nul_ui_artist_albums_register_actions (NulUiArtistAlbums *const self,
                                       GActionMap        *const map,
                                       GActionGroup      *const group)
{
}

void
nul_ui_artist_albums_update (NulUiArtistAlbums *const self,
                             GVariant          *const artist_albums)
{

  GtkListStore *const store = self->store;
  gtk_list_store_clear (store);

  g_autoptr(GVariantIter) artist_albums_iter = g_variant_iter_new (
    artist_albums
  );

  g_autoptr(GVariant) album;

  gint64 album_id;
  gchar const *album_name;
  gchar const *album_urn;

  while ((album = g_variant_iter_next_value (artist_albums_iter))) {

    g_auto(GVariantDict) album_dict;
    GtkTreeIter iter;

    g_variant_dict_init (&album_dict, album);
    g_variant_dict_lookup (
      &album_dict,
      "title",
      "s",
      &album_name
    );
    g_variant_dict_lookup (
      &album_dict,
      "urn",
      "s",
      &album_urn
    );
    g_variant_dict_lookup (
      &album_dict,
      "id",
      "x",
      &album_id
    );

    gtk_list_store_append (store, &iter);
    gtk_list_store_set (
      store,
      &iter,
      0, album_id,
      1, album_urn,
      2, album_name,
      -1
    );

  }

}
