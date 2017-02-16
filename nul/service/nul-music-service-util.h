#ifndef NUL_MUSIC_SERVICE_UTIL_H
#define NUL_MUSIC_SERVICE_UTIL_H

#include <glib.h>
#include <gio/gio.h>

G_BEGIN_DECLS

GDBusInterfaceSkeleton *
nul_music_service_util_get_skeleton (void);

G_END_DECLS

#endif /* NUL_MUSIC_SERVICE_UTIL_H */
