#include <locale.h>
#include <signal.h>

#include <glib.h>
#include <gio/gio.h>

#include "nul-tui-stfl-application.h"

static gboolean
input_cb (GIOChannel   *const chan,
          GIOCondition  const cond,
          GApplication *const app)
{

  nul_tui_stfl_application_input (NUL_TUI_STFL_APPLICATION (app));

  return G_SOURCE_CONTINUE;

}

static gboolean
winch_cb (void)
{

  nul_tui_stfl_application_resize (
    NUL_TUI_STFL_APPLICATION (g_application_get_default ())
  );

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

  g_autoptr(GApplication) app = nul_tui_stfl_application_new ();
  GIOChannel *const stdin_channel = g_io_channel_unix_new (STDIN_FILENO);

  g_io_add_watch (stdin_channel, G_IO_IN, (GIOFunc) input_cb, app);

  g_application_set_default (app);
  g_application_hold (app);

  {
    struct sigaction sa = {
      .sa_handler = winch_handler,
    };
    sigemptyset (&sa.sa_mask);
    sigaction (SIGWINCH, &sa, NULL);
  }

  return g_application_run (app, argc, argv);

}
