#include "nul-ui-artists.h"

#include <gtk/gtk.h>

struct _NulUiArtists {
  GtkBox *box;
  GtkTreeView *tree;
};

NulUiArtists *
nul_ui_artists_new (GtkBox      *const box,
                    GtkTreeView *const tree)
{

  NulUiArtists *const self = g_new0 (NulUiArtists, 1);

  self->box = g_object_ref (box);
  self->tree = g_object_ref (tree);

  return self;

}

void
nul_ui_artists_free (NulUiArtists *const self)
{

  g_object_unref (self->box);
  g_object_unref (self->tree);

  g_free (self);

}
