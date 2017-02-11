#ifndef NUL_SPARQL_QUERY_UTIL_H
#define NUL_SPARQL_QUERY_UTIL_H

#include <glib.h>
#include <gio/gio.h>
#include <tracker-sparql.h>

G_BEGIN_DECLS

void
nul_sparql_query_util_query_async     (gchar const         *const sparql,
                                       GCancellable        *const cancellable,
                                       GAsyncReadyCallback  const callback,
                                       gpointer             const user_data);

TrackerSparqlCursor *
nul_sparql_query_util_query_finish    (GAsyncResult        *const result,
                                       GError             **const error);

void
nul_sparql_query_util_load_int_async  (gchar const         *const sparql,
                                       GCancellable        *const cancellable,
                                       GAsyncReadyCallback  const callback,
                                       gpointer             const user_data);

gssize
nul_sparql_query_util_load_int_finish (GAsyncResult        *const result,
                                       GError             **const error);
G_END_DECLS

#endif /* NUL_SPARQL_QUERY_UTIL_H */
