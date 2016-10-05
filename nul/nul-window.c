/* nul-window.c
 *
 * Copyright (C) 2016 Jordan Yelloz <jordan@yelloz.me>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "nul-window.h"

struct _NulWindow
{
  GObject parent_instance;
};

G_DEFINE_TYPE (NulWindow, nul_window, GTK_TYPE_WINDOW)

enum {
  PROP_0,
  N_PROPS
};

/* static GParamSpec *properties [N_PROPS]; */

NulWindow *
nul_window_new (void)
{
  return g_object_new (NUL_TYPE_WINDOW, NULL);
}

static void
nul_window_finalize (GObject *object)
{
  G_OBJECT_CLASS (nul_window_parent_class)->finalize (object);
}

static void
nul_window_get_property (GObject    *object,
                         guint       prop_id,
                         GValue     *value,
                         GParamSpec *pspec)
{
  switch (prop_id)
    {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
nul_window_set_property (GObject      *object,
                         guint         prop_id,
                         const GValue *value,
                         GParamSpec   *pspec)
{
  switch (prop_id)
    {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
nul_window_class_init (NulWindowClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = nul_window_finalize;
  object_class->get_property = nul_window_get_property;
  object_class->set_property = nul_window_set_property;
}

static void
nul_window_init (NulWindow *self)
{
}

