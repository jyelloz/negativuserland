#include "nul-ui-application.h"

#if !GLIB_CHECK_VERSION (2, 48, 0)
#define G_APPLICATION_CAN_OVERRIDE_APP_ID 0
#endif

#define NUL_APP_ID "org.negativuserland.Ui"
#define NUL_APP_FLAGS G_APPLICATION_CAN_OVERRIDE_APP_ID

struct _NulUiApplication
{
  GtkApplication parent_instance;
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
nul_ui_application_finalize (GObject *const object)
{
  G_OBJECT_GET_CLASS (object)->finalize (object);
}

static void
nul_ui_application_class_init (NulUiApplicationClass *const cls)
{

  GObjectClass *const object_class = G_OBJECT_CLASS (cls);

  object_class->finalize = nul_ui_application_finalize;

}

static void
nul_ui_application_init (NulUiApplication *const self)
{
}
