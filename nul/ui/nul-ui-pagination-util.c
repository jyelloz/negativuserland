#include "nul-ui-pagination-util.h"

#include <glib.h>

struct _NulUiPager {
  gsize offset;
  gsize limit;
  gsize page_number;
  gsize page_count;
  gsize total;
};

gsize
nul_ui_pager_get_page_count (NulUiPager const *const self)
{
  gsize const limit = self->limit;
  gsize const total = self->total;
  g_return_val_if_fail (limit > 0, 0);
  return 1 + (total / limit);
}

gsize
nul_ui_pager_get_current_page (NulUiPager const *const self)
{
  gsize const offset = self->offset;
  gsize const limit = self->limit;
  g_return_val_if_fail (limit > 0, 0);
  return offset / limit;
}
