#ifndef NUL_PLAYER_APPLICATION_H
#define NUL_PLAYER_APPLICATION_H

#include <glib-object.h>
#include <gio/gio.h>

G_BEGIN_DECLS

#define NUL_TYPE_PLAYER_APPLICATION (nul_player_application_get_type ())

G_DECLARE_FINAL_TYPE (
  NulPlayerApplication,
  nul_player_application,
  NUL,
  PLAYER_APPLICATION,
  GApplication
)

GApplication *
nul_player_application_new (void);

G_END_DECLS

#endif /* NUL_PLAYER_APPLICATION_H */
