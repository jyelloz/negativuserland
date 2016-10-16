#ifndef NUL_UI_SERVICE_STATE_STACK_H
#define NUL_UI_SERVICE_STATE_STACK_H

#include <glib.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

typedef struct _NulUiServiceStateStack NulUiServiceStateStack;

NulUiServiceStateStack *
nul_ui_service_state_stack_new (GtkStack *const stack);

void
nul_ui_service_state_stack_connect (NulUiServiceStateStack *const self);

void
nul_ui_service_state_stack_disconnect (NulUiServiceStateStack *const self);

void
nul_ui_service_state_stack_free (NulUiServiceStateStack *const self);

G_END_DECLS

#endif /* NUL_UI_SERVICE_STATE_STACK_H */
