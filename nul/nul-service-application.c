#include "nul-service-application.h"

#include "nul-music-service-util.h"
#include "nul-geolocation-service-util.h"

struct _NulServiceApplication
{
  GApplication parent_instance;

  GDBusInterfaceSkeleton *music;
  GDBusInterfaceSkeleton *geolocation;

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
startup_cb (GApplication *const app, gpointer const user_data)
{

  NulServiceApplication *const self = NUL_SERVICE_APPLICATION (app);
  GDBusConnection *const conn = g_application_get_dbus_connection (app);

  /* TODO: move these into a dbus_register and add dbus_unregister */
  g_dbus_interface_skeleton_export (
    self->music,
    conn,
    "/",
    NULL
  );

  g_dbus_interface_skeleton_export (
    self->geolocation,
    conn,
    "/",
    NULL
  );

}

static void
nul_service_application_init (NulServiceApplication *const self)
{

  g_signal_connect (
    self,
    "startup",
    G_CALLBACK (startup_cb),
    NULL
  );

  self->music = nul_music_service_util_get_skeleton ();
  self->geolocation = nul_geolocation_service_util_get_skeleton ();

}
