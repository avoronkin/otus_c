#ifndef COUNTER_H_INCLUDED
#define COUNTER_H_INCLUDED 1

#include "hashtable.h"
#include <stdio.h>
#include <wchar.h>
#include <wctype.h>

typedef struct wf_entry
{
  wchar_t* word;
  int count;
} wf_entry;

typedef struct wf
{
  wf_entry** entries;
  size_t word_count;
} wf;

wf*
wf_count_words(FILE* fp);

#endif // COUNTER_H_INCLUDED
