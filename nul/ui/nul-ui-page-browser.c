#include "nul-ui-page-browser.h"

#include "nul-external-autocleanups.h"

#define get_priv(self) \
  nul_ui_page_browser_get_instance_private (NUL_UI_PAGE_BROWSER (self))

typedef struct
{

  GtkButton *prev_button;
  GtkButton *next_button;
  GtkLabel *status_label;

  GtkFlowBox *flow;
  GListStore *model;


} NulUiPageBrowserPrivate;

G_DEFINE_ABSTRACT_TYPE (
  NulUiPageBrowser,
  nul_ui_page_browser,
  GTK_TYPE_BOX
);

static void
child_activated_cb (GtkFlowBox       *const flow,
                    GtkFlowBoxChild  *const child,
                    NulUiPageBrowser *const self)
{

  NulUiPageBrowserPrivate *const priv = get_priv (self);
  GListModel *const model = G_LIST_MODEL (priv->model);
  gint const index = gtk_flow_box_child_get_index (child);

  GObject *const item = g_list_model_get_item (model, index);

  gchar const *const text = g_object_get_data (item, "text");

  nul_debug (
    "row %d activated, text=%s user_data is %s@%p",
    index,
    text,
    G_OBJECT_TYPE_NAME (self),
    self
  );

}

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

  gtk_widget_class_bind_template_child_private (
    widget_class,
    NulUiPageBrowser,
    status_label
  );

  gtk_widget_class_bind_template_child_private (
    widget_class,
    NulUiPageBrowser,
    flow
  );

  gtk_widget_class_bind_template_callback (
    widget_class,
    child_activated_cb
  );

}

static inline void
append (GListStore  *const model,
        gchar const *const text)
{
  GObject *const obj = g_object_new (G_TYPE_OBJECT, NULL);
  g_object_set_data (obj, "text", text);
  g_list_store_append (model, obj);
}

static GtkWidget *
create_widget (GObject          *const item,
               NulUiPageBrowser *const self)
{
  return gtk_label_new (g_object_get_data (item, "text"));
}

static void
nul_ui_page_browser_init (NulUiPageBrowser *const self)
{

  NulUiPageBrowserPrivate *const priv = get_priv (self);

  GListStore *const model = priv->model = g_list_store_new (G_TYPE_OBJECT);

  gtk_widget_init_template (GTK_WIDGET (self));

  gtk_label_set_text (
    priv->status_label,
    g_strdup_printf (
      "%s#%p",
      G_OBJECT_TYPE_NAME (self),
      self
    )
  );

  g_signal_connect (
    self,
    "map",
    G_CALLBACK (map_cb),
    NULL
  );

  gtk_flow_box_bind_model (
    priv->flow,
    G_LIST_MODEL (model),
    (GtkFlowBoxCreateWidgetFunc) create_widget,
    self,
    NULL
  );

}

NulUiPageBrowser *
nul_ui_page_browser_new (void)
{
  return g_object_new (
    NUL_UI_TYPE_PAGE_BROWSER,
    "width-request", 720,
    "height-request", 480,
    NULL
  );
}
