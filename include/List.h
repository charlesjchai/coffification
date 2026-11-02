#ifndef LIST_H
#define LIST_H
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct List List;
typedef struct {
    int (*set)(List *restrict, size_t, const void *restrict);
    int (*remove)(List *restrict, size_t);
    int (*resize)(List *, size_t);
    int (*push)(List *restrict, const void *restrict);
    void (*pop)(List *);
    void (*clear)(List *);
    void *(*get)(const List *, size_t);
    void *(*data)(const List *);
    bool (*contains)(const List *restrict, const void *restrict);
    ptrdiff_t (*indexOf)(const List *restrict, const void *restrict);
    size_t (*size)(const List *);
    size_t (*elementSize)(const List *);
    bool (*isEmpty)(const List *);
    void (*destroy)(List **);
} ListVtable;
struct List {
    const ListVtable *vt;
}; // clang-format off
static inline int listSet(List *restrict list, size_t idx, const void *restrict val) { return list->vt->set(list, idx, val); }
static inline int listRemove(List *list, size_t idx) { return list->vt->remove(list, idx); }
static inline int listResize(List *restrict list, size_t idx) { return list->vt->resize(list, idx); }
static inline int listPush(List *restrict list, const void *restrict val) { return list->vt->push(list, val); }
static inline void listPop(List *list) { list->vt->pop(list); }
static inline void listClear(List *list) { list->vt->clear(list); }
static inline void *listGet(const List *list, size_t idx) { return list->vt->get(list, idx); }
static inline void *listData(const List *list) { return list->vt->data(list); }
static inline bool listContains(const List *restrict list, const void *restrict val) { return list->vt->contains(list, val); }
static inline ptrdiff_t listIndexOf(const List *restrict list, const void *restrict val) { return list->vt->indexOf(list, val); }
static inline size_t listSize(const List *list) { return list->vt->size(list); }
static inline size_t listElementSize(const List *list) { return list->vt->elementSize(list); }
static inline bool listIsEmpty(const List *list) { return list->vt->isEmpty(list); }
static inline void listDestroy(List **list) { (*list)->vt->destroy(list);  }
#endif // clang-format on
