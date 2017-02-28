#include "nul-ui-page-browser.h"

#include "nul-external-autocleanups.h"

#define get_priv(self) \
  ((NulUiPageBrowserPrivate *) \
    nul_ui_page_browser_get_instance_private (NUL_UI_PAGE_BROWSER (self)))

typedef struct
{

  GtkButton *prev_button;
  GtkButton *next_button;
  GtkLabel *status_label;

  GtkFlowBox *flow;
  GListStore *model;

  int page;

} NulUiPageBrowserPrivate;

G_DEFINE_ABSTRACT_TYPE (
  NulUiPageBrowser,
  nul_ui_page_browser,
  GTK_TYPE_BOX
);

enum {
  PROP_0,
  PROP_PAGE,
  N_PROPS,
};

static GParamSpec *properties[N_PROPS];

static inline gint
get_page (NulUiPageBrowser *const self)
{
  return get_priv (self)->page;
}

static inline void
set_page (NulUiPageBrowser *const self,
          gint const              page)
{
  g_return_if_fail (page >= 0);
  get_priv (self)->page = MAX (0, page);
  g_object_notify_by_pspec (G_OBJECT (self), properties[PROP_PAGE]);
}

static void
prev_cb (GtkButton        *const button,
         NulUiPageBrowser *const self)
{
  nul_debug ("prev");
  set_page (self, get_page (self) - 1);
}

static void
next_cb (GtkButton        *const button,
         NulUiPageBrowser *const self)
{
  nul_debug ("next");
  set_page (self, get_page (self) + 1);
}

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
get_property (GObject    *const object,
              guint const       prop_id,
              GValue     *const value,
              GParamSpec *const pspec)
{

  NulUiPageBrowserPrivate *const priv = get_priv (object);

  switch (prop_id) {
    case PROP_PAGE:
      g_value_set_int (value, priv->page);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
  }

}

static void
set_property (GObject      *const object,
              guint const         prop_id,
              GValue const *const value,
              GParamSpec   *const pspec)
{
  switch (prop_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
  }
}


static void
nul_ui_page_browser_class_init (NulUiPageBrowserClass *const cls)
{

  GObjectClass *const gobject_class = G_OBJECT_CLASS (cls);
  GtkWidgetClass *const widget_class = GTK_WIDGET_CLASS (cls);

  gobject_class->get_property = get_property;
  gobject_class->set_property = set_property;

  properties[PROP_PAGE] = g_param_spec_int (
    "page", "Page", "Page",
    0, G_MAXINT,
    0,
    G_PARAM_STATIC_STRINGS | G_PARAM_READABLE
  );

  g_object_class_install_properties (
    gobject_class,
    N_PROPS,
    properties
  );

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
    prev_cb
  );

  gtk_widget_class_bind_template_callback (
    widget_class,
    next_cb
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
  g_object_set_data_full (obj, "text", g_strdup (text), g_free);
  g_list_store_append (model, obj);
}

static GtkWidget *
create_widget (GObject          *const item,
               NulUiPageBrowser *const self)
{
  return gtk_label_new (g_object_get_data (item, "text"));
}

static gboolean
page_to_prev_sensitive (GBinding     *const binding,
                        GValue const *const from,
                        GValue       *const to,
                        gpointer      const user_data)
{
  g_value_set_boolean (to, g_value_get_int (from) > 0);
  return TRUE;
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

  gtk_flow_box_bind_model (
    priv->flow,
    G_LIST_MODEL (model),
    (GtkFlowBoxCreateWidgetFunc) create_widget,
    self,
    NULL
  );

  g_object_bind_property_full (
    self,
    "page",
    priv->prev_button,
    "sensitive",
    G_BINDING_DEFAULT | G_BINDING_SYNC_CREATE,
    page_to_prev_sensitive,
    NULL,
    NULL,
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
