#include "nul-service-application.h"

struct _NulServiceApplication
{
  GApplication parent_instance;
};

G_DEFINE_TYPE (NulServiceApplication, nul_service_application, G_TYPE_APPLICATION)

enum {
  PROP_0,
  N_PROPS
};

static GParamSpec *properties [N_PROPS];

NulServiceApplication *
nul_service_application_new (void)
{
  return g_object_new (NUL_TYPE_SERVICE_APPLICATION, NULL);
}

static void
nul_service_application_finalize (GObject *object)
{
  NulServiceApplication *self = (NulServiceApplication *)object;

  G_OBJECT_CLASS (nul_service_application_parent_class)->finalize (object);
}

static void
nul_service_application_get_property (GObject    *object,
                                      guint       prop_id,
                                      GValue     *value,
                                      GParamSpec *pspec)
{
  NulServiceApplication *self = NUL_SERVICE_APPLICATION (object);

  switch (prop_id)
    {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
nul_service_application_set_property (GObject      *object,
                                      guint         prop_id,
                                      const GValue *value,
                                      GParamSpec   *pspec)
{
  NulServiceApplication *self = NUL_SERVICE_APPLICATION (object);

  switch (prop_id)
    {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
nul_service_application_class_init (NulServiceApplicationClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = nul_service_application_finalize;
  object_class->get_property = nul_service_application_get_property;
  object_class->set_property = nul_service_application_set_property;
}

static void
nul_service_application_init (NulServiceApplication *self)
{
}
