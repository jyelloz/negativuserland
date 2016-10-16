#include "nul-music-service-util.h"

#include "nul-music-service.h"

#include <glib.h>

#include "artists.c"
#include "albums.c"
#include "tracks.c"

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
