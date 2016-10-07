#ifndef __NUL_MUSIC_SERVICE_UTIL_H__
#define __NUL_MUSIC_SERVICE_UTIL_H__

#include <glib.h>
#include <gio/gio.h>

G_BEGIN_DECLS

GDBusInterfaceSkeleton *
nul_music_service_util_get_skeleton (void);

G_END_DECLS

#endif /* __NUL_MUSIC_SERVICE_UTIL_H__ */
