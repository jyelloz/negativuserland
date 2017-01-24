#include "nul-music-service-util.h"

#include "nul-music-service.h"

#include <glib.h>
#include <gee.h>

#include <string.h>

#include "artists.c"
#include "albums.c"
#include "tracks.c"

#define GEE_COLLECTION_SIZE(c) (GEE_IS_COLLECTION ((c)) ? \
                                gee_collection_get_size ((c)) : 0)

static GeeCollection *
get_tracks_by_album (NulMusicService *const self,
                     gchar const     *const album_id)
{

  GeeMultiMap *const tracks_by_album = GEE_MULTI_MAP (
    g_object_get_data (
      G_OBJECT (self),
      "tracks-by-album"
    )
  );

  return gee_multi_map_get (tracks_by_album, album_id);

}

static GeeCollection *
get_albums_by_artist (NulMusicService *const self,
                      gchar const     *const artist_id)
{

  GeeMultiMap *const albums_by_artist = GEE_MULTI_MAP (
    g_object_get_data (
      G_OBJECT (self),
      "albums-by-artist"
    )
  );

  return gee_multi_map_get (albums_by_artist, artist_id);

}

static gboolean
handle_get_artists (NulMusicService       *const self,
                    GDBusMethodInvocation *const invo,
                    guint64 const                offset,
                    guint64 const                limit)
{

  gsize const slice_length = MIN (ARTISTS_LENGTH - offset, limit);

  g_debug (
    "offset=%lu, limit=%lu, slice_length=%lu, n_artists=%lu",
    offset,
    limit,
    slice_length,
    ARTISTS_LENGTH
  );

  if (offset >= ARTISTS_LENGTH) {
    g_debug ("offset is over the edge, returning empty list");
    g_dbus_method_invocation_return_value (invo, g_variant_new ("(as)", NULL));
    return TRUE;
  }

  g_autofree gchar const **slice_array = g_new (const gchar *, slice_length);
  for (guint i = 0; i < slice_length; i++) {
    struct artist_t const artist = artists[offset + i];
    slice_array[i] = artist.artist;
  }

  GVariant *const slice = g_variant_new_strv (
    slice_array,
    slice_length
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

  gsize const slice_length = MIN (ALBUMS_LENGTH - offset, limit);

  g_debug (
    "offset=%lu, limit=%lu, slice_length=%lu, n_albums=%lu",
    offset,
    limit,
    slice_length,
    ALBUMS_LENGTH
  );

  if (offset >= ALBUMS_LENGTH) {
    g_debug ("offset is over the edge, returning empty list");
    g_dbus_method_invocation_return_value (invo, g_variant_new ("(as)", NULL));
    return TRUE;
  }

  g_autofree gchar const **slice_array = g_new0 (const gchar *, slice_length);
  for (guint i = 0; i < slice_length; i++) {
    struct album_t const album = albums[offset + i];
    slice_array[i] = album.album;
  }

  GVariant *const slice = g_variant_new_strv (
    slice_array,
    slice_length
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

  gsize const slice_length = MIN (TRACKS_LENGTH - offset, limit);

  g_debug (
    "offset=%lu, limit=%lu, slice_length=%lu, n_tracks=%lu",
    offset,
    limit,
    slice_length,
    TRACKS_LENGTH
  );

  if (offset >= TRACKS_LENGTH) {
    g_debug ("offset is over the edge, returning empty list");
    g_dbus_method_invocation_return_value (invo, g_variant_new ("(as)", NULL));
    return TRUE;
  }

  g_autofree gchar const **slice_array = g_new0 (const gchar *, slice_length);
  for (guint i = 0; i < slice_length; i++) {
    struct track_t const track = tracks[offset + i];
    slice_array[i] = track.url;
  }

  GVariant *const slice = g_variant_new_strv (
    slice_array,
    slice_length
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

  GeeCollection *const album_tracks = get_tracks_by_album (self, album_id);
  guint64 const n_album_tracks = GEE_COLLECTION_SIZE (album_tracks);
  gsize const slice_length = CLAMP (n_album_tracks - offset, 0, limit);

  g_debug (
    "album_id=%s"
    ", offset=%" G_GSIZE_FORMAT
    ", limit=%" G_GSIZE_FORMAT
    ", slice_length=%" G_GSIZE_FORMAT
    ", n_tracks=%" G_GUINT64_FORMAT,
    album_id,
    offset,
    limit,
    slice_length,
    n_album_tracks
  );

  if (offset >= n_album_tracks) {
    g_debug ("offset is over the edge, returning empty list");
    g_dbus_method_invocation_return_value (invo, g_variant_new ("(as)", NULL));
    g_object_unref (album_tracks);
    return TRUE;
  }

  g_autofree gchar const **album_tracks_array = (gchar const **)
    gee_collection_to_array (album_tracks, NULL);

  g_autofree gchar const **slice_array = g_memdup (
    &album_tracks_array[offset],
    slice_length * sizeof (gchar const *)
  );

  GVariant *const slice = g_variant_new_strv (
    slice_array,
    slice_length
  );

  g_dbus_method_invocation_return_value (
    invo,
    g_variant_new_tuple (&slice, 1)
  );

  g_object_unref (album_tracks);

  return TRUE;

}


static gboolean
handle_get_albums_for_artist (NulMusicService       *const self,
                              GDBusMethodInvocation *const invo,
                              gchar   const         *const artist_id,
                              guint64 const                offset,
                              guint64 const                limit)
{

  GeeCollection *const albums = get_albums_by_artist (self, artist_id);
  guint64 const n_albums = GEE_COLLECTION_SIZE (albums);
  gsize const slice_length = CLAMP (n_albums - offset, 0, limit);

  g_debug (
    "artist_id=%s"
    ", offset=%" G_GSIZE_FORMAT
    ", limit=%" G_GSIZE_FORMAT
    ", slice_length=%" G_GSIZE_FORMAT
    ", n_tracks=%" G_GUINT64_FORMAT,
    artist_id,
    offset,
    limit,
    slice_length,
    n_albums
  );

  if (offset >= n_albums) {
    g_debug ("offset is over the edge, returning empty list");
    g_dbus_method_invocation_return_value (invo, g_variant_new ("(as)", NULL));
    g_object_unref (albums);
    return TRUE;
  }

  g_autofree gchar const **albums_array = (gchar const **)
    gee_collection_to_array (albums, NULL);

  g_autofree gchar const **slice_array = g_memdup (
    &albums_array[offset],
    slice_length * sizeof (gchar const *)
  );

  GVariant *const slice = g_variant_new_strv (
    slice_array,
    slice_length
  );

  g_dbus_method_invocation_return_value (
    invo,
    g_variant_new_tuple (&slice, 1)
  );

  g_object_unref (albums);

  return TRUE;

}

static GeeMultiMap *
build_index_albums_by_artist (void)
{

  GeeTreeMultiMap *const albums_by_artist = gee_tree_multi_map_new (
    G_TYPE_STRING,
    (GBoxedCopyFunc) g_strdup,
    g_free,
    G_TYPE_STRING,
    (GBoxedCopyFunc) g_strdup,
    g_free,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
  );

  GeeMultiMap *const m = GEE_MULTI_MAP (albums_by_artist);

  return m;

}

static GeeMultiMap *
build_index_tracks_by_album (void)
{

  GeeTreeMultiMap *const tracks_by_album = gee_tree_multi_map_new (
    G_TYPE_STRING,
    (GBoxedCopyFunc) g_strdup,
    g_free,
    G_TYPE_STRING,
    (GBoxedCopyFunc) g_strdup,
    g_free,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
  );

  return GEE_MULTI_MAP (tracks_by_album);

}

GDBusInterfaceSkeleton *
nul_music_service_util_get_skeleton (void)
{

  NulMusicService *const music = nul_music_service_skeleton_new ();

  g_object_set_data_full (
    G_OBJECT (music),
    "tracks-by-album",
    build_index_tracks_by_album (),
    (GDestroyNotify) g_object_unref
  );

  g_object_set_data_full (
    G_OBJECT (music),
    "albums-by-artist",
    build_index_albums_by_artist (),
    (GDestroyNotify) g_object_unref
  );

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

  g_object_set (
    music,
    "artists-count", ARTISTS_LENGTH,
    NULL
  );

  g_object_set (
    music,
    "albums-count", ALBUMS_LENGTH,
    NULL
  );

  g_object_set (
    music,
    "tracks-count", TRACKS_LENGTH,
    NULL
  );

  return G_DBUS_INTERFACE_SKELETON (music);

}
