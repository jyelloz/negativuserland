#ifndef NUL_SERVICE_APPLICATION_H
#define NUL_SERVICE_APPLICATION_H

#include <glib-object.h>
#include <gio/gio.h>

G_BEGIN_DECLS

#define NUL_TYPE_SERVICE_APPLICATION (nul_service_application_get_type ())

G_DECLARE_FINAL_TYPE (
  NulServiceApplication,
  nul_service_application,
  NUL,
  SERVICE_APPLICATION,
  GApplication
)

GApplication *
nul_service_application_new (void);

G_END_DECLS

#endif /* NUL_SERVICE_APPLICATION_H */
