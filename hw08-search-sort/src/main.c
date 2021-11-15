#include "config.h"
#include "word_frequency.h"
#include <locale.h>
#include <stdlib.h>

int
main(int argc, char** argv)
{
  setlocale(LC_ALL, "");
  cfg config = cf_create(argc, argv);

  wf* word_frequency = wf_count_words(config.input);
  if (word_frequency == NULL) {
    fprintf(stderr, "Ошибка при подсчёте слов\n");
    exit(EXIT_FAILURE);
  }

  for (size_t i = 0; i < word_frequency->word_count; i++) {
    printf("%ls %d\n",
           word_frequency->entries[i]->word,
           word_frequency->entries[i]->count);
  }

  if (fclose(config.input) != 0) {
    fprintf(stderr, "Ошибка при закрытии файла\n");
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}
