#ifndef NUL_EXTERNAL_AUTOCLEANUPS_H
#define NUL_EXTERNAL_AUTOCLEANUPS_H

#include <glib.h>
#include <glib-object.h>

#ifdef __GEE_H__
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GeeList, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GeeCollection, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (GeeMultiMap, g_object_unref)
#endif

#ifdef __LIBTRACKER_SPARQL_H__
G_DEFINE_AUTOPTR_CLEANUP_FUNC (TrackerSparqlConnection, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (TrackerSparqlBuilder, g_object_unref)
G_DEFINE_AUTOPTR_CLEANUP_FUNC (TrackerSparqlCursor, g_object_unref)
#endif

#endif /* NUL_EXTERNAL_AUTOCLEANUPS_H */
