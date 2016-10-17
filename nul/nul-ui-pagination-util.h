#ifndef NUL_UI_PAGINATION_UTIL_H
#define NUL_UI_PAGINATION_UTIL_H

#include <glib.h>

G_BEGIN_DECLS

typedef struct _NulUiPager NulUiPager;

gsize
nul_ui_pager_get_page_count (NulUiPager const *const self);

gsize
nul_ui_pager_get_current_page (NulUiPager const *const self);

G_END_DECLS

#endif /* NUL_UI_PAGINATION_UTIL_H */
