#include "nul-geolocation-service-util.h"

#include "nul-geolocation-service.h"

GDBusInterfaceSkeleton *
nul_geolocation_service_util_get_skeleton (void)
{

  NulGeolocationService *const geo = nul_geolocation_service_skeleton_new ();


  return G_DBUS_INTERFACE_SKELETON (geo);

}
