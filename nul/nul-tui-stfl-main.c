#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <stfl.h>

#include <glib.h>
#include <gio/gio.h>

static struct stfl_form *form = NULL;

static void
service_appeared_cb (GDBusConnection *const conn,
                     const gchar     *const name,
                     const gchar     *const name_owner,
                     gpointer         const user_data)
{

  /* g_message ("service appeared"); */
  stfl_set (form, L"root_style", L"bg=green");
  stfl_redraw ();
  stfl_run (form, -1);

}

static void
service_vanished_cb (GDBusConnection *const conn,
                     const gchar     *const name,
                     gpointer         const user_data)
{

  /* g_message ("service vanished"); */
  stfl_set (form, L"root_style", L"bg=red");
  stfl_redraw ();
  stfl_run (form, -1);

}

static const int *const markup =
L"vbox"
L"  @style_normal[root_style]:bg=blue";

static void
startup_cb (GApplication *const app,
            gpointer      const user_data)
{

  GDBusConnection *const conn = g_application_get_dbus_connection (app);

  form = stfl_create (markup);

  g_bus_watch_name_on_connection (
    conn,
    "org.negativuserland.Service",
    G_BUS_NAME_WATCHER_FLAGS_NONE,
    (GBusNameAppearedCallback) service_appeared_cb,
    (GBusNameVanishedCallback) service_vanished_cb,
    NULL,
    NULL
  );

  stfl_run (form, -1);

}

static void
activate_cb (GApplication *const app,
             gpointer      const user_data)
{
  g_message ("activated");
}

gint
main (const gint          argc,
      const gchar **const argv)
{

  GApplication *const app = g_application_new (
    "org.negativuserland.Tui",
    G_APPLICATION_IS_SERVICE
  );

  g_signal_connect (app, "startup", G_CALLBACK (startup_cb), NULL);
  g_signal_connect (app, "activate", G_CALLBACK (activate_cb), NULL);

  g_application_hold (app);

  return g_application_run (app, 0, NULL);

}
