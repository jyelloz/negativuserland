#include "nul-player-gst.h"

#include <glib.h>
#include <glib-object.h>

#include <gio/gio.h>
#include <gst/gst.h>

#define gobj_class G_OBJECT_CLASS (nul_player_gst_parent_class)

static gchar const *pipeline_launcher[] = {
  "urisourcebin",
  "decodebin",
  "alsasink",
  NULL
};

struct _NulPlayerGst
{
  GObject parent_instance;
  GstPipeline *pipeline;
};

static void
initable_iface_init (GInitableIface *const iface);

G_DEFINE_TYPE_WITH_CODE (
  NulPlayerGst,
  nul_player_gst,
  G_TYPE_OBJECT,
  G_IMPLEMENT_INTERFACE (G_TYPE_INITABLE, initable_iface_init)
);

NulPlayerGst *
nul_player_gst_new (GError **const error)
{
  return g_initable_new (
    NUL_PLAYER_TYPE_GST,
    NULL,
    error,
    NULL
  );
}

static GstPipeline *
build_pipeline (GError **const error)
{

  GstElement *const element = gst_parse_launchv (pipeline_launcher, error);

  if (*error) {
    if (G_IS_OBJECT (element)) {
      g_object_unref (element);
    }
    return NULL;
  }

  return GST_PIPELINE (element);

}


static gboolean
initable_init (GInitable     *const initable,
               GCancellable  *const cancellable,
               GError       **const error)
{

  g_debug ("initable_init media player");
  NulPlayerGst *const self = NUL_PLAYER_GST (initable);
  g_autoptr(GError) inner_error = NULL;

  self->pipeline = build_pipeline (&inner_error);

  if (inner_error) {
    if (error) {
      *error = g_error_copy (inner_error);
    }
    return FALSE;
  }

  return TRUE;

}

static void
nul_player_gst_init (NulPlayerGst *const self)
{
  g_debug ("init media player");
}

static void
nul_player_gst_finalize (GObject *const object)
{

  NulPlayerGst *const self = NUL_PLAYER_GST (object);

  g_clear_object (&self->pipeline);

  gobj_class->finalize (object);

}

static void
nul_player_gst_class_init (NulPlayerGstClass *const cls)
{

  GObjectClass *const object_class = G_OBJECT_CLASS (cls);

  object_class->finalize = nul_player_gst_finalize;

}

static void
initable_iface_init (GInitableIface *const iface)
{
  iface->init = initable_init;
}
