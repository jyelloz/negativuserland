#include "nul-tui-stfl-application.h"
#include "nul-music-service.h"

#include <wchar.h>
#include <langinfo.h>

#include <stfl.h>

#include <gio/gio.h>

#if !GLIB_CHECK_VERSION (2, 48, 0)
#define G_APPLICATION_CAN_OVERRIDE_APP_ID 0
#endif

#define NUL_APP_ID    "org.negativuserland.Tui"
#define NUL_APP_FLAGS \
  (G_APPLICATION_IS_SERVICE | G_APPLICATION_CAN_OVERRIDE_APP_ID)

#define NUL_SERVICE_NAME "org.negativuserland.Service"
#define NUL_SERVICE_PATH "/org/negativuserland/Service"

#define gapp_class G_APPLICATION_CLASS (nul_tui_stfl_application_parent_class)
#define gobj_class G_OBJECT_CLASS (nul_tui_stfl_application_parent_class)

struct _NulTuiStflApplication
{
  GApplication parent_instance;
  struct stfl_form *form;
  struct stfl_ipool *ipool;
  guint service_watcher;
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

void
nul_tui_stfl_application_input (NulTuiStflApplication *const self)
{
  struct stfl_form *const form = self->form;
  wchar_t const *const event = stfl_run (form, 0);
  stfl_set (form, L"event_label_text", event ? event : L"");
  stfl_run (form, -1);
}

static inline wchar_t const *
create_listitem (struct stfl_ipool *const ipool, gchar const *const text)
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
artists_ready_cb (NulMusicService       *const proxy,
                  GAsyncResult          *const result,
                  NulTuiStflApplication *const self)
{

  g_autoptr(GVariant) artists;
  struct stfl_form *const form = self->form;
  struct stfl_ipool *const ipool = self->ipool;

  nul_music_service_call_get_artists_finish (proxy, &artists, result, NULL);

  stfl_modify (
    form,
    L"artists",
    L"replace",
    L"list[artists]"
  );

  g_autoptr(GVariant) artist;
  g_autoptr(GVariantIter) iter = g_variant_iter_new (artists);

  while ((artist = g_variant_iter_next_value (iter))) {

    g_auto(GVariantDict) artist_dict;
    g_variant_dict_init (&artist_dict, artist);

    g_autoptr(GVariant) artist_name_value = g_variant_dict_lookup_value (
      &artist_dict,
      "name",
      G_VARIANT_TYPE_STRING
    );

    gchar const *const artist_name = g_variant_get_string (
      artist_name_value,
      NULL
    );

    g_autofree wchar_t const *line = create_listitem (ipool, artist_name);

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
albums_ready_cb (NulMusicService       *const proxy,
                 GAsyncResult          *const result,
                 NulTuiStflApplication *const self)
{

  g_autoptr(GVariant) albums;
  struct stfl_form *const form = self->form;
  struct stfl_ipool *const ipool = self->ipool;

  nul_music_service_call_get_albums_finish (proxy, &albums, result, NULL);

  stfl_modify (
    form,
    L"albums",
    L"replace",
    L"list[albums]"
  );

  g_autoptr(GVariant) album;
  g_autoptr(GVariantIter) iter = g_variant_iter_new (albums);

  while ((album = g_variant_iter_next_value (iter))) {

    g_auto(GVariantDict) album_dict;
    g_variant_dict_init (&album_dict, album);

    g_autoptr(GVariant) album_name_value = g_variant_dict_lookup_value (
      &album_dict,
      "name",
      G_VARIANT_TYPE_STRING
    );

    gchar const *const album_name = g_variant_get_string (
      album_name_value,
      NULL
    );

    g_autofree wchar_t const *line = create_listitem (ipool, album_name);

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
music_service_ready_cb (NulMusicService       *const proxy,
                        GAsyncResult          *const result,
                        NulTuiStflApplication *const self)
{

  struct stfl_form *const form = self->form;
  struct stfl_ipool *const ipool = self->ipool;

  gchar const *const lower_box_h_ascii = stfl_ipool_fromwc (
    ipool,
    stfl_get (form, L"lower_box:h")
  );

  guint64 const lower_box_h = g_ascii_strtoull (lower_box_h_ascii, NULL, 0) - 1;

  nul_music_service_call_get_artists (
    proxy,
    0,
    lower_box_h,
    NULL,
    (GAsyncReadyCallback) artists_ready_cb,
    self
  );

  nul_music_service_call_get_albums (
    proxy,
    0,
    lower_box_h,
    NULL,
    (GAsyncReadyCallback) albums_ready_cb,
    self
  );

}

static void
service_appeared_cb (GDBusConnection       *const conn,
                     const gchar           *const name,
                     const gchar           *const name_owner,
                     NulTuiStflApplication *const self)
{

  struct stfl_form *const form = self->form;

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
    self
  );

}

static void
service_vanished_cb (GDBusConnection       *const conn,
                     const gchar           *const name,
                     NulTuiStflApplication *const self)
{

  struct stfl_form *const form = self->form;

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

  if (!gapp_class->dbus_register (app, conn, path, err)) {
    return FALSE;
  }

  NulTuiStflApplication *const self = NUL_TUI_STFL_APPLICATION (app);

  self->service_watcher = g_bus_watch_name_on_connection (
    conn,
    NUL_SERVICE_NAME,
    G_BUS_NAME_WATCHER_FLAGS_NONE,
    (GBusNameAppearedCallback) service_appeared_cb,
    (GBusNameVanishedCallback) service_vanished_cb,
    self,
    NULL
  );

  return TRUE;

}

static void
dbus_unregister (GApplication    *const app,
                 GDBusConnection *const conn,
                 gchar const     *const path)
{

  NulTuiStflApplication *const self = NUL_TUI_STFL_APPLICATION (app);

  if (self->service_watcher)
    g_bus_unwatch_name (self->service_watcher);

  gapp_class->dbus_unregister (app, conn, path);

}

static void
activate (GApplication *const app)
{
  g_debug ("service activated");
}

static inline wchar_t const *
get_stfl_resource (struct stfl_ipool *const ipool,
                   gchar const       *const path)
{

  g_autoptr(GError) error = NULL;

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
startup (GApplication *const app)
{

  G_APPLICATION_CLASS (nul_tui_stfl_application_parent_class)->startup (app);

  NulTuiStflApplication *const self = NUL_TUI_STFL_APPLICATION (app);

  stfl_set (
    self->form,
    L"title_label_text",
    stfl_ipool_towc (self->ipool, PACKAGE_NAME)
  );

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

  struct stfl_ipool *const ipool = self->ipool = stfl_ipool_create (
    nl_langinfo (CODESET)
  );

  wchar_t const *const stfl = get_stfl_resource (
    ipool,
    "/org/negativuserland/Tui/tui.stfl"
  );

  self->form = stfl_create (stfl);

  self->service_watcher = 0;

}
