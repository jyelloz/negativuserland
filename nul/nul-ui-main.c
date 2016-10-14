#include <glib.h>
#include <gtk/gtk.h>

#include "nul-ui-application.h"

gint
main (gint const argc, gchar **const argv)
{

  GApplication *const app = G_APPLICATION (nul_ui_application_new ());

  g_application_set_default (app);

  return g_application_run (app, argc, argv);

}
