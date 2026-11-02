#include "ArrayList.h"
#include "List.h"
#include "OOP.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

struct ArrayList {
    List base;
    char *data;
    union {
        size_t length;
        size_t size;
    };
    size_t capacity;
    size_t elementSize;
};

static int ArrLSet(List *restrict list, size_t idx,
                   const void *restrict value) {
    OOP_DEFSELF(list, ArrayList, base);
    if (idx >= self->length) {
        return -1;
    }
    memcpy(self->data + (idx * self->elementSize), value, self->elementSize);
    return 0;
}
static int ArrLResize(List *list, size_t newLen) {
    OOP_DEFSELF(list, ArrayList, base);
    if (newLen == self->length) {
        return 0;
    }
    if (newLen < self->length) {
        // Truncates the array
        size_t offset = newLen * self->elementSize;
        memset(self->data + offset, 0, offset);
        self->length = newLen;
        return 0;
    }
    if (newLen > self->length) {
        if (newLen <= self->capacity) {
            self->length = newLen;
            return 0; // Grow the array if it fits within capacity
        }
        char *tmp =
            realloc(self->data, self->capacity == 0 ? 1 : self->capacity * 2);
        if (!tmp) {
            return -1;
        }
        self->length = newLen;
        self->data = tmp;
    }
    return 0;
}
static int ArrLPush(List *restrict list, const void *restrict value) {
    OOP_DEFSELF(list, ArrayList, base);
    if (ArrLResize(list, self->length + 1) != 0) {
        return -1;
    }
    return ArrLSet(list, self->length - 1, value);
}
static void ArrLPop(List *list) {
    OOP_DEFSELF(list, ArrayList, base);
    // Deletes the last element
    memset(self->data + (--self->length * self->elementSize), 0,
           self->elementSize);
}
static void ArrLClear(List *list) {
    OOP_DEFSELF(list, ArrayList, base);
    memset(self->data, 0, self->length * self->elementSize);
}
static void *ArrLGet(const List *list, size_t idx) {
    OOP_DEFCONSTSELF(list, ArrayList, base);
    if (idx >= self->length) {
        return NULL;
    }
    return self->data + (idx * self->elementSize);
}
static void *ArrLData(const List *list) {
    OOP_DEFCONSTSELF(list, ArrayList, base);
    return self->data;
}
static bool ArrLContains(const List *restrict list,
                         const void *restrict element) {
    OOP_DEFCONSTSELF(list, ArrayList, base);
    char *current_element = self->data;
    for (size_t i = 0; i < self->length;
         i++, current_element += self->elementSize) {
        if (strncmp(current_element, element, self->elementSize) == 0) {
            return true;
        }
    }
    return false;
}
static ptrdiff_t ArrLIndexOf(const List *restrict list,
                             const void *restrict element) {
    OOP_DEFCONSTSELF(list, ArrayList, base);
    char *current_element = self->data;
    for (size_t i = 0; i < self->length;
         i++, current_element += self->elementSize) {
        if (strncmp(current_element, element, self->elementSize) == 0) {
            return (ptrdiff_t)i;
        }
    }
    return -1;
}
static size_t ArrLSize(const List *list) {
    OOP_DEFCONSTSELF(list, ArrayList, base);
    return self->length;
}
static size_t ArrLElementSize(const List *list) {
    OOP_DEFCONSTSELF(list, ArrayList, base);
    return self->elementSize;
}
static bool ArrLIsEmpty(const List *list) {
    OOP_DEFCONSTSELF(list, ArrayList, base);
    return self->length == 0;
}
static void ArrListDestroy(List **list) {
    if (!list || !*list) {
        return;
    }
    OOP_DEFSELF(*list, ArrayList, base);
    if (self->data) {
        free(self->data);
    }
    free(self);
    *list = NULL;
}
const ListVtable ArrayListVtable = {.set = ArrLSet,
                                    .push = ArrLPush,
                                    .resize = ArrLResize,
                                    .pop = ArrLPop,
                                    .clear = ArrLClear,
                                    .get = ArrLGet,
                                    .data = ArrLData,
                                    .contains = ArrLContains,
                                    .indexOf = ArrLIndexOf,
                                    .size = ArrLSize,
                                    .elementSize = ArrLElementSize,
                                    .isEmpty = ArrLIsEmpty,
                                    .destroy = ArrListDestroy};

static size_t nextPowerOf2(size_t n) {
    if (n == 0)
        return 1;
    n--;
    n |= n >> 1U;
    n |= n >> 2U;
    n |= n >> 4U;
    n |= n >> 8U;
    n |= n >> 16U;
#if __SIZEOF_SIZE_T__ == 8 || defined(__x86_64__) || defined(__aarch64__) ||   \
    defined(__amd64__) || defined(_M_IX86) || defined(_M_X64)
    n |= n >> 32U;
#endif
    // TODO: future proofing
    return n;
}
List *ArrListNew(size_t length, size_t elementSize) {
    ArrayList *list = malloc(sizeof(ArrayList));
    if (!list)
        return NULL;
    list->base.vt = &ArrayListVtable;
    list->data = calloc(length, elementSize);
    if (!list->data) {
        free(list);
        return NULL;
    }
    list->length = length;
    list->capacity = nextPowerOf2(length);
    list->elementSize = elementSize;
    return &list->base;
}
List *ArrListFrom(const List *src) {
    // Check for fast path
    if (src->vt == &ArrayListVtable) {
        const ArrayList *arrListSrc = (const ArrayList *)src;
        ArrayList *other = (ArrayList *)ArrListNew(arrListSrc->length,
                                                   arrListSrc->elementSize);
        if (!other)
            return NULL;
        memcpy(other->data, arrListSrc->data,
               arrListSrc->length * arrListSrc->elementSize);
        return &other->base;
    }
    size_t len = listSize(src);
    size_t element_size = listElementSize(src);

    List *other = ArrListNew(len, element_size);
    if (!other)
        return NULL;
    for (size_t i = 0; i < len; i++) {
        if (listPush(other, listGet(src, i)) != 0) {
            listDestroy(&other); // Stop if pushing fails
            return NULL;
        }
    }
    return other;
}
