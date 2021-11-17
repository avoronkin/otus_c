#include "hashtable.h"
#define HT_LOAD_FACTOR 0.5
#define HT_RESIZE_FACTOR 2

ht*
ht_create(size_t capacity)
{
  ht_entry deleted_entry;
  ht* table = malloc(sizeof(ht));
  if (!table) {
    return NULL;
  }

  table->length = 0;
  table->capacity = capacity;
  table->deleted_entry = &deleted_entry;

  table->entries = calloc(table->capacity, sizeof(ht_entry*));
  if (table->entries == NULL) {
    free(table);
    return NULL;
  }

  return table;
}

ht*
ht_realloc(ht* table, size_t new_capacity)
{
  ht_entry** new_entries = calloc(new_capacity, sizeof(ht_entry*));
  if (new_entries == NULL) {
    return NULL;
  }

  for (size_t i = 0; i < table->capacity; i++) {
    ht_entry* entry = table->entries[i];
    if (entry != NULL && entry != table->deleted_entry) {
      unsigned int index = ht_get_index(entry->key, new_capacity);
      new_entries[index] = entry;
    }
  }

  free(table->entries);
  table->entries = new_entries;
  table->capacity = new_capacity;

  return table;
}

void
ht_free(ht* table)
{
  for (size_t i = 0; i < table->capacity; i++) {
    if (table->entries[i] != NULL && table->entries[i]->key != NULL) {
      free(table->entries[i]->key);
      free(table->entries[i]);
    }
  }

  free(table->entries);
  free(table);
}

int
ht_insert(ht* table, char* key, int value)
{

  if ((double)table->length / (double)table->capacity >= HT_LOAD_FACTOR) {
    table = ht_realloc(table, table->capacity * HT_RESIZE_FACTOR);
    if (table == NULL) {
      return -1;
    }
  }

  unsigned int hash_index = ht_get_index(key, table->capacity);

  while (table->entries[hash_index] &&
         table->entries[hash_index] != table->deleted_entry) {

    if (strcmp(table->entries[hash_index]->key, key) == 0) {
      table->entries[hash_index]->value = value;
      return 0;
    }

    hash_index++;

    if (hash_index == table->capacity) {
      hash_index = 0;
    }
  }

  ht_entry* new_entry = (ht_entry*)malloc(sizeof(ht_entry));
  if (new_entry == NULL) {
    return -1;
  }

  char* temp_key = (char*)malloc(strlen(key) * sizeof(char) + 1);
  if (temp_key == NULL) {
    return -1;
  }
  strcpy(temp_key, key);

  new_entry->key = temp_key;
  new_entry->value = value;

  table->entries[hash_index] = new_entry;
  table->length++;
  return 0;
}

int
ht_get(ht* table, char* key)
{
  unsigned int hash_index = ht_get_index(key, table->capacity);

  while (table->entries[hash_index]) {
    if (strcmp(table->entries[hash_index]->key, key) == 0) {
      return table->entries[hash_index]->value;
    }

    hash_index++;

    if (hash_index == table->capacity) {
      hash_index = 0;
    }
  }

  return -1;
}

void
ht_remove(ht* table, char* key)
{
  unsigned int hash_index = ht_get_index(key, table->capacity);

  while (table->entries[hash_index]) {

    if ((table->entries[hash_index] != table->deleted_entry) &&
        (strcmp(table->entries[hash_index]->key, key) == 0)) {

      free(table->entries[hash_index]->key);
      free(table->entries[hash_index]);
      table->entries[hash_index] = table->deleted_entry;
      table->length--;

      break;
    }

    hash_index++;

    if (hash_index == table->capacity) {
      hash_index = 0;
    }
  }
}

unsigned int
ht_get_index(char* key, size_t capacity)
{
  long long hash = 0;
  int i = 0;

  while (key[i] != '\0') {
    hash = hash * 31 + key[i];
    i++;
  }

  return (hash % capacity);
}
