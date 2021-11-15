#include "hashtable.h"

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

void
ht_destroy(ht* table)
{
  for (size_t i = 0; i < table->capacity; i++) {
    if (table->entries[i] != NULL && table->entries[i]->key != NULL) {
      free(table->entries[i]->key);
    }
  }

  free(table->entries);
  free(table);
}

int
ht_insert(ht* table, wchar_t* key, int value)
{
  if (table->length == table->capacity) {
    return -1;
  }

  unsigned int hash_index = ht_get_index(table, key);

  while (table->entries[hash_index] &&
         table->entries[hash_index] != table->deleted_entry) {

    if (wcscmp(table->entries[hash_index]->key, key) == 0) {
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

  wchar_t* temp_key = (wchar_t*)malloc(wcslen(key) * sizeof(wchar_t) + 1);
  if (temp_key == NULL) {
    return -1;
  }
  wcscpy(temp_key, key);

  new_entry->key = temp_key;
  new_entry->value = value;

  table->entries[hash_index] = new_entry;
  table->length++;
  return 0;
}

int
ht_get(ht* table, wchar_t* key)
{
  unsigned int hash_index = ht_get_index(table, key);

  while (table->entries[hash_index]) {
    if (wcscmp(table->entries[hash_index]->key, key) == 0) {
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
ht_remove(ht* table, wchar_t* key)
{
  unsigned int hash_index = ht_get_index(table, key);

  while (table->entries[hash_index]) {

    if ((table->entries[hash_index] != table->deleted_entry) &&
        (wcscmp(table->entries[hash_index]->key, key) == 0)) {

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

  return;
}

long long
ht_get_hash(wchar_t* key)
{
  long long hash = 0;
  int i = 0;

  while (key[i] != '\0') {
    hash = hash * 31 + key[i];
    i++;
  }

  return hash;
}

int
ht_get_index(ht* table, wchar_t* key)
{
  long long hash = ht_get_hash(key);

  return (hash % table->capacity);
}
