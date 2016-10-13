#include "nul-ui-application.h"
#include "nul-music-service.h"
#include "nul-geolocation-service.h"

#if !GLIB_CHECK_VERSION (2, 48, 0)
#define G_APPLICATION_CAN_OVERRIDE_APP_ID 0
#endif

#define NUL_APP_ID "org.negativuserland.Ui"
#define NUL_APP_FLAGS G_APPLICATION_CAN_OVERRIDE_APP_ID

#define NUL_SERVICE_APP_ID "org.negativuserland.Service"
#define NUL_SERVICE_OBJECT_PATH "/org/negativuserland/Service"

struct _NulUiApplication
{
  GtkApplication parent_instance;

  guint bus_watcher;

  NulMusicService *music;
  NulGeolocationService *geolocation;

  GtkStack *front_page;

  GtkFlowBox *loading_screen;
  GtkGrid *stats_screen;

  GtkLabel *artists_count_label;
  GtkLabel *albums_count_label;
  GtkLabel *tracks_count_label;

};

G_DEFINE_TYPE (
  NulUiApplication,
  nul_ui_application,
  GTK_TYPE_APPLICATION
)

GtkApplication *
nul_ui_application_new (void)
{
  return g_object_new (
    NUL_TYPE_UI_APPLICATION,
    "application-id", NUL_APP_ID,
    "flags", NUL_APP_FLAGS,
    NULL
  );
}

static void
activate (GApplication *const app)
{

  g_debug ("ui activated");

  GtkApplication *const gtk_app = GTK_APPLICATION (app);
  NulUiApplication *const self = NUL_UI_APPLICATION (app);

  g_autoptr(GtkBuilder) builder = gtk_builder_new_from_file (
    "data/negativuserland.ui"
  );

  self->artists_count_label = GTK_LABEL (
    gtk_builder_get_object (builder, "artists-count-label")
  );
  self->albums_count_label = GTK_LABEL (
    gtk_builder_get_object (builder, "albums-count-label")
  );
  self->tracks_count_label = GTK_LABEL (
    gtk_builder_get_object (builder, "tracks-count-label")
  );

  self->front_page = GTK_STACK (
    gtk_builder_get_object (builder, "front-page")
  );
  self->loading_screen = GTK_FLOW_BOX (
    gtk_builder_get_object (builder, "loading-screen")
  );
  self->stats_screen = GTK_GRID (
    gtk_builder_get_object (builder, "stats-screen")
  );

  GtkWidget *const window = GTK_WIDGET (
    gtk_builder_get_object (builder, "main-window")
  );

  gtk_window_set_application (GTK_WINDOW (window), gtk_app);
  gtk_widget_show_all (window);

}

static inline void
update_stats_view (NulUiApplication *const self)
{

  NulMusicService *const music = self->music;

  g_autofree gchar const *artists_count_text = g_strdup_printf (
    "%lu",
    nul_music_service_get_artists_count (music)
  );

  g_autofree gchar const *albums_count_text = g_strdup_printf (
    "%lu",
    nul_music_service_get_albums_count (music)
  );

  g_autofree gchar const *tracks_count_text = g_strdup_printf (
    "%lu",
    nul_music_service_get_tracks_count (music)
  );

  gtk_label_set_text (self->artists_count_label, artists_count_text);
  gtk_label_set_text (self->albums_count_label, albums_count_text);
  gtk_label_set_text (self->tracks_count_label , tracks_count_text);

}

static void
update_stats (NulUiApplication *const self,
              GParamSpec const *const pspec,
              gpointer          const user_data)
{
  update_stats_view (self);
}

static void
music_service_ready_cb (GObject          *const obj,
                        GAsyncResult     *const res,
                        NulUiApplication *const self)
{
  g_message ("music service ready");

  NulMusicService *const music = self->music =
    nul_music_service_proxy_new_finish (res, NULL);

  update_stats_view (self);

  g_signal_connect_swapped (
    music,
    "notify",
    (GCallback) update_stats,
    self
  );

}

static void
geolocation_service_ready_cb (GObject          *const obj,
                              GAsyncResult     *const res,
                              NulUiApplication *const self)
{
  g_message ("geolocation service ready");
  self->geolocation = nul_geolocation_service_proxy_new_finish (res, NULL);
}

static void
service_appeared_cb (GDBusConnection  *const conn,
                     const gchar      *const name,
                     const gchar      *const name_owner,
                     NulUiApplication *const app)
{

  g_message ("service appeared");

  gtk_stack_set_visible_child (
    app->front_page,
    GTK_WIDGET (app->stats_screen)
  );

  nul_music_service_proxy_new (
    conn,
    G_DBUS_PROXY_FLAGS_NONE,
    NUL_SERVICE_APP_ID,
    NUL_SERVICE_OBJECT_PATH,
    NULL,
    (GAsyncReadyCallback) music_service_ready_cb,
    app
  );

  nul_geolocation_service_proxy_new (
    conn,
    G_DBUS_PROXY_FLAGS_NONE,
    NUL_SERVICE_APP_ID,
    NUL_SERVICE_OBJECT_PATH,
    NULL,
    (GAsyncReadyCallback) geolocation_service_ready_cb,
    app
  );

}

static void
service_vanished_cb (GDBusConnection  *const conn,
                     const gchar      *const name,
                     NulUiApplication *const app)
{

  g_message ("service vanished");

  gtk_stack_set_visible_child (
    app->front_page,
    GTK_WIDGET (app->loading_screen)
  );

  if (app->music)
    g_object_unref (app->music);

  if (app->geolocation)
    g_object_unref (app->geolocation);

}

static gboolean
dbus_register (GApplication    *const app,
               GDBusConnection *const conn,
               gchar const     *const path,
               GError         **const err)
{

  NulUiApplication *const self = NUL_UI_APPLICATION (app);

  self->bus_watcher = g_bus_watch_name_on_connection (
    conn,
    NUL_SERVICE_APP_ID,
    G_BUS_NAME_WATCHER_FLAGS_NONE,
    (GBusNameAppearedCallback) service_appeared_cb,
    (GBusNameVanishedCallback) service_vanished_cb,
    app,
    NULL
  );

  return G_APPLICATION_CLASS (nul_ui_application_parent_class)->
    dbus_register (app, conn, path, err);

}

static void
dbus_unregister (GApplication    *const app,
                 GDBusConnection *const conn,
                 gchar const     *const path)
{

  NulUiApplication *const self = NUL_UI_APPLICATION (app);

  if (self->bus_watcher > 0)
    g_bus_unwatch_name (self->bus_watcher);

  G_APPLICATION_CLASS (nul_ui_application_parent_class)->
    dbus_unregister (app, conn, path);

}

static void
nul_ui_application_finalize (GObject *const object)
{
  G_OBJECT_GET_CLASS (object)->finalize (object);
}

static void
nul_ui_application_class_init (NulUiApplicationClass *const cls)
{

  GObjectClass *const object_class = G_OBJECT_CLASS (cls);
  GApplicationClass *const app_class = G_APPLICATION_CLASS (cls);

  app_class->dbus_register = dbus_register;
  app_class->dbus_unregister = dbus_unregister;
  app_class->activate = activate;

  object_class->finalize = nul_ui_application_finalize;

}

static void
nul_ui_application_init (NulUiApplication *const self)
{
  self->bus_watcher = 0;
}
