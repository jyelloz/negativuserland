#include "nul-tui-stfl-application.h"

#include <wchar.h>
#include <stfl.h>

#include <gio/gio.h>

#if !GLIB_CHECK_VERSION (2, 48, 0)
#define G_APPLICATION_CAN_OVERRIDE_APP_ID 0
#endif

#define NUL_APP_ID    "org.negativuserland.Tui"
#define NUL_APP_FLAGS \
  (G_APPLICATION_IS_SERVICE | G_APPLICATION_CAN_OVERRIDE_APP_ID)

#define gapp_class G_APPLICATION_CLASS (nul_tui_stfl_application_parent_class)
#define gobj_class G_OBJECT_CLASS (nul_tui_stfl_application_parent_class)

struct _NulTuiStflApplication
{
  GApplication parent_instance;
  struct stfl_form *form;
  struct stfl_ipool *ipool;
};

G_DEFINE_TYPE (
  NulTuiStflApplication,
  nul_tui_stfl_application,
  G_TYPE_APPLICATION
)

GApplication *
nul_tui_stfl_application_new (void)
{
  return g_object_new (
    NUL_TYPE_TUI_STFL_APPLICATION,
    "application-id", NUL_APP_ID,
    "flags", NUL_APP_FLAGS,
    NULL
  );
}

void
nul_tui_stfl_application_resize (NulTuiStflApplication *const self)
{
  stfl_reset (self->form);
  stfl_run (self->form, -1);
}

static void
nul_tui_stfl_application_finalize (GObject *const object)
{
  gobj_class->finalize (object);
}

static gboolean
dbus_register (GApplication    *const app,
               GDBusConnection *const conn,
               gchar const     *const path,
               GError         **const err)
{

  NulTuiStflApplication *const self = NUL_TUI_STFL_APPLICATION (app);

  return gapp_class->dbus_register (app, conn, path, err);

}

static void
dbus_unregister (GApplication    *const app,
                 GDBusConnection *const conn,
                 gchar const     *const path)
{

  NulTuiStflApplication *const self = NUL_TUI_STFL_APPLICATION (app);

  gapp_class->dbus_unregister (app, conn, path);

}

static void
activate (GApplication *const app)
{
  g_debug ("service activated");
}

static inline wchar_t const *
get_stfl_resource (NulTuiStflApplication *const self,
                   gchar const           *const path)
{

  GError *error = NULL;

  g_autoptr(GBytes) stfl = g_resources_lookup_data (
    path,
    G_RESOURCE_LOOKUP_FLAGS_NONE,
    &error
  );

  gconstpointer const stfl_data = g_bytes_get_data (stfl, NULL);

  if (stfl == NULL) {
    g_error ("failed to load STFL resource `%s': %s", path, error->message);
    return NULL;
  }

  return stfl_ipool_towc (self->ipool, stfl_data);

}

static void
startup (GApplication *const app)
{

  G_APPLICATION_CLASS (nul_tui_stfl_application_parent_class)->startup (app);

}

static void
nul_tui_stfl_application_class_init (NulTuiStflApplicationClass *const cls)
{

  GApplicationClass *const app_class = G_APPLICATION_CLASS (cls);
  GObjectClass *const object_class = G_OBJECT_CLASS (cls);

  object_class->finalize = nul_tui_stfl_application_finalize;

  app_class->dbus_register = dbus_register;
  app_class->dbus_unregister = dbus_unregister;
  app_class->activate = activate;
  app_class->startup = startup;

}

static void
nul_tui_stfl_application_init (NulTuiStflApplication *const self)
{
}
