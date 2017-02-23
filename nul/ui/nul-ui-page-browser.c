#include "nul-ui-page-browser.h"

#define get_priv nul_ui_page_browser_get_instance_private

typedef struct
{

  GtkButton *prev_button;
  GtkButton *next_button;

} NulUiPageBrowserPrivate;

G_DEFINE_ABSTRACT_TYPE (
  NulUiPageBrowser,
  nul_ui_page_browser,
  GTK_TYPE_BOX
)

static void
nul_ui_page_browser_class_init (NulUiPageBrowserClass *const cls)
{

  GtkWidgetClass *const widget_class = GTK_WIDGET_CLASS (cls);

  gtk_widget_class_set_template_from_resource (
    widget_class,
    "/org/negativuserland/Ui/page-browser.ui"
  );

  gtk_widget_class_bind_template_child_private (
    widget_class,
    NulUiPageBrowser,
    prev_button
  );

  gtk_widget_class_bind_template_child_private (
    widget_class,
    NulUiPageBrowser,
    next_button
  );

}

static void
nul_ui_page_browser_init (NulUiPageBrowser *const self)
{

  gtk_widget_init_template (GTK_WIDGET (self));

}
