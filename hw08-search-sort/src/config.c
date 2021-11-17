#include "config.h"
#include <string.h>

void
print_help()
{
  fprintf(stderr,
          "Подсчитывает и выводит на экран информацию о том, сколько раз в "
          "файле встречается каждое слово, которое есть в файле\n"
          "Использование: wf [-i input.txt]\n"
          "Параметры:\n"
          "  -i входной файл\n"
          "  -h справка\n"
          "Пример: wf -i input.txt\n");
}

cfg
cf_create(int argc, char** argv)
{
  cfg config = { .input = stdin };

  for (int i = 1; i < argc; i++) {
    char* key = argv[i];
    char* value = argv[i + 1];

    if (strcmp(key, "-h") == 0) {
      print_help();
      exit(EXIT_FAILURE);
    } else if (strcmp(key, "-i") == 0) {
      i++;

      if (value != NULL) {
        config.input = fopen(value, "r");
        if (config.input == NULL) {
          fprintf(
            stderr, "Ошибка: не удалось открыть входной файл %s\n", value);
          exit(EXIT_FAILURE);
        }
      }
    }
  }

  return config;
}
