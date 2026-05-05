#include <stdio.h>

// #include "ugame/array.h"
// #include "ugame/vector.h"
// #include "ugame/matrix.h"

#define CHARBUFFER_IMPL
#include "ugame/charbuffer.h"

// File for Testing only.

int main() {
    // ArrayType(uint32_t) a = {};
    // //a = {};
    //
    // arrayPush(&a, 12);
    // arrayPush(&a, 333);
    // arrayInsert(&a, 0, 432);
    // arrayRemove(&a, 1);
    // arrayPop(&a);
    //
    // for (int i = 0; i < a.size; i++)
    //     printf("%d\n", a.data[i]);
    //
    // arrayClean(&a);
    // return 0;

    CharBuffer chars = {};
    charbuffer_astr(&chars, "Hello, World");
    for (int i = 0; i < 10; i++)
        charbuffer_achar(&chars, '!');

    // printf("%s\n", chars.chars);

    charbuffer_clear(&chars);

    CharView v = strview("Hello, World?");

    charbuffer_achars(&chars, v);
    printf("%s\n", chars.chars);

    charbuffer_ichars(&chars, charview_substr(v, 8, 11), 3);
    printf("%s\n", chars.chars);

    charbuffer_rm_prefix(&chars, 5);
    printf("%s\n", chars.chars);


    charbuffer_clean(&chars);
}
