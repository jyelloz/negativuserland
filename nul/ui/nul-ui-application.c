#include "nul-ui-application.h"
#include "nul-music-service.h"
#include "nul-geolocation-service.h"
#include "nul-ui-service-state-stack.h"
#include "nul-ui-main-menu.h"
#include "nul-ui-artists.h"
#include "nul-ui-resources.h"

#if !GLIB_CHECK_VERSION (2, 48, 0)
#define G_APPLICATION_CAN_OVERRIDE_APP_ID 0
#endif

#define NUL_APP_ID "org.negativuserland.Ui"
#define NUL_APP_FLAGS G_APPLICATION_CAN_OVERRIDE_APP_ID

#define NUL_SERVICE_APP_ID "org.negativuserland.Service"
#define NUL_SERVICE_OBJECT_PATH "/org/negativuserland/Service"

#define gapp_class G_APPLICATION_CLASS (nul_ui_application_parent_class)
#define gobj_class G_OBJECT_CLASS (nul_ui_application_parent_class)

struct _NulUiApplication
{
  GtkApplication parent_instance;

  guint bus_watcher;

  NulMusicService *music;
  NulGeolocationService *geolocation;

  NulUiServiceStateStack *service_state_stack;
  NulUiMainMenu *main_menu;
  NulUiArtists *artists;

  GtkWidget *music_screen;
  GtkWidget *music_screen_stack;

  GtkLabel *artists_count_label;
  GtkLabel *albums_count_label;
  GtkLabel *tracks_count_label;

  GtkBuilder *builder;

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
    NUL_UI_TYPE_APPLICATION,
    "application-id", NUL_APP_ID,
    "flags", NUL_APP_FLAGS,
    NULL
  );
}

GtkBuilder *
nul_ui_application_get_builder (NulUiApplication *const self)
{
  g_return_val_if_fail (NUL_UI_IS_APPLICATION (self), NULL);
  return self->builder;
}

GtkBuilder *
nul_ui_application_get_builder_default (void)
{
  NulUiApplication *const app = NUL_UI_APPLICATION (
    g_application_get_default ()
  );
  return app == NULL ? NULL : nul_ui_application_get_builder (app);
}

static void
artists_ready_cb (NulMusicService  *const proxy,
                  GAsyncResult     *const result,
                  NulUiApplication *const self)
{

  g_autoptr(GVariant) artists;
  GtkStack *const stack = GTK_STACK (self->music_screen_stack);

  nul_music_service_call_get_artists_finish (proxy, &artists, result, NULL);

  nul_ui_artists_update (self->artists, artists);

  gtk_stack_set_visible_child_name (
    stack,
    "artists-list-box"
  );

}

static gboolean
activate_artists_cb (GtkLabel         *const link,
                     gchar const      *const url,
                     NulUiApplication *const app)
{

  nul_music_service_call_get_artists (
    app->music,
    0,
    10,
    NULL,
    (GAsyncReadyCallback) artists_ready_cb,
    app
  );

  return TRUE;

}

static void
activate (GApplication *const app)
{

#define B_OBJ(name) gtk_builder_get_object ((builder), (name))

  nul_debug ("ui activated");

  GtkApplication *const gtk_app = GTK_APPLICATION (app);
  NulUiApplication *const self = NUL_UI_APPLICATION (app);

  GtkBuilder *const builder = gtk_builder_new_from_resource (
    "/org/negativuserland/Ui/negativuserland.ui"
  );

  self->builder = builder;

  GObject *const service_state_stack = B_OBJ ("service-state-stack");
  GObject *const connected_state = B_OBJ ("connected-state");
  GObject *const main_menu = B_OBJ ("main-menu");

  GObject *const artists_button = B_OBJ ("artists-field-label");
  GObject *const geolocation = B_OBJ ("geolocation-screen");
  GObject *const automotive = B_OBJ ("automotive-screen");
  GObject *const settings = B_OBJ ("settings-screen");

  GObject *const music_screen = B_OBJ ("music-screen");
  GObject *const music_screen_stack = B_OBJ ("music-screen-stack");
  GObject *const window = B_OBJ ("main-window");

  self->service_state_stack = nul_ui_service_state_stack_new (
   GTK_STACK (service_state_stack)
  );

  self->main_menu = nul_ui_main_menu_new (
    GTK_LIST_BOX (main_menu),
    GTK_WIDGET (music_screen),
    GTK_WIDGET (geolocation),
    GTK_WIDGET (automotive),
    GTK_WIDGET (settings),
    GTK_STACK (connected_state)
  );

  self->artists = nul_ui_artists_new (
    GTK_BOX (B_OBJ ("artists-list-box")),
    GTK_TREE_VIEW (B_OBJ ("artists-list")),
    GTK_LIST_STORE (B_OBJ ("artists-liststore")),
    GTK_LABEL (B_OBJ ("artists-page-status-label")),
    GTK_BUTTON (B_OBJ ("artists-page-prev-button")),
    GTK_BUTTON (B_OBJ ("artists-page-next-button"))
  );

  self->artists_count_label = GTK_LABEL (B_OBJ ("artists-count-label"));
  self->albums_count_label = GTK_LABEL (B_OBJ ("albums-count-label"));
  self->tracks_count_label = GTK_LABEL (B_OBJ ("tracks-count-label"));

  nul_ui_main_menu_register_actions (
    self->main_menu,
    G_ACTION_MAP (window),
    G_ACTION_GROUP (window)
  );

  nul_ui_artists_register_actions (
    self->artists,
    G_ACTION_MAP (window),
    G_ACTION_GROUP (window)
  );

  self->music_screen = GTK_WIDGET (music_screen);
  self->music_screen_stack = GTK_WIDGET (music_screen_stack);

  g_signal_connect (
    GTK_LABEL (artists_button),
    "activate-link",
    G_CALLBACK (activate_artists_cb),
    self
  );

  gtk_window_set_application (GTK_WINDOW (window), gtk_app);
  gtk_widget_show_all (GTK_WIDGET (window));

#undef B_OBJ

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
  nul_debug ("music service ready");

  g_autoptr(GError) error = NULL;

  NulMusicService *const music = self->music =
    nul_music_service_proxy_new_finish (res, &error);

  if (error) {
    nul_error ("failed to get music dbus proxy: %s", error->message);
  }

  update_stats_view (self);

  g_signal_connect_swapped (
    music,
    "notify",
    (GCallback) update_stats,
    self
  );

  nul_ui_artists_register (self->artists, music);

}

static void
geolocation_service_ready_cb (GObject          *const obj,
                              GAsyncResult     *const res,
                              NulUiApplication *const self)
{
  nul_debug ("geolocation service ready");
  self->geolocation = nul_geolocation_service_proxy_new_finish (res, NULL);
}

static void
service_appeared_cb (GDBusConnection  *const conn,
                     const gchar      *const name,
                     const gchar      *const name_owner,
                     NulUiApplication *const app)
{

  nul_debug ("service appeared");

  nul_ui_service_state_stack_connect (app->service_state_stack);

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

  nul_debug ("service vanished");

  nul_ui_service_state_stack_disconnect (app->service_state_stack);
  nul_ui_artists_unregister (app->artists);

  gtk_label_set_text (app->artists_count_label, "-");
  gtk_label_set_text (app->albums_count_label, "-");
  gtk_label_set_text (app->tracks_count_label, "-");

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

  return gapp_class->dbus_register (app, conn, path, err);

}

static void
dbus_unregister (GApplication    *const app,
                 GDBusConnection *const conn,
                 gchar const     *const path)
{

  NulUiApplication *const self = NUL_UI_APPLICATION (app);

  if (self->bus_watcher > 0)
    g_bus_unwatch_name (self->bus_watcher);

  gapp_class->dbus_unregister (app, conn, path);

}

static void
activate_quit (GSimpleAction *const action,
               GVariant      *const parameter,
               gpointer       const app)
{
  g_application_quit (G_APPLICATION (app));
}

static GActionEntry const app_entries[] = {
  {"quit", activate_quit, NULL, NULL, NULL},
};

static void
nul_ui_application_finalize (GObject *const object)
{

  NulUiApplication *const self = NUL_UI_APPLICATION (object);

  g_clear_pointer (&self->service_state_stack, nul_ui_service_state_stack_free);
  g_clear_pointer (&self->main_menu, nul_ui_main_menu_free);
  g_clear_object (&self->artists);

  g_clear_object (&self->builder);

  gobj_class->finalize (object);

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

  g_action_map_add_action_entries (
    G_ACTION_MAP (self),
    app_entries,
    G_N_ELEMENTS (app_entries),
    self
  );

}
