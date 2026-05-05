#pragma once

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define __ARRAY_INITIAL_CAPACITY 64

#define ArrayType(T) struct { T* data; size_t size; size_t capacity; }

typedef ArrayType(void*) Array;

typedef ArrayType(int32_t)  Arrayi32;
typedef ArrayType(int64_t)  Arrayi64;
typedef ArrayType(uint32_t) Arrayu32;
typedef ArrayType(uint64_t) Arrayu64;
typedef ArrayType(size_t)   Arraysz;

typedef ArrayType(float)  Arrayf32;
typedef ArrayType(double) Arrayf64;

static inline
void __array_alloc (void** data, size_t* size, size_t* capacity, size_t sz) {
    if (*capacity == 0)
        *capacity = __ARRAY_INITIAL_CAPACITY;

    if (*data == NULL)
        *data = malloc(sz * *capacity);

    if (*size >= *capacity){
        *capacity *= 2;
        *data = realloc(*data, sz * *capacity);
    }
}

static inline
void __array_move_in (void** data, size_t* size, size_t* capacity, size_t sz, size_t index) {
    assert(*size >= index);

    __array_alloc(data, size, capacity, sz);

    uintptr_t src = (uintptr_t) *data + (index * sz);
    uintptr_t dst = (uintptr_t) *data + ((index + 1) * sz);

    memmove((void*) dst, (void*) src, (*size - index) * sz);
    (*size)++;
}

static inline
void __array_move_out (void** data, size_t* size, size_t* capacity, size_t sz, size_t index) {
    assert(*size > index);

    uintptr_t src = (uintptr_t) *data + (index * sz);
    uintptr_t dst = (uintptr_t) *data + ((index + 1) * sz);
    uintptr_t end = (uintptr_t) *data + ((*size - 1) * sz);

    void* p = alloca(sz);
    memcpy(p, (void*) src, sz);
    memmove((void*) src, (void*) dst, (*size - index) * sz); // This is backwards on purpose.
    memcpy((void*) end, p, sz);
    (*size)--;
}


#define arrayPush(array, item) \
  ( __array_alloc((void**) &(array)->data, &(array)->size, &(array)->capacity, sizeof *(array)->data), \
    (array)->data[(array)->size++] = (item) )

#define arrayPop(array) \
  ( assert((array)->size > 0), \
    (array)->data[--(array)->size] )

#define arrayPeek(array) \
  ( assert((array)->size > 0), \
    (array)->data[(array)->size - 1] )


#define arrayInsert(array, index, item) \
  ( __array_move_in((void**) &(array)->data, &(array)->size, &(array)->capacity, sizeof *(array)->data, (index)), \
    (array)->data[index] = (item) )

#define arrayRemove(array, index) \
  ( __array_move_out((void**) &(array)->data, &(array)->size, &(array)->capacity, sizeof *(array)->data, (index)), \
    (array)->data[(array)->size] )


#define arrayClear(array) \
  ( (array)->size = 0 )

#define arrayClean(array) \
  ( free((array)->data), \
    (array)->data = NULL, \
    (array)->size = 0 )


#define array_push arrayPush
#define array_pop arrayPop
#define array_peek arrayPeek
#define array_insert arrayInsert
#define array_remove arrayRemove
#define array_clear arrayClear
#define array_clean arrayClean
