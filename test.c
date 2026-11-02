#include "ArrayList.h"
#include "List.h"
#include <stdio.h>
#include <stddef.h>
#undef NDEBUG
#include <assert.h>

int main(void) {
    List *a = ArrListNew(0, sizeof(int));
    listPush(a, &(int){1});
    assert(*(int *)listGet(a, 0) == 1);
    listPop(a);
    assert(listIsEmpty(a));
    listResize(a, 3);
    listSet(a, 0, &(int){1});
    listSet(a, 2, &(int){2});
    assert(listContains(a, &(int){1}));
    assert(listIndexOf(a, &(int){1}) == 0);
    assert(listSize(a) == 3);
    List *b = ArrListFrom(a);
    for (size_t i = 0; i < listSize(a); i++) {
        assert(*(int *)listGet(a, i) == *(int *)listGet(b, i));
    }
    listDestroy(&a);
    listDestroy(&b);
}
