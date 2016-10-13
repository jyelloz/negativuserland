#include "nul-music-service-util.h"

#include "nul-music-service.h"

#include "artists.c"

static gboolean
handle_get_artists (NulMusicService       *const self,
                    GDBusMethodInvocation *const invo,
                    guint64 const                offset,
                    guint64 const                limit)
{

  gsize const slice_length = MIN (ARTISTS_LENGTH - offset, limit);

  g_debug (
    "offset=%lu, limit=%lu, slice_length=%lu",
    offset,
    limit,
    slice_length
  );

  if (offset >= ARTISTS_LENGTH) {
    g_debug ("offset is over the edge, returning empty list");
    g_dbus_method_invocation_return_value (invo, g_variant_new ("(as)", NULL));
    return TRUE;
  }

  GVariant *const slice = g_variant_new_strv (
    &(artists[offset]),
    slice_length
  );

  g_dbus_method_invocation_return_value (
    invo,
    g_variant_new_tuple (&slice, 1)
  );

  return TRUE;

}

static gboolean
update_music_service (NulMusicService *const music)
{

  g_object_set (
    music,
    "albums-count", (guint) g_random_int_range (9000, 20000),
    NULL
  );

  g_object_set (
    music,
    "tracks-count", (guint) g_random_int_range (50000, 100000),
    NULL
  );

  return G_SOURCE_CONTINUE;

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

  g_object_set (
    music,
    "artists-count", ARTISTS_LENGTH,
    NULL
  );

  update_music_service (music);

  g_timeout_add_seconds (1, (GSourceFunc) update_music_service, music);

  return G_DBUS_INTERFACE_SKELETON (music);

}
