#ifndef NUL_MACROS_H
#define NUL_MACROS_H

#ifdef G_LOG_USE_STRUCTURED

#define nul_error g_error
#define nul_message g_message
#define nul_critical g_critical
#define nul_warning g_warning
#define nul_info g_info
#define nul_debug g_debug

#else

#define nul_error(format, ...) \
  g_error ("%s: " format, G_STRLOC, ##__VA_ARGS__)
#define nul_message(format, ...) \
  g_message ("%s: " format, G_STRLOC, ##__VA_ARGS__)
#define nul_critical(format, ...) \
  g_critical ("%s: " format, G_STRLOC, ##__VA_ARGS__)
#define nul_warning(format, ...) \
  g_warning ("%s: " format, G_STRLOC, ##__VA_ARGS__)
#define nul_info(format, ...) \
  g_info ("%s: " format, G_STRLOC, ##__VA_ARGS__)
#define nul_debug(format, ...) \
  g_debug ("%s: " format, G_STRLOC, ##__VA_ARGS__)

#endif

#endif /* NUL_MACROS_H */
