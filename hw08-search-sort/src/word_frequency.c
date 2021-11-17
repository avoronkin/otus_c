#include "word_frequency.h"
#include "hashtable.h"
#include <wchar.h>
#include <wctype.h>

char*
prepare_word(char* mbs)
{
  const size_t strSize = strlen(mbs) + 1;
  wchar_t ws[strSize];
  wchar_t* wcs = ws;
  mbstowcs(wcs, mbs, strSize);

  for (int i = 0; wcs[i]; i++) {
    wcs[i] = towlower(wcs[i]);
  }

  while (iswdigit(*wcs) || iswspace(*wcs) || iswpunct(*wcs) ||
         *wcs == L'–' || *wcs == L'—' || *wcs == L'«') {
    wcs++;
  }

  if (*wcs == 0) {
    return NULL;
  }

  wchar_t* end = wcs + wcslen(wcs) - 1;
  while (end > wcs && (iswdigit(*end) || iswspace(*end) || iswpunct(*end) ||
                       *end == L'–' || *end == L'—' || *end == L'»')) {
    end--;
  }

  end[1] = '\0';

  wcstombs(mbs, wcs, strSize);

  return mbs;
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

  ht* word_table = ht_create(1024);
  if (word_table == NULL) {
    return NULL;
  }

  char tmp[101];
  while (fscanf(fp, "%100s", tmp) != EOF) {
    char* word = prepare_word(tmp);
    if (word == NULL) {
      continue;
    }

    int count = ht_get(word_table, word);
    if (count == -1) {
      count = 1;
    } else {
      count += 1;
    }

    int return_code = ht_insert(word_table, word, count);
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

      new_entry->word =
        (char*)malloc(strlen(word_table->entries[i]->key) * sizeof(char) + 1);
      if (new_entry->word == NULL) {
        return NULL;
      }
      strcpy(new_entry->word, word_table->entries[i]->key);

      new_entry->count = word_table->entries[i]->value;

      result->entries[i_unique++] = new_entry;
      result->word_count += 1;
    }
  }

  ht_free(word_table);

  qsort(result->entries, result->word_count, sizeof(ht_entry*), cmp_count);

  return result;
}

void
wf_free(wf* wf)
{
  if (wf == NULL) {
    return;
  }

  for (size_t i = 0; i < wf->word_count; i++) {
    free(wf->entries[i]->word);
    free(wf->entries[i]);
  }

  free(wf->entries);
  free(wf);
}
