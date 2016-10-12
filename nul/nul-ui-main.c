#include <stddef.h>
#include <stdlib.h>

#include <glib.h>
#include <gtk/gtk.h>

#include "nul-ui-application.h"
#include "nul-window.h"
#include "nul-music-service.h"

static const gchar css[] =
".nul-status-box {"
"  margin: 16px;"
"  background-color: red;"
"  transition: background-color 500ms ease-in-out;"
"}"
".nul-status-box.available {"
"  background-color: lime;"
"  transition: background-color 500ms ease-in-out;"
"}";

static void
service_appeared_cb (GDBusConnection *const conn,
                     const gchar     *const name,
                     const gchar     *const name_owner,
                     GtkWidget       *const widget)
{
  g_message ("service appeared");

  GtkStyleContext *const style = gtk_widget_get_style_context (widget);
  gtk_style_context_add_class (style, "available");

}

static void
service_vanished_cb (GDBusConnection *const conn,
                     const gchar     *const name,
                     GtkWidget       *const widget)
{

  g_message ("service vanished");

  GtkStyleContext *const style = gtk_widget_get_style_context (widget);
  gtk_style_context_remove_class (style, "available");

}

static void
startup_cb (GApplication *const app,
            gpointer      const user_data)
{

  GtkWidget *const window = gtk_application_window_new (GTK_APPLICATION (app));
  GtkWidget *const widget = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  GtkStyleContext *const style = gtk_widget_get_style_context (widget);
  GtkCssProvider *const prov = gtk_css_provider_new ();

  gtk_css_provider_load_from_data (
    prov,
    css,
    sizeof (css),
    NULL
  );

  gtk_style_context_add_provider (
    style,
    GTK_STYLE_PROVIDER (prov),
    GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
  );

  gtk_style_context_add_class (style, "nul-status-box");
  gtk_style_context_add_class (style, "unavailable");

  gtk_widget_set_size_request (window, 256, 256);
  gtk_window_set_default_size (GTK_WINDOW (window), 800, 450);
  gtk_window_set_title (GTK_WINDOW (window), "Negativuserland");
  gtk_container_add (GTK_CONTAINER (window), widget);
  gtk_widget_show_all (window);

  GDBusConnection *const conn = g_application_get_dbus_connection (app);

  g_bus_watch_name_on_connection (
    conn,
    "org.negativuserland.Service",
    G_BUS_NAME_WATCHER_FLAGS_NONE,
    (GBusNameAppearedCallback) service_appeared_cb,
    (GBusNameVanishedCallback) service_vanished_cb,
    widget,
    NULL
  );

}

static void
activate_cb (GApplication *const app,
             GtkWidget    *const window)
{
  g_message ("activated");
}

gint
main (gint const         argc,
      gchar      **const argv)
{

  GApplication *const app = G_APPLICATION (nul_ui_application_new ());

  g_signal_connect (app, "startup", G_CALLBACK (startup_cb), NULL);
  g_signal_connect (app, "activate", G_CALLBACK (activate_cb), NULL);

  return g_application_run (app, argc, argv);

}
