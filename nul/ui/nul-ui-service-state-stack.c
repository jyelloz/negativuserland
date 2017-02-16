#include "nul-ui-service-state-stack.h"

#include <gtk/gtk.h>

struct _NulUiServiceStateStack {
  GtkStack *stack;
};

NulUiServiceStateStack *
nul_ui_service_state_stack_new (GtkStack *const stack)
{
  NulUiServiceStateStack *const self = g_new0 (NulUiServiceStateStack, 1);
  self->stack = g_object_ref (stack);
  return self;
}

void
nul_ui_service_state_stack_connect (NulUiServiceStateStack *const self)
{
  gtk_stack_set_visible_child_name (self->stack, "connected");
}

void
nul_ui_service_state_stack_disconnect (NulUiServiceStateStack *const self)
{
  gtk_stack_set_visible_child_name (self->stack, "disconnected");
}

void
nul_ui_service_state_stack_free (NulUiServiceStateStack *const self)
{
  g_clear_object (&self->stack);
  g_free (self);
}
