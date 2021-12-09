#ifndef HW12_LOGGER_LIBRARY_H
#define HW12_LOGGER_LIBRARY_H

#include <stdarg.h>
#include <stdio.h>

#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_WARNING 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_DEBUG 3

#define LOG_LEVEL_ERROR_NAME "ERROR"
#define LOG_LEVEL_WARNING_NAME "WARNING"
#define LOG_LEVEL_INFO_NAME "INFO"
#define LOG_LEVEL_DEBUG_NAME "DEBUG"

typedef struct log_t
{

  int log_level;
  FILE* out;
} log_t;

log_t*
log_create(int log_level, FILE* out);

void
log_destroy(log_t* log);

void
log_print(log_t* log, int log_level, char* file, int line, const char* fmt, ...);

#define log_error(out, ...)                                                     \
  log_print(out, LOG_LEVEL_ERROR, __FILE__, __LINE__, __VA_ARGS__)

#define log_warning(out, ...)                                                    \
  log_print(out, LOG_LEVEL_WARNING, __FILE__, __LINE__, __VA_ARGS__)

#define log_info(out, ...)                                                     \
  log_print(out, LOG_LEVEL_INFO, __FILE__, __LINE__, __VA_ARGS__)

#define log_debug(out, ...)                                                     \
  log_print(out, LOG_LEVEL_DEBUG, __FILE__, __LINE__, __VA_ARGS__)

#endif // HW12_LOGGER_LIBRARY_H
