#include <stdio.h>

#include "ugame/array.h"

// File for Testing only.

int main() {
    ArrayType(uint32_t) a = {};
    //a = {};

    arrayPush(a, 12);
    arrayPush(a, 333);
    arrayInsert(a, 0, 432);
    arrayRemove(a, 1);

    for (int i = 0; i < a.size; i++)
        printf("%d\n", a.data[i]);

    arrayClean(a);
}
