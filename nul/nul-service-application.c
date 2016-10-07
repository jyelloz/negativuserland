#include "nul-service-application.h"

#include "nul-music-service.h"
#include "nul-geolocation-service.h"

struct _NulServiceApplication
{
  GApplication parent_instance;

  NulMusicService       *music;
  NulGeolocationService *geolocation;

};

G_DEFINE_TYPE (
  NulServiceApplication,
  nul_service_application,
  G_TYPE_APPLICATION
)

GApplication *
nul_service_application_new (void)
{
  return g_object_new (
    NUL_TYPE_SERVICE_APPLICATION,
    "application-id", "org.negativuserland.Service",
    "flags", G_APPLICATION_IS_SERVICE,
    NULL
  );
}

static void
nul_service_application_finalize (GObject *const object)
{

  NulServiceApplication *const self = (NulServiceApplication *) object;

  g_object_unref (self->geolocation);
  g_object_unref (self->music);

  G_OBJECT_CLASS (nul_service_application_parent_class)->finalize (object);

}

static void
nul_service_application_class_init (NulServiceApplicationClass *const cls)
{

  GObjectClass *const object_class = G_OBJECT_CLASS (cls);

  object_class->finalize = nul_service_application_finalize;

}

static void
nul_service_application_init (NulServiceApplication *const self)
{
}

#if 0
/* TODO: integrate with dbus skeletons */
static gboolean
handle_get_artists (NulMusicService       *const self,
                    GDBusMethodInvocation *const invo,
                    guint64 const                offset,
                    guint64 const                limit)
{

  g_debug ("offset=%lu, limit=%lu", offset, limit);

  g_dbus_method_invocation_return_value (invo, g_variant_new ("(as)", NULL));

  return TRUE;

}

static inline GDBusConnection *
get_dbus_connection (NulServiceApplication *const app)
{
  return g_application_get_dbus_connection (G_APPLICATION (app));
}

static gboolean
setup_music_service (NulServiceApplication *const app,
                     GError               **const error)
{

  GDBusConnection *const conn = get_dbus_connection (app);
  NulMusicService *const music = app->music = nul_music_service_skeleton_new ();

  g_signal_connect (
    music,
    "handle-get-artists",
    G_CALLBACK (handle_get_artists),
    NULL
  );

  return g_dbus_interface_skeleton_export (
    G_DBUS_INTERFACE_SKELETON (music),
    conn,
    "/",
    error
  );

}

static gboolean
setup_geolocation_service (NulServiceApplication *const app,
                           GError               **const error)
{

  GDBusConnection *const conn = get_dbus_connection (app);
  NulGeolocationService *const geolocation = app->geolocation =
    nul_geolocation_service_skeleton_new ();

  return g_dbus_interface_skeleton_export (
    G_DBUS_INTERFACE_SKELETON (geolocation),
    conn,
    "/",
    error
  );

}
#endif
