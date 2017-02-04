#include "nul-music-service-util.h"

#include "nul-music-service.h"
#include "nul-external-autocleanups.h"

#include <glib.h>
#include <gee.h>
#include <tracker-sparql.h>

#include <string.h>

#define GEE_COLLECTION_SIZE(c) (GEE_IS_COLLECTION ((c)) ? \
                                gee_collection_get_size ((c)) : 0)

#define LIMIT_OFFSET_TEMPLATE \
  "LIMIT %" G_GUINT64_FORMAT " OFFSET %" G_GUINT64_FORMAT

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
album_track_cursor_to_variant_dict (TrackerSparqlCursor *const cursor)
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

static GeeList *
artists_cursor_to_gee_list (TrackerSparqlCursor *const cursor)
{

  g_return_val_if_fail (cursor, NULL);

  GeeLinkedList *const list = gee_linked_list_new (
    G_TYPE_VARIANT,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
  );

  GeeCollection *const c = GEE_COLLECTION (list);

  while (tracker_sparql_cursor_next (cursor, NULL, NULL)) {
    GVariant *const row = artist_cursor_to_variant_dict (cursor);
    gee_collection_add (c, row);
  }

  return GEE_LIST (list);

}

static GeeList *
tracks_cursor_to_gee_list (TrackerSparqlCursor *const cursor)
{

  g_return_val_if_fail (cursor, NULL);

  GeeLinkedList *const list = gee_linked_list_new (
    G_TYPE_VARIANT,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
  );

  GeeCollection *const c = GEE_COLLECTION (list);

  while (tracker_sparql_cursor_next (cursor, NULL, NULL)) {
    GVariant *const row = album_track_cursor_to_variant_dict (cursor);
    gee_collection_add (c, row);
  }

  return GEE_LIST (list);

}

static GeeList *
albums_cursor_to_gee_list (TrackerSparqlCursor *const cursor)
{

  g_return_val_if_fail (cursor, NULL);

  GeeLinkedList *const list = gee_linked_list_new (
    G_TYPE_VARIANT,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
  );

  GeeCollection *const c = GEE_COLLECTION (list);

  while (tracker_sparql_cursor_next (cursor, NULL, NULL)) {
    GVariant *const row = album_cursor_to_variant_dict (cursor);
    gee_collection_add (c, row);
  }

  return GEE_LIST (list);

}

static GeeList *
artist_album_cursor_to_gee_list (TrackerSparqlCursor *const cursor)
{

  g_return_val_if_fail (cursor, NULL);

  GeeLinkedList *const list = gee_linked_list_new (
    G_TYPE_VARIANT,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
  );

  GeeCollection *const c = GEE_COLLECTION (list);

  while (tracker_sparql_cursor_next (cursor, NULL, NULL)) {
    GVariant *const row = artist_album_cursor_to_variant_dict (cursor);
    gee_collection_add (c, row);
  }

  return GEE_LIST (list);

}

static GeeList *
album_track_cursor_to_gee_list (TrackerSparqlCursor *const cursor)
{

  g_return_val_if_fail (cursor, NULL);

  GeeLinkedList *const list = gee_linked_list_new (
    G_TYPE_VARIANT,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
  );

  GeeCollection *const c = GEE_COLLECTION (list);

  while (tracker_sparql_cursor_next (cursor, NULL, NULL)) {
    GVariant *const row = album_track_cursor_to_variant_dict (cursor);
    gee_collection_add (c, row);
  }

  return GEE_LIST (list);

}

static gboolean
handle_get_artists (NulMusicService       *const self,
                    GDBusMethodInvocation *const invo,
                    guint64 const                offset,
                    guint64 const                limit)
{

  g_autoptr(TrackerSparqlConnection) conn = tracker_sparql_connection_get (
    NULL,
    NULL
  );
  g_autofree gchar *sparql = g_strdup_printf (
    get_artists_sparql,
    limit,
    offset
  );

  g_debug ("running query %s", sparql);

  g_autoptr(TrackerSparqlCursor) cursor = tracker_sparql_connection_query (
    conn,
    sparql,
    NULL,
    NULL
  );

  if (cursor == NULL) {
    g_debug ("no results, returning empty list");
    g_dbus_method_invocation_return_value (
      invo,
      g_variant_new ("(aa{sv})", NULL)
    );
    return TRUE;
  }

  g_autoptr(GeeList) items = artists_cursor_to_gee_list (cursor);
  gint array_length;
  g_autofree GVariant **array = (GVariant **) gee_collection_to_array (
    GEE_COLLECTION (items),
    &array_length
  );

  GVariant *const slice = g_variant_new_array (
    G_VARIANT_TYPE_VARDICT,
    array,
    array_length
  );

  g_dbus_method_invocation_return_value (
    invo,
    g_variant_new_tuple (&slice, 1)
  );

  return TRUE;

}

static gboolean
handle_get_albums (NulMusicService       *const self,
                   GDBusMethodInvocation *const invo,
                   guint64 const                offset,
                   guint64 const                limit)
{

  g_autoptr(TrackerSparqlConnection) conn = tracker_sparql_connection_get (
    NULL,
    NULL
  );
  g_autofree gchar *sparql = g_strdup_printf (
    get_albums_sparql,
    limit,
    offset
  );

  g_debug ("running query %s", sparql);

  g_autoptr(TrackerSparqlCursor) cursor = tracker_sparql_connection_query (
    conn,
    sparql,
    NULL,
    NULL
  );

  if (cursor == NULL) {
    g_debug ("no results, returning empty list");
    g_dbus_method_invocation_return_value (
      invo,
      g_variant_new ("(aa{sv})", NULL)
    );
    return TRUE;
  }

  g_autoptr(GeeList) items = albums_cursor_to_gee_list (cursor);
  gint array_length;
  g_autofree GVariant **array = (GVariant **) gee_collection_to_array (
    GEE_COLLECTION (items),
    &array_length
  );

  GVariant *const slice = g_variant_new_array (
    G_VARIANT_TYPE_VARDICT,
    array,
    array_length
  );

  g_dbus_method_invocation_return_value (
    invo,
    g_variant_new_tuple (&slice, 1)
  );

  return TRUE;

}

static gboolean
handle_get_tracks (NulMusicService       *const self,
                   GDBusMethodInvocation *const invo,
                   guint64 const                offset,
                   guint64 const                limit)
{

  g_autoptr(TrackerSparqlConnection) conn = tracker_sparql_connection_get (
    NULL,
    NULL
  );
  g_autofree gchar *sparql = g_strdup_printf (
    get_tracks_sparql,
    limit,
    offset
  );

  g_debug ("running query %s", sparql);

  g_autoptr(TrackerSparqlCursor) cursor = tracker_sparql_connection_query (
    conn,
    sparql,
    NULL,
    NULL
  );

  if (cursor == NULL) {
    g_debug ("no results, returning empty list");
    g_dbus_method_invocation_return_value (
      invo,
      g_variant_new ("(aa{sv})", NULL)
    );
    return TRUE;
  }

  g_autoptr(GeeList) items = tracks_cursor_to_gee_list (cursor);
  gint array_length;
  g_autofree GVariant **array = (GVariant **) gee_collection_to_array (
    GEE_COLLECTION (items),
    &array_length
  );

  GVariant *const slice = g_variant_new_array (
    G_VARIANT_TYPE_VARDICT,
    array,
    array_length
  );

  g_dbus_method_invocation_return_value (
    invo,
    g_variant_new_tuple (&slice, 1)
  );

  return TRUE;

}

static gboolean
handle_get_tracks_for_album (NulMusicService       *const self,
                             GDBusMethodInvocation *const invo,
                             gchar   const         *const album_id,
                             guint64 const                offset,
                             guint64 const                limit)
{

  g_autoptr(TrackerSparqlConnection) conn = tracker_sparql_connection_get (
    NULL,
    NULL
  );
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

  g_debug ("running query %s", sparql);

  g_autoptr(TrackerSparqlCursor) cursor = tracker_sparql_connection_query (
    conn,
    sparql,
    NULL,
    NULL
  );

  if (cursor == NULL) {
    g_debug ("no results, returning empty list");
    g_dbus_method_invocation_return_value (
      invo,
      g_variant_new ("(aa{sv})", NULL)
    );
    return TRUE;
  }

  g_autoptr(GeeList) items = album_track_cursor_to_gee_list (cursor);
  gint array_length;
  g_autofree GVariant **array = (GVariant **) gee_collection_to_array (
    GEE_COLLECTION (items),
    &array_length
  );

  GVariant *const slice = g_variant_new_array (
    G_VARIANT_TYPE_VARDICT,
    array,
    array_length
  );

  g_dbus_method_invocation_return_value (
    invo,
    g_variant_new_tuple (&slice, 1)
  );

  return TRUE;

}


static gboolean
handle_get_albums_for_artist (NulMusicService       *const self,
                              GDBusMethodInvocation *const invo,
                              gchar   const         *const artist_id,
                              guint64 const                offset,
                              guint64 const                limit)
{

  g_autoptr(TrackerSparqlConnection) conn = tracker_sparql_connection_get (
    NULL,
    NULL
  );
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

  g_debug ("running query %s", sparql);

  g_autoptr(TrackerSparqlCursor) cursor = tracker_sparql_connection_query (
    conn,
    sparql,
    NULL,
    NULL
  );

  if (cursor == NULL) {
    g_debug ("no results, returning empty list");
    g_dbus_method_invocation_return_value (
      invo,
      g_variant_new ("(aa{sv})", NULL)
    );
    return TRUE;
  }

  g_autoptr(GeeList) items = artist_album_cursor_to_gee_list (cursor);
  gint array_length;
  g_autofree GVariant **array = (GVariant **) gee_collection_to_array (
    GEE_COLLECTION (items),
    &array_length
  );

  GVariant *const slice = g_variant_new_array (
    G_VARIANT_TYPE_VARDICT,
    array,
    array_length
  );

  g_dbus_method_invocation_return_value (
    invo,
    g_variant_new_tuple (&slice, 1)
  );

  return TRUE;

}

static inline gint64
load_integer_value (gchar const *const sparql)
{

  g_autoptr(TrackerSparqlConnection) conn = tracker_sparql_connection_get (
    NULL,
    NULL
  );

  g_debug ("running query %s", sparql);

  g_autoptr(TrackerSparqlCursor) cursor = tracker_sparql_connection_query (
    conn,
    sparql,
    NULL,
    NULL
  );

  tracker_sparql_cursor_next (cursor, NULL, NULL);

  return tracker_sparql_cursor_get_integer (cursor, 0);

}

static gboolean
update_artists_count (NulMusicService *const self)
{

  gint64 const count = load_integer_value (get_artists_count_sparql);

  g_object_set (
    self,
    "artists-count", count,
    NULL
  );

  return G_SOURCE_REMOVE;

}

static gboolean
update_albums_count (NulMusicService *const self)
{

  gint64 const count = load_integer_value (get_albums_count_sparql);

  g_object_set (
    self,
    "albums-count", count,
    NULL
  );

  return G_SOURCE_REMOVE;

}

static gboolean
update_tracks_count (NulMusicService *const self)
{

  gint64 const count = load_integer_value (get_tracks_count_sparql);

  g_object_set (
    self,
    "tracks-count", count,
    NULL
  );

  return G_SOURCE_REMOVE;

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

  g_idle_add ((GSourceFunc) update_artists_count, music);
  g_idle_add ((GSourceFunc) update_albums_count, music);
  g_idle_add ((GSourceFunc) update_tracks_count, music);

  return G_DBUS_INTERFACE_SKELETON (music);

}
