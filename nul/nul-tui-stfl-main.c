#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <langinfo.h>

#include <stfl.h>

#include <glib.h>
#include <gio/gio.h>

#include "nul-music-service.h"

static struct stfl_form *form = NULL;
static struct stfl_ipool *ipool = NULL;

static void
artists_ready_cb (NulMusicService *const proxy,
                  GAsyncResult    *const result,
                  gpointer         const user_data)
{

  g_autofree gchar **artists = NULL;

  nul_music_service_call_get_artists_finish (proxy, &artists, result, NULL);

  stfl_modify (
    form,
    L"artists",
    L"replace",
    L"list[artists]"
  );

  for (gchar *const *artist = artists; artist != NULL; artist++) {

    if (*artist == NULL) {
      break;
    }

    g_autofree gchar *line = g_strdup_printf (
      "{listitem text:\"%s\"}",
      *artist
    );

    g_autofree wchar_t const *line_stfl = stfl_ipool_towc (
      ipool,
      line
    );

    stfl_modify (
      form,
      L"artists",
      L"append",
      line_stfl
    );

  }

  stfl_run (form, -1);

}

static void
music_service_ready_cb (NulMusicService *const proxy,
                        GAsyncResult    *const result,
                        gpointer         const user_data)
{
  nul_music_service_call_get_artists (
    proxy,
    1000,
    50,
    NULL,
    (GAsyncReadyCallback) artists_ready_cb,
    NULL
  );
}

static void
service_appeared_cb (GDBusConnection *const conn,
                     const gchar     *const name,
                     const gchar     *const name_owner,
                     gpointer         const user_data)
{

  stfl_set (form, L"status_area_style", L"bg=blue,attr=bold");
  stfl_set (form, L"status_label_text", L"CONNECTED");
  stfl_redraw ();
  stfl_run (form, -1);

  nul_music_service_proxy_new (
    conn,
    G_DBUS_PROXY_FLAGS_NONE,
    "org.negativuserland.Service",
    "/org/negativuserland/Service",
    NULL,
    (GAsyncReadyCallback) music_service_ready_cb,
    user_data
  );

}

static void
service_vanished_cb (GDBusConnection *const conn,
                     const gchar     *const name,
                     gpointer         const user_data)
{

  stfl_set (form, L"status_area_style", L"bg=yellow");
  stfl_set (form, L"status_label_text", L"DOWN");
  stfl_modify (
    form,
    L"artists",
    L"replace",
    L"list[artists]"
  );

  stfl_redraw ();
  stfl_run (form, -1);

}

static void
startup_cb (GApplication *const app,
            gpointer      const user_data)
{

  GDBusConnection *const conn = g_application_get_dbus_connection (app);

  form = stfl_create (L"<data/tui.stfl>");
  ipool = stfl_ipool_create (nl_langinfo (CODESET));
  stfl_set (form, L"title_label_text", L"negatıvuserland");

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

}

static gboolean
input_cb (GIOChannel   *const chan,
          GIOCondition  const cond,
          GApplication *const app)
{

  wchar_t const *const event = stfl_run (form, 0);

  stfl_set (form, L"event_label_text", event ? event : L"");
  stfl_run (form, -1);

  return G_SOURCE_CONTINUE;

}

gint
main (gint const          argc,
      gchar const **const argv)
{

  setlocale (LC_ALL, "");

  GApplication *const app = g_application_new (
    "org.negativuserland.Tui",
    G_APPLICATION_IS_SERVICE
  );
  GIOChannel *const stdin_channel = g_io_channel_unix_new (STDIN_FILENO);

  g_io_add_watch (stdin_channel, G_IO_IN, (GIOFunc) input_cb, app);
  g_signal_connect (app, "startup", G_CALLBACK (startup_cb), NULL);
  g_signal_connect (app, "activate", G_CALLBACK (activate_cb), NULL);

  g_application_set_default (app);
  g_application_hold (app);

  return g_application_run (app, 0, NULL);

}
