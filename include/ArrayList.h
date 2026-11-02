#ifndef ARRAYLIST_H
#define ARRAYLIST_H
#include "List.h"
#include <stddef.h>

typedef struct ArrayList ArrayList;
extern const ListVtable ArrayListVtable;

/**
 * @brief Makes a new ArrayList with a given length.
 * Creates an ArrayList on the heap with length and capacity set to length, and
 * elementSize set to the parameter of the same name.
 * @param[in] length The amount of elements in the ArrayList.
 * @param[in] elementSize The size of each element (e.g. 4 for int).
 * @return List* A new ArrayList (cast to List) on the heap.
 */
List *ArrListNew(size_t length, size_t elementSize);

/**
 * @brief Copies a List from an existing one.
 * Copies other into a new ArrayList on the heap.
 * @param[in] other The other List to be copied.
 * @return List* A new ArrayList (cast to List) on the heap.
 * @note other is still a heap variable, so it must be freed with listDestroy().
 */
List *ArrListFrom(const List *src);
#endif
