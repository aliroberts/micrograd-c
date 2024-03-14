#ifndef DARRAY_H
#define DARRAY_H

#include <stdlib.h>  // For size_t definition

// Declaration of the DynamicArray struct
typedef struct {
  void *items;       // Pointer to the array of items
  int size;          // Number of elements currently in the array
  size_t item_size;  // Size of each item in the array
  int capacity;      // Current capacity of the array
} DynamicArray;

// Function prototypes

// Initialize a new dynamic array with items of item_size
DynamicArray *darray_init(size_t item_size);

// Free the memory allocated for the dynamic array
void darray_free(DynamicArray *darray);

// Resize the dynamic array to the new capacity
void darray_resize(DynamicArray *darray, int new_capacity);

// Append an item to the dynamic array
void darray_append(DynamicArray *darray, void *item);

// Get an item from the dynamic array by index
void *darray_get(DynamicArray *darray, int index);

// Set an item in the dynamic array at the specified index
void darray_set(DynamicArray *darray, int index, void *item);

// Peek at the last item in the dynamic array
void *darray_peek(DynamicArray *darray);

// Find the index of an item in the dynamic array
int darray_index_of(DynamicArray *darray, void *item);

// Check if an item is contained in the dynamic array
int darray_contains(DynamicArray *darray, void *item);

#endif  // DARRAY_H
