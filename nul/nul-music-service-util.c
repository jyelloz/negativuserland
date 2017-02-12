#include "nul-music-service-util.h"

#include "nul-music-service.h"
#include "nul-sparql-query-util.h"
#include <glib.h>
#include <gio/gio.h>
#include <tracker-sparql.h>

#include <string.h>

#include "nul-external-autocleanups.h"

#define LIMIT_OFFSET_TEMPLATE \
  "LIMIT %" G_GUINT64_FORMAT " OFFSET %" G_GUINT64_FORMAT

#define VARDICT_ARRAY ((GVariantType *) "aa{sv}")

typedef GVariant * (*CursorConvertItemFunc) (TrackerSparqlCursor *);

gchar const get_artists_count_sparql[] =
"SELECT "
"COUNT(?artist) "
"WHERE { "
"?artist a nmm:Artist "
"} ";

gchar const get_albums_count_sparql[] =
"SELECT "
"COUNT(?album) "
"WHERE { "
"?album a nmm:MusicAlbum "
"} ";

gchar const get_tracks_count_sparql[] =
"SELECT "
"COUNT(?track) "
"WHERE { "
"?track a nmm:MusicPiece "
"} ";

gchar const get_artists_sparql[] =
"SELECT "
"?artist "
"tracker:id(?artist) as ?id "
"nmm:artistName(?artist) as ?name "
"WHERE { "
"?artist a nmm:Artist "
"} "
LIMIT_OFFSET_TEMPLATE;

gchar const get_tracks_sparql[] =
"SELECT "
"?track "
"tracker:id(?track) as ?id "
"?url "
"nie:title(?track) as ?title "
"nmm:trackNumber(?track) as ?track_number "
"nmm:setNumber(?disc) as ?disc_number "
"WHERE { "
"?track a nmm:MusicPiece ; "
"nie:url ?url . "
"OPTIONAL { ?track nmm:musicAlbumDisc ?disc } "
"} "
"ORDER BY ?disc_number ?track_number "
LIMIT_OFFSET_TEMPLATE;

gchar const get_albums_sparql[] =
"SELECT "
"?album "
"tracker:id(?album) as ?id "
"?title "
"WHERE { "
"?album a nmm:MusicAlbum ; "
"nie:title ?title . "
"} "
LIMIT_OFFSET_TEMPLATE;

gchar const get_albums_for_artist_sparql[] =
"SELECT "
"?album "
"tracker:id(?album) as ?id "
"nie:title(?album) as ?title "
"WHERE { "
"?album a nmm:MusicAlbum ; "
"nie:title ?title ; "
"nie:url ?url ; "
"nmm:albumArtist \"urn:artist:%s\" . "
"} "
LIMIT_OFFSET_TEMPLATE;

gchar const get_tracks_for_album_sparql[] =
"SELECT "
"?track "
"tracker:id(?track) as ?id "
"?url "
"nie:title(?track) as ?title "
"nmm:trackNumber(?track) as ?track_number "
"nmm:setNumber(?disc) as ?disc_number "
"WHERE { "
"?track a nmm:MusicPiece ; "
"nie:title ?title ; "
"nie:url ?url ; "
"nmm:musicAlbum \"urn:album:%s\" . "
"OPTIONAL { ?track nmm:musicAlbumDisc ?disc } "
"} "
"ORDER BY ?disc_number ?track_number "
LIMIT_OFFSET_TEMPLATE;

static inline void
insert_string_value (TrackerSparqlCursor *const cursor,
                     GVariantDict        *const dict,
                     gchar const         *const key,
                     gint const                 column)
{

  GVariant *const value = g_variant_new_printf (
    "%s",
    tracker_sparql_cursor_get_string (cursor, column, NULL)
  );

  g_variant_dict_insert_value (dict, key, value);

}

static inline void
insert_integer_value (TrackerSparqlCursor *const cursor,
                      GVariantDict        *const dict,
                      gchar const         *const key,
                      gint const                 column)
{

  GVariant *const value = g_variant_new_int64 (
    tracker_sparql_cursor_get_integer (cursor, column)
  );

  g_variant_dict_insert_value (dict, key, value);

}

static GVariant *
artist_cursor_to_variant_dict (TrackerSparqlCursor *const cursor)
{

  GVariantDict dict;
  g_variant_dict_init (&dict, NULL);

  insert_string_value (cursor, &dict, "urn", 0);
  insert_integer_value (cursor, &dict, "id", 1);
  insert_string_value (cursor, &dict, "name", 2);

  return g_variant_dict_end (&dict);

}

static GVariant *
album_cursor_to_variant_dict (TrackerSparqlCursor *const cursor)
{

  GVariantDict dict;
  g_variant_dict_init (&dict, NULL);

  insert_string_value (cursor, &dict, "urn", 0);
  insert_integer_value (cursor, &dict, "id", 1);
  insert_string_value (cursor, &dict, "name", 2);

  return g_variant_dict_end (&dict);

}

static GVariant *
artist_album_cursor_to_variant_dict (TrackerSparqlCursor *const cursor)
{

  GVariantDict dict;
  g_variant_dict_init (&dict, NULL);

  insert_string_value (cursor, &dict, "urn", 0);
  insert_integer_value (cursor, &dict, "id", 1);
  insert_string_value (cursor, &dict, "title", 2);

  return g_variant_dict_end (&dict);

}

static GVariant *
track_cursor_to_variant_dict (TrackerSparqlCursor *const cursor)
{

  GVariantDict dict;
  g_variant_dict_init (&dict, NULL);

  insert_string_value (cursor, &dict, "urn", 0);
  insert_integer_value (cursor, &dict, "id", 1);
  insert_string_value (cursor, &dict, "url", 2);
  insert_string_value (cursor, &dict, "title", 3);
  insert_integer_value (cursor, &dict, "track-number", 4);
  insert_integer_value (cursor, &dict, "disc-number", 5);

  return g_variant_dict_end (&dict);

}

typedef struct _AsyncSparqlWork {
  gchar *sparql;
  TrackerSparqlCursor *cursor;
  GVariantBuilder *builder;
  GVariant *variant;
  GDBusMethodInvocation *invo;
  CursorConvertItemFunc func;
} AsyncSparqlWork;

static inline AsyncSparqlWork *
async_sparql_work_get (GTask *const task)
{
  return g_task_get_task_data (task);
}

static void
async_sparql_work_free (AsyncSparqlWork *const work)
{

  g_debug ("freeing AsyncSparqlWork#%p", work);

  if (work == NULL) {
    return;
  }

  g_variant_builder_unref (work->builder);
  g_variant_unref (work->variant);
  g_object_unref (work->cursor);
  g_free (work->sparql);
  g_free (work);

}

static void
done_cb (GObject         *const object,
         GAsyncResult    *const result,
         NulMusicService *const music)
{

  g_autoptr(GError) error = NULL;
  g_autoptr(GTask) task = G_TASK (result);

  AsyncSparqlWork *const work = (AsyncSparqlWork *) g_task_propagate_pointer (
    task,
    &error
  );

  GDBusMethodInvocation *const invo = work->invo;
  GVariant *const variant = work->variant;

  g_dbus_method_invocation_return_value (
    invo,
    g_variant_new_tuple (&variant, 1)
  );

}

static void
next_cb (GObject      *const object,
         GAsyncResult *const result,
         GTask        *const task)
{

  g_autoptr(GError) error = NULL;

  AsyncSparqlWork *const work = async_sparql_work_get (task);
  TrackerSparqlCursor *const cursor = work->cursor;

  gboolean const more = tracker_sparql_cursor_next_finish (
    cursor,
    result,
    &error
  );

  if (error) {
    g_debug ("error: %s", error->message);
    g_task_return_error (task, g_error_copy (error));
    return;
  }

  if (!more) {
    GVariant *const variant = g_variant_builder_end (work->builder);
    g_debug (
      "end of data after %" G_GSIZE_FORMAT " rows",
      g_variant_n_children (variant)
    );
    work->variant = g_variant_ref_sink (variant);
    g_task_return_pointer (task, work, NULL);
    return;
  }

  g_variant_builder_add_value (
    work->builder,
    work->func (cursor)
  );

  tracker_sparql_cursor_next_async (
    cursor,
    g_task_get_cancellable (task),
    (GAsyncReadyCallback) next_cb,
    task
  );

}

static void
query_cb (GObject      *const object,
          GAsyncResult *const result,
          GTask        *const task)
{

  g_autoptr(GError) error = NULL;

  g_autoptr(TrackerSparqlCursor) cursor = nul_sparql_query_util_query_finish (
    result,
    &error
  );

  AsyncSparqlWork *const work = async_sparql_work_get (task);
  work->cursor = g_object_ref_sink (cursor);
  work->builder = g_variant_builder_new (VARDICT_ARRAY);

  tracker_sparql_cursor_next_async (
    cursor,
    g_task_get_cancellable (task),
    (GAsyncReadyCallback) next_cb,
    task
  );

}

static inline gboolean
handle_query_async (NulMusicService       *const self,
                    GDBusMethodInvocation *const invo,
                    gchar const           *const sparql,
                    CursorConvertItemFunc  const convert_item_func)
{

  AsyncSparqlWork *const work = g_new0 (AsyncSparqlWork, 1);
  work->sparql = g_strdup (sparql);
  work->invo = invo;
  work->func = convert_item_func;

  GTask *const task = g_task_new (
    NULL,
    NULL,
    (GAsyncReadyCallback) done_cb,
    self
  );

  g_task_set_task_data (
    task,
    work,
    (GDestroyNotify) async_sparql_work_free
  );

  nul_sparql_query_util_query_async (
    sparql,
    g_task_get_cancellable (task),
    (GAsyncReadyCallback) query_cb,
    task
  );

  return TRUE;

}

static gboolean
handle_get_artists (NulMusicService       *const self,
                    GDBusMethodInvocation *const invo,
                    guint64 const                offset,
                    guint64 const                limit)
{

  g_autofree gchar *sparql = g_strdup_printf (
    get_artists_sparql,
    limit,
    offset
  );

  return handle_query_async (self, invo, sparql, artist_cursor_to_variant_dict);

}

static gboolean
handle_get_albums (NulMusicService       *const self,
                   GDBusMethodInvocation *const invo,
                   guint64 const                offset,
                   guint64 const                limit)
{

  g_autofree gchar *sparql = g_strdup_printf (
    get_albums_sparql,
    limit,
    offset
  );

  return handle_query_async (self, invo, sparql, album_cursor_to_variant_dict);

}

static gboolean
handle_get_tracks (NulMusicService       *const self,
                   GDBusMethodInvocation *const invo,
                   guint64 const                offset,
                   guint64 const                limit)
{

  g_autofree gchar *sparql = g_strdup_printf (
    get_tracks_sparql,
    limit,
    offset
  );

  return handle_query_async (self, invo, sparql, track_cursor_to_variant_dict);

}

static gboolean
handle_get_tracks_for_album (NulMusicService       *const self,
                             GDBusMethodInvocation *const invo,
                             gchar   const         *const album_id,
                             guint64 const                offset,
                             guint64 const                limit)
{

  g_autofree gchar const *album_id_safe = tracker_sparql_escape_uri_printf (
    "%s",
    album_id
  );
  g_autofree gchar *sparql = g_strdup_printf (
    get_tracks_for_album_sparql,
    album_id_safe,
    limit,
    offset
  );

  return handle_query_async (self, invo, sparql, track_cursor_to_variant_dict);

}


static gboolean
handle_get_albums_for_artist (NulMusicService       *const self,
                              GDBusMethodInvocation *const invo,
                              gchar   const         *const artist_id,
                              guint64 const                offset,
                              guint64 const                limit)
{

  g_autofree gchar const *artist_id_safe = tracker_sparql_escape_uri_printf (
    "%s",
    artist_id
  );
  g_autofree gchar *sparql = g_strdup_printf (
    get_albums_for_artist_sparql,
    artist_id_safe,
    limit,
    offset
  );

  return handle_query_async (
    self,
    invo,
    sparql,
    artist_album_cursor_to_variant_dict
  );

}

static void
update_artists_count_cb (GObject         *const object,
                         GAsyncResult    *const result,
                         NulMusicService *const music)
{

  g_autoptr(GError) error = NULL;

  gssize const count = nul_sparql_query_util_load_int_finish (
    result,
    &error
  );

  if (error) {
    g_error ("failed to get count: %s", error->message);
    return;
  }

  g_debug ("count is %" G_GINT64_FORMAT, count);

  g_object_set (
    music,
    "artists-count", count,
    NULL
  );

}

static void
update_artists_count_async (NulMusicService *const self,
                            GCancellable    *const cancellable)
{

  nul_sparql_query_util_load_int_async (
    get_artists_count_sparql,
    cancellable,
    (GAsyncReadyCallback) update_artists_count_cb,
    self
  );

}

static void
update_albums_count_cb (GObject         *const object,
                        GAsyncResult    *const result,
                        NulMusicService *const music)
{

  g_autoptr(GError) error = NULL;

  gssize const count = nul_sparql_query_util_load_int_finish (
    result,
    &error
  );

  if (error) {
    g_error ("failed to get count: %s", error->message);
    return;
  }

  g_debug ("count is %" G_GINT64_FORMAT, count);

  g_object_set (
    music,
    "albums-count", count,
    NULL
  );

}

static void
update_albums_count_async (NulMusicService *const self,
                           GCancellable    *const cancellable)
{

  nul_sparql_query_util_load_int_async (
    get_albums_count_sparql,
    cancellable,
    (GAsyncReadyCallback) update_albums_count_cb,
    self
  );

}

static void
update_tracks_count_cb (GObject         *const object,
                        GAsyncResult    *const result,
                        NulMusicService *const music)
{

  g_autoptr(GError) error = NULL;

  gssize const count = nul_sparql_query_util_load_int_finish (
    result,
    &error
  );

  if (error) {
    g_error ("failed to get count: %s", error->message);
    return;
  }

  g_debug ("count is %" G_GINT64_FORMAT, count);

  g_object_set (
    music,
    "tracks-count", count,
    NULL
  );

}

static void
update_tracks_count_async (NulMusicService *const self,
                           GCancellable    *const cancellable)
{

  nul_sparql_query_util_load_int_async (
    get_tracks_count_sparql,
    cancellable,
    (GAsyncReadyCallback) update_tracks_count_cb,
    self
  );

}

GDBusInterfaceSkeleton *
nul_music_service_util_get_skeleton (void)
{

  NulMusicService *const music = nul_music_service_skeleton_new ();

  g_signal_connect (
    music,
    "handle-get-artists",
    G_CALLBACK (handle_get_artists),
    NULL
  );

  g_signal_connect (
    music,
    "handle-get-albums",
    G_CALLBACK (handle_get_albums),
    NULL
  );

  g_signal_connect (
    music,
    "handle-get-tracks",
    G_CALLBACK (handle_get_tracks),
    NULL
  );

  g_signal_connect (
    music,
    "handle-get-tracks-for-album",
    G_CALLBACK (handle_get_tracks_for_album),
    NULL
  );

  g_signal_connect (
    music,
    "handle-get-albums-for-artist",
    G_CALLBACK (handle_get_albums_for_artist),
    NULL
  );

  update_artists_count_async (music, NULL);
  update_albums_count_async (music, NULL);
  update_tracks_count_async (music, NULL);

  return G_DBUS_INTERFACE_SKELETON (music);

}
