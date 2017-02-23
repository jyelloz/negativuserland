#ifndef NUL_UI_PAGE_BROWSER_H
#define NUL_UI_PAGE_BROWSER_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define NUL_UI_TYPE_PAGE_BROWSER (nul_ui_page_browser_get_type ())

struct _NulUiPageBrowserClass
{
  GtkBoxClass parent_class;
};

G_DECLARE_DERIVABLE_TYPE (
  NulUiPageBrowser,
  nul_ui_page_browser,
  NUL_UI,
  PAGE_BROWSER,
  GtkBox
)

G_END_DECLS

#endif /* NUL_UI_PAGE_BROWSER_H */
