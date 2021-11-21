#include "config.h"
#include <string.h>

void
print_help()
{
  fprintf(stderr,
          "Приложение выводит на экран прогноз погоды на текущий день: текстовое "
          "описание погоды, направление и скорость ветра, диапазон температуры\n"
          "Параметры:\n"
          "  -l название локации\n"
          "  -h справка\n"
          "Пример: weather -l Moscow\n");
}

cfg
cf_create(int argc, char** argv)
{
  cfg config = { NULL };

  for (int i = 1; i < argc; i++) {
    char* key = argv[i];
    char* value = argv[i + 1];

    if (strcmp(key, "-h") == 0) {
      print_help();
      exit(EXIT_FAILURE);
    } else if (strcmp(key, "-l") == 0) {
      i++;

      config.location_name = value;
    }
  }

  if (!config.location_name) {
    fprintf(stderr, "Ошибка: Не указано название локации\n\n");
    print_help();
    exit(EXIT_FAILURE);
  }

  return config;
}
