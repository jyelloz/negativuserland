#ifndef __NUL_GEOLOCATION_SERVICE_UTIL_H__
#define __NUL_GEOLOCATION_SERVICE_UTIL_H__

#include <glib.h>
#include <gio/gio.h>

G_BEGIN_DECLS

GDBusInterfaceSkeleton *
nul_geolocation_service_util_get_skeleton (void);

G_END_DECLS

#endif /* __NUL_GEOLOCATION_SERVICE_UTIL_H__ */
