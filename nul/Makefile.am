NULL =

bin_PROGRAMS = nul-ui
libexec_PROGRAMS = nul-service

nul_ui_SOURCES = \
	nul-ui-main.c \
	nul-window.c \
	$(NULL)

nul_service_SOURCES = \
	nul-service-main.c \
	$(NULL)

nul_ui_CFLAGS = $(NUL_UI_CFLAGS)
nul_service_CFLAGS = $(NUL_SERVICE_CFLAGS)

nul_ui_LDADD = $(NUL_UI_LIBS)
nul_service_LDADD = $(NUL_SERVICE_LIBS)

CLEANFILES =
