#include "logger.h"
#include <stdlib.h>
#include <string.h>

void
func3(log_t* log)
{
  log_error(log, "eror in func3()");
}

void
func2(log_t* log)
{
  func3(log);
}

void
func1(log_t* log)
{
  func2(log);
}

int
main(int argc, char* argv[])
{
  int log_level = LOG_LEVEL_DEBUG;
  FILE* log_file = stdout;

  for (int i = 1; i < argc; i++) {
    char* key = argv[i];
    char* value = argv[i + 1];

    if (strcmp(key, "-l") == 0) {
      if (strcmp(value, "error") == 0) {
        log_level = LOG_LEVEL_ERROR;
      } else if (strcmp(value, "warning") == 0) {
        log_level = LOG_LEVEL_WARNING;
      } else if (strcmp(value, "info") == 0) {
        log_level = LOG_LEVEL_INFO;
      } else if (strcmp(value, "debug") == 0) {
        log_level = LOG_LEVEL_DEBUG;
      } else {
        fprintf(stderr, "Invalid log level: %s\n", value);
        exit(EXIT_FAILURE);
      }
      i++;

    } else if (strcmp(key, "-f") == 0) {
      i++;

      if (value != NULL) {
        log_file = fopen(value, "w");
        if (log_file == NULL) {
          fprintf(stderr, "Could not open file %s\n", value);
          exit(EXIT_FAILURE);
        }
      }
    }
  }

  log_t* log = log_create(log_level, log_file);
  fprintf(stdout, "log level: %d\n", log->log_level);

  log_info(log, "info log level");
  log_info(log, "info log level with formating. %s %d", "It's a test.", 2);
  log_error(log, "eror log level");
  log_warning(log, "warning log level");
  log_debug(log, "debug log level");
  func1(log);

  log_destroy(log);

  return 0;
}
