#include "nul-service-application.h"

#include "nul-music-service-util.h"
#include "nul-geolocation-service-util.h"

#if !GLIB_CHECK_VERSION (2, 48, 0)
#define G_APPLICATION_CAN_OVERRIDE_APP_ID 0
#endif

#define NUL_APP_ID    "org.negativuserland.Service"
#define NUL_APP_FLAGS \
  (G_APPLICATION_IS_SERVICE | G_APPLICATION_CAN_OVERRIDE_APP_ID)

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
    "application-id", NUL_APP_ID,
    "flags", NUL_APP_FLAGS,
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

static gboolean
dbus_register (GApplication    *const app,
               GDBusConnection *const conn,
               gchar const     *const path,
               GError         **const err)
{

  NulServiceApplication *const self = NUL_SERVICE_APPLICATION (app);

  if (!g_dbus_interface_skeleton_export (self->music, conn, path, err)) {
    return FALSE;
  }

  if (!g_dbus_interface_skeleton_export (self->geolocation, conn, path, err)) {
    return FALSE;
  }

  return G_APPLICATION_CLASS (nul_service_application_parent_class)->
    dbus_register (app, conn, path, err);

}

static void
dbus_unregister (GApplication    *const app,
                 GDBusConnection *const conn,
                 gchar const     *const path)
{

  NulServiceApplication *const self = NUL_SERVICE_APPLICATION (app);

  g_dbus_interface_skeleton_unexport (self->music);
  g_dbus_interface_skeleton_unexport (self->geolocation);

  G_APPLICATION_CLASS (nul_service_application_parent_class)->dbus_unregister (
    app, conn, path
  );

}

static void
activate (GApplication *const app)
{
  g_debug ("service activated");
}

static void
nul_service_application_class_init (NulServiceApplicationClass *const cls)
{

  GApplicationClass *const app_class = G_APPLICATION_CLASS (cls);
  GObjectClass *const object_class = G_OBJECT_CLASS (cls);

  object_class->finalize = nul_service_application_finalize;

  app_class->dbus_register = dbus_register;
  app_class->dbus_unregister = dbus_unregister;
  app_class->activate = activate;

}

static void
nul_service_application_init (NulServiceApplication *const self)
{
  self->music = nul_music_service_util_get_skeleton ();
  self->geolocation = nul_geolocation_service_util_get_skeleton ();
}
