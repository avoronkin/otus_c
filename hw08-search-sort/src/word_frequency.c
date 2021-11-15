#include "word_frequency.h"
#include "hashtable.h"

wchar_t*
prepare_word(wchar_t* str)
{
  while (iswspace(*str) || iswpunct(*str) || *str == L'–' || *str == L'—' ||
         *str == L'«') {
    str++;
  }

  if (*str == 0) {
    return str;
  }

  wchar_t* end;
  end = str + wcslen(str) - 1;
  while (end > str && (iswspace(*end) || iswpunct(*end) || *end == L'–' ||
                       *end == L'—' || *end == L'»')) {
    end--;
  }

  end[1] = '\0';

  for (int i = 0; str[i]; i++) {
    str[i] = towlower(str[i]);
  }

  return str;
}

int
cmp_count(const void* p1, const void* p2)
{
  struct ht_entry* ptr1 = *(ht_entry**)p1;
  struct ht_entry* ptr2 = *(ht_entry**)p2;

  int c1 = ptr1->value;
  int c2 = ptr2->value;

  if (c1 == c2)
    return 0;
  if (c1 < c2)
    return 1;
  return -1;
}

wf*
wf_count_words(FILE* fp)
{

  ht* word_table = ht_create(60000);
  if (word_table == NULL) {
    return NULL;
  }

  wchar_t word[101];
  while (fwscanf(fp, L"%100ls", word) != EOF) {
    wchar_t* tmp_word = prepare_word(word);

    int count = ht_get(word_table, tmp_word);
    if (count == -1) {
      count = 1;
    } else {
      count += 1;
    }

    int return_code = ht_insert(word_table, tmp_word, count);
    if (return_code == -1) {
      return NULL;
    }
  }

  wf* result = malloc(sizeof(wf));
  if (result == NULL) {
    return NULL;
  }

  result->word_count = 0;
  result->entries = calloc(word_table->length, sizeof(ht_entry*));

  for (size_t i = 0, i_unique = 0; i < word_table->capacity; i++) {
    if (word_table->entries[i] != NULL && word_table->entries[i]->key != NULL) {
      wf_entry* new_entry = (wf_entry*)malloc(sizeof(wf_entry));
      if (new_entry == NULL) {
        return NULL;
      }

      new_entry->word = (wchar_t*)malloc(wcslen(word_table->entries[i]->key) * sizeof(wchar_t) + 1);
      if (new_entry->word == NULL) {
        return NULL;
      }
      wcscpy(new_entry->word, word_table->entries[i]->key);

      new_entry->count = word_table->entries[i]->value;

      result->entries[i_unique++] = new_entry;
      result->word_count += 1;
    }
  }

  ht_destroy(word_table);

  qsort(result->entries, result->word_count, sizeof(ht_entry*), cmp_count);

  return result;
}
