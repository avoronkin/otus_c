#ifndef COUNTER_H_INCLUDED
#define COUNTER_H_INCLUDED 1

#include "hashtable.h"
#include <stdio.h>

typedef struct wf_entry
{
  char* word;
  int count;
} wf_entry;

typedef struct wf
{
  wf_entry** entries;
  size_t word_count;
} wf;

wf*
wf_count_words(FILE* fp);

void
wf_free(wf* wf);

#endif // COUNTER_H_INCLUDED
