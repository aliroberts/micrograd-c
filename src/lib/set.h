#ifndef SET_H
#define SET_H

#include <stdlib.h> // For size_t and malloc, calloc, free
#include <stdbool.h> // For bool type

// Forward declaration of SetItem struct to allow for self-referencing
typedef struct SetItem {
  uintptr_t key;  // Key of the item
  struct SetItem *next;  // Pointer to the next item in the chain (for handling collisions)
} SetItem;

// Declaration of the Set struct
typedef struct {
  SetItem **items;  // Array of pointers to items
  int capacity;     // Capacity of the set
  int size;         // Current number of items in the set
} Set;

// Function prototypes

// Initialize a new set
Set *set_init();

// Free the memory allocated for the set
void set_free(Set *set);

// Print all items in the set
void print_set(Set *set);

// Add an item to the set
void set_add(Set *set, void *item);

// Check if an item is contained in the set
bool set_contains(Set *set, void *item);

#endif // SET_H
