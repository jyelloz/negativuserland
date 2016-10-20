#include "nul-ui-main-menu.h"

#include <gio/gio.h>
#include <gtk/gtk.h>

struct _NulUiMainMenu {

  GtkListBox *main_menu;

  GtkWidget *music_stats;
  GtkWidget *geolocation;
  GtkWidget *automotive;
  GtkWidget *settings;

  GtkStack *connected_state;

};

static void
activate_main_menu (GSimpleAction *const action,
                    GVariant      *const parameter,
                    gpointer       const user_data)
{
  NulUiMainMenu *const self = (NulUiMainMenu *) user_data;
  gtk_stack_set_visible_child (
    self->connected_state,
    GTK_WIDGET (self->main_menu)
  );
}

static void
activate_music (GSimpleAction *const action,
                GVariant      *const parameter,
                gpointer       const user_data)
{
  NulUiMainMenu *const self = (NulUiMainMenu *) user_data;
  gtk_stack_set_visible_child (
    self->connected_state,
    self->music_stats
  );
}

static void
activate_geolocation (GSimpleAction *const action,
                      GVariant      *const parameter,
                      gpointer       const user_data)
{
  NulUiMainMenu *const self = (NulUiMainMenu *) user_data;
  gtk_stack_set_visible_child (
    self->connected_state,
    self->geolocation
  );
}

static void
activate_automotive (GSimpleAction *const action,
                     GVariant      *const parameter,
                     gpointer       const user_data)
{
  NulUiMainMenu *const self = (NulUiMainMenu *) user_data;
  gtk_stack_set_visible_child (
    self->connected_state,
    self->automotive
  );
}

static void
activate_settings (GSimpleAction *const action,
                   GVariant      *const parameter,
                   gpointer       const user_data)
{
  NulUiMainMenu *const self = (NulUiMainMenu *) user_data;
  gtk_stack_set_visible_child (
    self->connected_state,
    self->settings
  );
}

static GActionEntry const entries[] = {
  {"main-menu", activate_main_menu, NULL, NULL, NULL},
  {"music", activate_music, NULL, NULL, NULL},
  {"geolocation", activate_geolocation, NULL, NULL, NULL},
  {"automotive", activate_automotive, NULL, NULL, NULL},
  {"settings", activate_settings, NULL, NULL, NULL},
};

NulUiMainMenu *
nul_ui_main_menu_new (GtkListBox *const main_menu,
                      GtkWidget  *const music_stats,
                      GtkWidget  *const geolocation,
                      GtkWidget  *const automotive,
                      GtkWidget  *const settings,
                      GtkStack   *const connected_state)
{

  NulUiMainMenu *const self = g_new0 (NulUiMainMenu, 1);

  self->connected_state = g_object_ref (connected_state);
  self->music_stats = g_object_ref (music_stats);
  self->geolocation = g_object_ref (geolocation);
  self->automotive = g_object_ref (automotive);
  self->settings = g_object_ref (settings);
  self->main_menu = g_object_ref (main_menu);

  return self;

}

static void
row_activated_cb (GtkListBox    *const box,
                  GtkListBoxRow *const row,
                  GActionGroup  *const group)
{

  GtkWidget *const child = gtk_bin_get_child (GTK_BIN (row));
  gchar const *const name = gtk_widget_get_name (child);

  g_action_group_activate_action (group, name, NULL);

}

void
nul_ui_main_menu_free (NulUiMainMenu *const self)
{
  g_object_unref (self->connected_state);
  g_object_unref (self->music_stats);
  g_object_unref (self->main_menu);
  g_free (self);
}

void
nul_ui_main_menu_register_actions (NulUiMainMenu *const self,
                                   GActionMap    *const map,
                                   GActionGroup  *const group)
{

  g_action_map_add_action_entries (
    map,
    entries,
    G_N_ELEMENTS (entries),
    self
  );

  g_signal_connect (
    self->main_menu,
    "row-activated",
    G_CALLBACK (row_activated_cb),
    group
  );

}
