#include "nul-ui-window.h"

struct _NulUiWindow
{
  GtkWindow parent_instance;
};

G_DEFINE_TYPE (NulUiWindow, nul_ui_window, GTK_TYPE_WINDOW)

NulUiWindow *
nul_ui_window_new (void)
{
  return g_object_new (NUL_TYPE_UI_WINDOW, NULL);
}

static void
nul_ui_window_finalize (GObject *object)
{
  G_OBJECT_CLASS (nul_ui_window_parent_class)->finalize (object);
}

static void
nul_ui_window_class_init (NulUiWindowClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = nul_ui_window_finalize;
}

static void
nul_ui_window_init (NulUiWindow *self)
{
}
