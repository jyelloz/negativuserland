#include <stddef.h>
#include <stdlib.h>

#include <glib.h>
#include <glib/gprintf.h>
#include <glib-object.h>
#include <gio/gio.h>

#include "nul-service-application.h"

static void
nul_service_activate_cb (GApplication *const app,
                         gpointer      const user_data)
{
  g_message ("negativuserland service is active");
}

static void
nul_service_startup_cb (GApplication *const app,
                        gpointer      const user_data)
{
  g_message ("negativuserland service has started up");
}

static void
nul_service_shutdown_cb (GApplication *const app,
                         gpointer      const user_data)
{
  g_message ("negativuserland service is shutting down");
}

gint
main (const gint          argc,
      const gchar **const argv)
{

  GApplication *const app = nul_service_application_new ();

  g_signal_connect (
    app,
    "activate",
    G_CALLBACK (nul_service_activate_cb),
    NULL
  );

  g_signal_connect (
    app,
    "startup",
    G_CALLBACK (nul_service_startup_cb),
    NULL
  );

  g_signal_connect (
    app,
    "shutdown",
    G_CALLBACK (nul_service_shutdown_cb),
    NULL
  );

  g_application_register (app, NULL, NULL);
  g_application_activate (app);
  g_application_hold (app);

  return g_application_run (app, 0, NULL);

}
