#include "nul-sparql-query-util.h"
#include "nul-external-autocleanups.h"

#include <glib.h>
#include <gio/gio.h>

#include <tracker-sparql.h>

typedef struct _SparqlQueryWork {
  gchar *sparql;
  TrackerSparqlConnection *connection;
  TrackerSparqlCursor *cursor;
} SparqlQueryWork;

static inline SparqlQueryWork *
sparql_query_work_get (GTask *const task)
{
  return (SparqlQueryWork *) g_task_get_task_data (task);
}

static void
sparql_query_work_free (SparqlQueryWork *const work)
{

  g_debug ("freeing work at %p", work);

  if (work == NULL) {
    return;
  }

  g_object_unref (work->connection);
  g_object_unref (work->cursor);
  g_free (work->sparql);
  g_free (work);

}

static void
do_sparql_query_async_cursor_cb (GObject      *const object,
                                 GAsyncResult *const result,
                                 GTask        *const task)
{

  g_autoptr(GError) error = NULL;

  SparqlQueryWork *const work = sparql_query_work_get (task);
  TrackerSparqlConnection *const conn = work->connection;

  TrackerSparqlCursor *const cur = tracker_sparql_connection_query_finish (
    conn,
    result,
    &error
  );

  if (error) {
    g_task_return_error (task, g_error_copy (error));
    return;
  }

  g_task_return_pointer (task, cur, g_object_unref);

}

static void
do_sparql_query_async_connection_cb (GObject      *const object,
                                     GAsyncResult *const result,
                                     GTask        *const task)
{

  g_autoptr(GError) error = NULL;

  TrackerSparqlConnection *const conn = tracker_sparql_connection_get_finish (
    result,
    &error
  );

  if (error) {
    g_task_return_error (task, g_error_copy (error));
    return;
  }

  SparqlQueryWork *const work = sparql_query_work_get (task);
  work->connection = conn;

  tracker_sparql_connection_query_async (
    conn,
    work->sparql,
    g_task_get_cancellable (task),
    (GAsyncReadyCallback) do_sparql_query_async_cursor_cb,
    task
  );

}

void
nul_sparql_query_util_query_async (gchar const         *const sparql,
                                   GCancellable        *const cancellable,
                                   GAsyncReadyCallback  const callback,
                                   gpointer             const user_data)
{

  g_debug ("scheduling query %s", sparql);

  GTask *const task = g_task_new (NULL, cancellable, callback, user_data);

  SparqlQueryWork *const work = g_new0 (SparqlQueryWork, 1);
  work->sparql = g_strdup (sparql);

  g_task_set_task_data (task, work, (GDestroyNotify) sparql_query_work_free);

  tracker_sparql_connection_get_async (
    cancellable,
    (GAsyncReadyCallback) do_sparql_query_async_connection_cb,
    task
  );

}

TrackerSparqlCursor *
nul_sparql_query_util_query_finish (GAsyncResult  *const result,
                                    GError       **const error)
{

  GTask *const task = G_TASK (result);

  g_return_val_if_fail (g_task_is_valid (task, NULL), NULL);

  return g_task_propagate_pointer (task, error);

}

static void
load_integer_value_next_cb (TrackerSparqlCursor *const cursor,
                            GAsyncResult        *const result,
                            GTask               *const task)
{

  g_autoptr(GError) error = NULL;

  gboolean const more = tracker_sparql_cursor_next_finish (
    cursor,
    result,
    &error
  );

  if (!more) {
    g_warning ("no more rows to load");
    g_task_return_int (task, -1);
    return;
  }

  gint64 const integer = tracker_sparql_cursor_get_integer (cursor, 0);

  g_task_return_int (task, integer);

}

static void
load_integer_value_query_cb (GObject      *const object,
                             GAsyncResult *const result,
                             GTask        *const task)
{

  g_autoptr(GError) error = NULL;

  g_autoptr(TrackerSparqlCursor) cursor = nul_sparql_query_util_query_finish (
    result,
    &error
  );

  tracker_sparql_cursor_next_async (
    cursor,
    g_task_get_cancellable (task),
    (GAsyncReadyCallback) load_integer_value_next_cb,
    task
  );

}

void
nul_sparql_query_util_load_int_async (gchar const         *const sparql,
                                      GCancellable        *const cancellable,
                                      GAsyncReadyCallback  const callback,
                                      gpointer             const user_data)
{

  GTask *const task = g_task_new (NULL, cancellable, callback, user_data);

  nul_sparql_query_util_query_async (
    sparql,
    cancellable,
    (GAsyncReadyCallback) load_integer_value_query_cb,
    task
  );

}

gssize
nul_sparql_query_util_load_int_finish (GAsyncResult  *const result,
                                       GError       **const error)
{

  g_autoptr(GTask) task = G_TASK (result);

  return g_task_propagate_int (task, error);

}
