#ifndef HASHTABLE_H_INCLUDED
#define HASHTABLE_H_INCLUDED 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ht_entry
{
  char* key;
  int value;
} ht_entry;

typedef struct ht
{
  ht_entry** entries;
  ht_entry* deleted_entry;
  size_t length;
  size_t capacity;
} ht;

ht*
ht_create(size_t capacity);

ht*
ht_realloc(ht* table, size_t new_capacity);

void
ht_free(ht* table);

int
ht_insert(ht* table, char* key, int value);

int
ht_get(ht* table, char* key);

void
ht_remove(ht* table, char* key);

unsigned int
ht_get_index(char* key, size_t capacity);

#endif /* HASHTABLE_H_INCLUDED */
