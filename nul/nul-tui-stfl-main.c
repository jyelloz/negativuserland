#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <langinfo.h>
#include <sys/types.h>
#include <signal.h>

#include <stfl.h>

#include <glib.h>
#include <gio/gio.h>

#include "nul-music-service.h"
#include "nul-tui-resources.h"

#define NUL_SERVICE_NAME "org.negativuserland.Service"
#define NUL_SERVICE_PATH "/org/negativuserland/Service"

static struct stfl_form *form = NULL;
static struct stfl_ipool *ipool = NULL;

static inline wchar_t const *
create_listitem (gchar const *const text)
{

  g_autofree wchar_t const *text_wc = stfl_ipool_towc (ipool, text);
  wchar_t const *text_wc_esc = stfl_quote (text_wc);
  g_autofree gchar const *text_esc = stfl_ipool_fromwc (ipool, text_wc_esc);
  g_autofree gchar const *item = g_strdup_printf (
    "{listitem text:%s}",
    text_esc
  );

  return stfl_ipool_towc (ipool, item);

}

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

  for (gchar **artist = artists; artist && *artist; artist++) {

    g_autofree wchar_t const *line = create_listitem (*artist);

    stfl_modify (
      form,
      L"artists",
      L"append",
      line
    );

  }

  stfl_run (form, -1);

}

static void
albums_ready_cb (NulMusicService *const proxy,
                 GAsyncResult    *const result,
                 gpointer         const user_data)
{

  g_autofree gchar **albums = NULL;

  nul_music_service_call_get_albums_finish (proxy, &albums, result, NULL);

  stfl_modify (
    form,
    L"albums",
    L"replace",
    L"list[albums]"
  );

  for (gchar **album = albums; album && *album; album++) {

    g_autofree wchar_t const *line = create_listitem (*album);

    stfl_modify (
      form,
      L"albums",
      L"append",
      line
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
    0,
    25,
    NULL,
    (GAsyncReadyCallback) artists_ready_cb,
    NULL
  );
  nul_music_service_call_get_albums (
    proxy,
    0,
    25,
    NULL,
    (GAsyncReadyCallback) albums_ready_cb,
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
    NUL_SERVICE_NAME,
    NUL_SERVICE_PATH,
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

  stfl_modify (
    form,
    L"albums",
    L"replace",
    L"list[albums]"
  );

  stfl_redraw ();
  stfl_run (form, -1);

}

static inline wchar_t const *
get_stfl_resource (gchar const *const path)
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

  return stfl_ipool_towc (ipool, stfl_data);

}

static void
startup_cb (GApplication *const app,
            gpointer      const user_data)
{

  GDBusConnection *const conn = g_application_get_dbus_connection (app);

  ipool = stfl_ipool_create (nl_langinfo (CODESET));

  wchar_t const *const stfl = get_stfl_resource (
    "/org/negativuserland/Tui/tui.stfl"
  );

  form = stfl_create (stfl);
  stfl_set (form, L"title_label_text", L"negatıvuserland");

  g_bus_watch_name_on_connection (
    conn,
    NUL_SERVICE_NAME,
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

static gboolean
winch_cb (void)
{
  stfl_reset (form);
  stfl_run (form, -1);
  return G_SOURCE_REMOVE;
}

static void
winch_handler (gint const signum)
{
  g_idle_add ((GSourceFunc) winch_cb, NULL);
}

gint
main (gint const argc, gchar **const argv)
{

  setlocale (LC_ALL, "");

  GApplication *const app = g_application_new (
    "org.negativuserland.Tui",
    G_APPLICATION_IS_SERVICE
  );
  GIOChannel *const stdin_channel = g_io_channel_unix_new (STDIN_FILENO);

  {
    struct sigaction sa = {
      .sa_handler = winch_handler,
    };
    sigemptyset (&sa.sa_mask);
    sigaction (SIGWINCH, &sa, NULL);
  }

  g_io_add_watch (stdin_channel, G_IO_IN, (GIOFunc) input_cb, app);
  g_signal_connect (app, "startup", G_CALLBACK (startup_cb), NULL);
  g_signal_connect (app, "activate", G_CALLBACK (activate_cb), NULL);

  g_application_set_default (app);
  g_application_hold (app);

  return g_application_run (app, argc, argv);

}
