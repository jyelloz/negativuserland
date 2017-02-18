#include "nul-player-gst.h"

#include <glib.h>
#include <glib-object.h>

#include <gio/gio.h>
#include <gst/gst.h>

#define gobj_class G_OBJECT_CLASS (nul_player_gst_parent_class)

static gchar const *pipeline_launcher[] = {
  "playbin",
  NULL
};

struct _NulPlayerGst
{
  GObject parent_instance;
  GstPipeline *pipeline;
  gchar const **track;
};

static void
initable_iface_init (GInitableIface *const iface);

static void
about_to_finish_cb (GstElement *const playbin, NulPlayerGst *const self);

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

void
nul_player_gst_play (NulPlayerGst *const self)
{

  g_return_if_fail (self && NUL_PLAYER_IS_GST (self));

  GstPipeline *const pl = self->pipeline;

  gchar const *uri = *self->track;

  g_object_set (
    G_OBJECT (pl),
    "uri", uri,
    NULL
  );

  gst_element_set_state (GST_ELEMENT (pl), GST_STATE_PLAYING);

}

void
nul_player_gst_pause (NulPlayerGst *const self)
{

  g_return_if_fail (self && NUL_PLAYER_IS_GST (self));

  GstPipeline *const pl = self->pipeline;

  gst_element_set_state (GST_ELEMENT (pl), GST_STATE_PAUSED);

}

void
nul_player_gst_stop (NulPlayerGst *const self)
{

  g_return_if_fail (self && NUL_PLAYER_IS_GST (self));

  GstPipeline *const pl = self->pipeline;

  gst_element_set_state (GST_ELEMENT (pl), GST_STATE_NULL);

}

static void
about_to_finish_cb (GstElement   *const playbin,
                    NulPlayerGst *const self)
{

  self->track++;

  gchar const *const uri = *self->track;

  if (uri == NULL) {
    return;
  }

  nul_debug ("new uri=%s", uri);

  g_object_set (
    playbin,
    "uri", uri,
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

  NulPlayerGst *const self = NUL_PLAYER_GST (initable);
  g_autoptr(GError) inner_error = NULL;

  GstPipeline *const pl = self->pipeline = build_pipeline (&inner_error);

  if (inner_error) {
    if (error) {
      *error = g_error_copy (inner_error);
    }
    return FALSE;
  }

  g_signal_connect (
    pl,
    "about-to-finish",
    G_CALLBACK (about_to_finish_cb),
    self
  );

  return TRUE;

}

static void
nul_player_gst_init (NulPlayerGst *const self)
{
  nul_debug ("init media player");
}

static void
nul_player_gst_finalize (GObject *const object)
{

  NulPlayerGst *const self = NUL_PLAYER_GST (object);

  if (GST_IS_ELEMENT (self->pipeline)) {
    gst_element_set_state (GST_ELEMENT (self->pipeline), GST_STATE_NULL);
  }

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
