#ifndef NUL_EXTERNAL_AUTOCLEANUPS_H
#define NUL_EXTERNAL_AUTOCLEANUPS_H

#include <glib.h>
#include <tracker-sparql.h>
#include <gee.h>

G_DEFINE_AUTOPTR_CLEANUP_FUNC (GeeList, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GeeCollection, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GeeMultiMap, g_object_unref)

G_DEFINE_AUTOPTR_CLEANUP_FUNC (TrackerSparqlConnection, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (TrackerSparqlBuilder, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (TrackerSparqlCursor, g_object_unref)

#endif /* NUL_EXTERNAL_AUTOCLEANUPS_H */
