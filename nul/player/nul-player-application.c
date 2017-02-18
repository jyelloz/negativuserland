#include "nul-player-application.h"
#include "nul-player-gst.h"

#include <gst/gst.h>

#if !GLIB_CHECK_VERSION (2, 48, 0)
#define G_APPLICATION_CAN_OVERRIDE_APP_ID 0
#endif

#define NUL_APP_ID    "org.negativuserland.Player"
#define NUL_APP_FLAGS \
  (G_APPLICATION_IS_SERVICE | G_APPLICATION_CAN_OVERRIDE_APP_ID)

#define gapp_class G_APPLICATION_CLASS (nul_player_application_parent_class)
#define gobj_class G_OBJECT_CLASS (nul_player_application_parent_class)

struct _NulPlayerApplication
{
  GApplication parent_instance;

  NulPlayerGst *player;

};

G_DEFINE_TYPE (
  NulPlayerApplication,
  nul_player_application,
  G_TYPE_APPLICATION
)

GApplication *
nul_player_application_new (void)
{
  return g_object_new (
    NUL_TYPE_PLAYER_APPLICATION,
    "application-id", NUL_APP_ID,
    "flags", NUL_APP_FLAGS,
    NULL
  );
}

static void
nul_player_application_finalize (GObject *const object)
{

  NulPlayerApplication *const self = NUL_PLAYER_APPLICATION (object);

  gobj_class->finalize (object);

}

static gboolean
dbus_register (GApplication    *const app,
               GDBusConnection *const conn,
               gchar const     *const path,
               GError         **const err)
{

  NulPlayerApplication *const self = NUL_PLAYER_APPLICATION (app);

  return gapp_class->dbus_register (app, conn, path, err);

}

static void
dbus_unregister (GApplication    *const app,
                 GDBusConnection *const conn,
                 gchar const     *const path)
{

  NulPlayerApplication *const self = NUL_PLAYER_APPLICATION (app);

  gapp_class->dbus_unregister (app, conn, path);

}

static void
startup (GApplication *const app)
{

  g_autoptr(GError) error = NULL;

  nul_debug ("starting up");
  NulPlayerApplication *const self = NUL_PLAYER_APPLICATION (app);

  self->player = nul_player_gst_new (&error);

  if (error) {
    nul_error ("failed to initialize player: %s", error->message);
  }

  gapp_class->startup (app);

}

static void
activate (GApplication *const app)
{
  nul_debug ("player activated");
}

static void
nul_player_application_class_init (NulPlayerApplicationClass *const cls)
{

  GApplicationClass *const app_class = G_APPLICATION_CLASS (cls);
  GObjectClass *const object_class = G_OBJECT_CLASS (cls);

  object_class->finalize = nul_player_application_finalize;

  app_class->dbus_register = dbus_register;
  app_class->dbus_unregister = dbus_unregister;
  app_class->activate = activate;
  app_class->startup = startup;

}

static void
nul_player_application_init (NulPlayerApplication *const self)
{

  nul_debug ("initing application");

  GApplication *const app = G_APPLICATION (self);

  GOptionGroup *const gst_group = gst_init_get_option_group ();

  g_application_add_option_group (app, gst_group);

}
