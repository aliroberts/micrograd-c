#include "darray.h"

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DynamicArray *darray_init(size_t item_size) {
  int capacity = 1024;
  DynamicArray *result = malloc(sizeof(DynamicArray));
  result->capacity = capacity;
  result->items = calloc(capacity, item_size);
  result->item_size = item_size;
  result->size = 0;
  return result;
}

void darray_free(DynamicArray *darray) {
  free(darray->items);
  free(darray);
}

void darray_resize(DynamicArray *darray, int new_capacity) {
  darray->items = realloc(darray->items, new_capacity * darray->item_size);
  darray->capacity = new_capacity;
}

void darray_append(DynamicArray *darray, void *item) {
  if (darray->size + 1 > darray->capacity) {
    int updated_capacity = darray->capacity * 2;
    darray_resize(darray, updated_capacity);
  }

  int next_item_index = darray->size * darray->item_size;
  memcpy(darray->items + next_item_index, item, darray->item_size);
  darray->size++;
}

void *darray_get(DynamicArray *darray, int index) {
  return darray->items + index * darray->item_size;
}

void darray_set(DynamicArray *darray, int index, void *item) {
  printf("Setting item at index: %i\n", index);
  if (index >= darray->size) {
    int new_size = index + 1;
    if (new_size > darray->capacity) {
      int new_capacity = new_size * 2;
      darray_resize(darray, new_capacity);
    }
    darray->size = new_size;
  }
  memcpy(darray->items + index * darray->item_size, item, darray->item_size);
}

void *darray_peek(DynamicArray *darray) {
  return darray_get(darray, darray->size - 1);
}

int darray_index_of(DynamicArray *darray, void *item) {
  for (int i = 0; i < darray->size; i++) {
    if (memcmp(darray_get(darray, i), item, darray->item_size) == 0) {
      return i;
    }
  }
  return -1;
}

int darray_contains(DynamicArray *darray, void *item) {
  return darray_index_of(darray, item) != -1;
}