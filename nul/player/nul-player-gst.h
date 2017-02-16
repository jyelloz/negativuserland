#ifndef NUL_PLAYER_GST_H
#define NUL_PLAYER_GST_H

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define NUL_PLAYER_TYPE_GST (nul_player_gst_get_type ())

G_DECLARE_FINAL_TYPE (
  NulPlayerGst,
  nul_player_gst,
  NUL_PLAYER,
  GST,
  GObject
)

NulPlayerGst *
nul_player_gst_new (GError **const error);

G_END_DECLS

#endif /* NUL_PLAYER_GST_H */
