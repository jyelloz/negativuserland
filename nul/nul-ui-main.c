#include <locale.h>

#include <glib.h>
#include <gio/gio.h>

#include "nul-ui-application.h"

gint
main (gint const argc, gchar **const argv)
{

  setlocale (LC_ALL, "");

  g_set_application_name (PACKAGE_NAME);
  g_set_prgname (PACKAGE);

  g_autoptr(GApplication) app = G_APPLICATION (nul_ui_application_new ());

  g_application_set_default (app);

  return g_application_run (app, argc, argv);

}
