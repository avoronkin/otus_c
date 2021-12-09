#include "logger.h"
#include <execinfo.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void
print_level(log_t* log, int log_level)
{
  switch (log_level) {
    case LOG_LEVEL_DEBUG:
      fprintf(log->out, "[%s] ", LOG_LEVEL_DEBUG_NAME);
      break;
    case LOG_LEVEL_INFO:
      fprintf(log->out, "[%s] ", LOG_LEVEL_INFO_NAME);
      break;
    case LOG_LEVEL_WARNING:
      fprintf(log->out, "[%s] ", LOG_LEVEL_WARNING_NAME);
      break;
    case LOG_LEVEL_ERROR:
      fprintf(log->out, "[%s] ", LOG_LEVEL_ERROR_NAME);
      break;
    default:
      fprintf(log->out, "[UNKNOWN] ");
      break;
  }
}

static void
print_time(log_t* log)
{
  time_t current_time = time(NULL);
  tzset();
  char current_time_string[128];
  strftime(current_time_string,
           sizeof(current_time_string),
           "%a %b %d %T %Y",
           localtime(&current_time));
  fprintf(log->out, "%s ", current_time_string);
}

static void
print_backtrace(log_t* log)
{
  void* buffer[100];
  int nptrs = backtrace(buffer, 100);
  fprintf(log->out, "\n");

  char** strings = backtrace_symbols(buffer, nptrs);
  if (strings == NULL) {
    return;
  }

  for (int j = 0; j < nptrs; j++) {
    fprintf(log->out, "%s\n", strings[j]);
  }

  free(strings);
}

log_t*
log_create(int log_level, FILE* out)
{
  log_t* log = malloc(sizeof(log_t));
  if (!log) {
    return NULL;
  }

  log->log_level = log_level;
  log->out = out;

  return log;
}

void
log_destroy(log_t* log)
{
  free(log);
}


void
log_print(log_t* log, int log_level, char* file, int line, const char* fmt, ...)
{
  if (log_level > log->log_level) {
    return;
  }

  print_level(log, log_level);
  print_time(log);
  fprintf(log->out, "%s:%d ", file, line);

  va_list args;
  va_start(args, fmt);
  vfprintf(log->out, fmt, args);
  va_end(args);

  if (log_level == LOG_LEVEL_ERROR) {
    print_backtrace(log);
  } else {
    fprintf(log->out, "\n");
  }
}
