#include "set.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

uintptr_t hash(void *item) { return ((uintptr_t)item >> 5); }

Set *set_init() {
  Set *result = malloc(sizeof(Set));
  result->capacity = 1024;
  result->items = calloc(result->capacity, sizeof(SetItem *));
  result->size = 0;
  return result;
}

void set_free(Set *set) {
  // Free all the items in the set and those in the linked list
  for (int i = 0; i < set->capacity; i++) {
    SetItem *item = set->items[i];
    while (item != NULL) {
      SetItem *next = item->next;
      free(item);
      item = next;
    }
  }
}

void print_set(Set *set) {
  for (int i = 0; i < set->capacity; i++) {
    SetItem *item = set->items[i];
    while (item != NULL) {
      printf("-> Item key: %lu", item->key);
      item = item->next;
    }
    if (set->items[i] != NULL) {
      printf("\n");
    }
  }
}

void copy_set_items(SetItem **from_items, int from_capacity, SetItem **to_items,
                    int to_capacity) {
  for (int i = 0; i < from_capacity; i++) {
    SetItem *item = from_items[i];
    if (item == NULL) {
      continue;
    }
    int index = item->key % to_capacity;
    SetItem *current = to_items[index];
    SetItem *prev = NULL;

    while (current != NULL) {
      prev = current;
      current = current->next;
    }

    if (prev == NULL) {
      to_items[index] = item;
    } else {
      prev->next = item;
    }
  }
}

void set_add(Set *set, void *item) {
  uintptr_t key = hash(item);
  int index = key % set->capacity;

  // Insert the item into the set
  SetItem *current = set->items[index];
  SetItem *prev = NULL;
  while (current != NULL) {
    if (current->key == key) {
      return;
    }
    prev = current;
    current = current->next;
  }
  SetItem *new_item = malloc(sizeof(SetItem));
  new_item->key = key;
  new_item->next = NULL;
  if (prev == NULL) {
    set->items[index] = new_item;
  } else {
    prev->next = new_item;
  }
  set->size++;

  // Resize the set if necessary
  if (set->size > set->capacity * 0.5) {
    int new_capacity = set->capacity * 2;
    SetItem **new_items = calloc(new_capacity, sizeof(SetItem *));
    SetItem **old_items = set->items;
    copy_set_items(old_items, set->capacity, new_items, new_capacity);
    set->items = new_items;
    set->capacity = new_capacity;

    free(old_items);
  }
}

bool set_contains(Set *set, void *item) {
  uintptr_t key = hash(item);
  int index = key % set->capacity;
  SetItem *current = set->items[index];
  while (current != NULL) {
    if (current->key == key) {
      return true;
    }
    current = current->next;
  }
  return false;
}