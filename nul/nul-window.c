#include "nul-window.h"

struct _NulWindow
{
  GtkWindow parent_instance;
};

G_DEFINE_TYPE (NulWindow, nul_window, GTK_TYPE_WINDOW)

NulWindow *
nul_window_new (void)
{
  return g_object_new (NUL_TYPE_WINDOW, NULL);
}

static void
nul_window_finalize (GObject *object)
{
  G_OBJECT_CLASS (nul_window_parent_class)->finalize (object);
}

static void
nul_window_class_init (NulWindowClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = nul_window_finalize;
}

static void
nul_window_init (NulWindow *self)
{
}
