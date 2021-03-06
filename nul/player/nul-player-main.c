#include <locale.h>

#include <glib.h>
#include <gio/gio.h>

#include "nul-player-application.h"

gint
main (gint const argc, gchar **const argv)
{

  setlocale (LC_ALL, "");

  g_set_prgname ("nul-player");

  g_autoptr(GApplication) app = nul_player_application_new ();

  g_application_set_default (app);
  g_application_hold (app);

  return g_application_run (app, argc, argv);

}
