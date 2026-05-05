// -- Combined Header/Implementation file. --
// -- Define `CHARBUFFER_IMPL` to include implementation. --

/* CharBuffer and CharView.
 *  - CharBuffer: Dynamic character array which is always null-terminated (string-builder).
 *  - CharView: Sized static character array (string-view).
 */

#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// ---------- //
//   Header   //
// ---------- //

#ifndef CHARBUFFER_INCLUDE
#define CHARBUFFER_INCLUDE

typedef struct charbuffer CharBuffer;
typedef struct charview CharView;

struct charbuffer {
    char*  chars;
    size_t size;
    size_t capacity;
};

struct charview {
    const char* chars;
    size_t      size;
};


CharView strview (const char* str);


void charbuffer_clear (CharBuffer* cb);

void charbuffer_clean (CharBuffer* cb);


void charbuffer_achar (CharBuffer* cb, char ch);

void charbuffer_achars (CharBuffer* cb, CharView src);

void charbuffer_astr (CharBuffer* cb, const char* str);


void charbuffer_ichar (CharBuffer* cb, char ch, size_t i);

void charbuffer_ichars (CharBuffer* cb, CharView src, size_t i);

void charbuffer_istr (CharBuffer* cb, const char* str, size_t i);


char charbuffer_char (CharBuffer* cb, size_t i);

CharView charbuffer_substr (CharBuffer* cb, size_t i, size_t j);

CharView charbuffer_prefix (CharBuffer* cb, size_t i);

CharView charbuffer_suffix (CharBuffer* cb, size_t i);

CharView charbuffer_view (CharBuffer* cb);


char charview_char (CharView src, size_t i);

CharView charview_substr (CharView src, size_t i, size_t j);

CharView charview_prefix (CharView src, size_t i);

CharView charview_suffix (CharView src, size_t i);


void charbuffer_rm_char (CharBuffer* cb, size_t i);

void charbuffer_rm_prefix (CharBuffer* cb, size_t i);

void charbuffer_rm_suffix (CharBuffer* cb, size_t i);

void charbuffer_rm_substr (CharBuffer* cb, size_t i, size_t j);

//
// void charbuffer_read (CharBuffer* cb, FILE* file);
//
// bool charbuffer_read_line (CharBuffer* cb, FILE* file);
//
// void charbuffer_write (CharBuffer* cb, FILE* file);

#endif

// ------------------ //
//   Implementation   //
// ------------------ //

#ifdef CHARBUFFER_IMPL

#define __CHARBUFFER_INITIAL_CAPACITY 256

//
// String View.
//

//
// C-String.
CharView strview (const char* str) {
    return (CharView) { .chars = str, .size = strlen(str) };
}


//
// Buffer Management.
//

//
// Clear buffer
void charbuffer_clear (CharBuffer* cb) {
    cb->size = 0;
    memset(cb->chars, 0, cb->capacity);
}

//
// Cleanup buffer.
void charbuffer_clean (CharBuffer* cb) {
    cb->size = 0;
    cb->capacity = 0;
    cb->chars = (free(cb->chars), NULL);
}

//
// Expand Char Buffer.
static
void __charbuffer_expand (CharBuffer* cb, size_t size) {
    if (cb->capacity == 0) {
        cb->capacity = __CHARBUFFER_INITIAL_CAPACITY;
    }

    size_t new_capacity = cb->capacity;
    while (size + 1 > new_capacity) {
        new_capacity *= 2;
    }

    if (cb->chars == NULL) {
        cb->chars = calloc(new_capacity, sizeof(char));
        cb->capacity = new_capacity;
    } else if (new_capacity > cb->capacity){
        char* new_chars = calloc(new_capacity, sizeof(char));
        for (size_t i = 0; i < cb->size; ++i) {
            new_chars[i] = cb->chars[i];
        }
        cb->chars = (free(cb->chars), new_chars);
        cb->capacity = new_capacity;
    }
}


//
// Appending.
//

//
// Append Character.
void charbuffer_achar (CharBuffer* cb, char ch) {
    __charbuffer_expand(cb, cb->size + 1);
    cb->chars[cb->size] = ch;
    cb->size++;
}

//
// Append Characters.
void charbuffer_achars (CharBuffer* cb, CharView src) {
    __charbuffer_expand(cb, cb->size + src.size);

    for (int i = 0; i < src.size; ++i)
        cb->chars[cb->size + i] = src.chars[i];

    cb->size += src.size;
}

//
// Append String.
void charbuffer_astr (CharBuffer* cb, const char* str) {
    uint32_t len = strlen(str);
    __charbuffer_expand(cb, cb->size + len);

    for (int i = 0; i < len; ++i)
        cb->chars[cb->size + i] = str[i];

    cb->size += len;
}


//
// Inserting,
//

//
// Insert Character.
void charbuffer_ichar (CharBuffer* cb, char ch, size_t i) {
    __charbuffer_expand(cb, cb->size + 1);

    for (size_t x = cb->size - 1; x >= (int) i; --x)
        cb->chars[x + 1] = cb->chars[x];

    cb->chars[i] = ch;
    cb->size++;
}

//
// Insert Characters.
void charbuffer_ichars (CharBuffer* cb, CharView src, size_t i) {
    if (i >= cb->size) {
        charbuffer_achars(cb, src);
        return;
    }

    __charbuffer_expand(cb, cb->size + src.size);

    for (size_t x = cb->size - 1; x >= (int) i; --x)
        cb->chars[x + src.size] = cb->chars[x];

    for (size_t x = 0; x < src.size; ++x)
        cb->chars[x + i] = src.chars[x];

    cb->size += src.size;
}


//
// Insert String.
void charbuffer_istr (CharBuffer* cb, const char* str, size_t i){
    if (i >= cb->size) {
        charbuffer_astr(cb, str);
        return;
    }

    size_t len = strlen(str);
    __charbuffer_expand(cb, cb->size + len);

    for (size_t x = cb->size - 1; x >= (int) i; --x)
        cb->chars[x + len] = cb->chars[x];

    for (size_t x = 0; x < len; ++x)
        cb->chars[x + i] = str[x];

    cb->size += len;
}


//
// View of CharBuffer.
//

//
// Get Character.
char charbuffer_get (CharBuffer* cb, size_t i) {
    return i < cb->size ? cb->chars[i] : 0;
}

//
// Substring.
CharView charbuffer_substr (CharBuffer* cb, size_t i, size_t j){
    if (i > cb->size) i = cb->size;
    if (j > cb->size) j = cb->size;
    if (j < i) j = i;
    return (CharView) { .chars = cb->chars + i, .size = j - i };
}

//
// Prefix.
CharView charbuffer_prefix (CharBuffer* cb, size_t i) {
    return charbuffer_substr(cb, 0, i);
}

//
// Suffix.
CharView charbuffer_suffix (CharBuffer* cb, size_t i) {
    return charbuffer_substr(cb, i, cb->size);
}

//
// Whole Buffer.
CharView charbuffer_view (CharBuffer* cb) {
    return (CharView) { .chars = cb->chars, .size = cb->size };
}


//
// CharView Manipulation.
//

//
// Get Character.
char charview_char (CharView src, size_t i) {
    return i < src.size ? src.chars[i] : 0;
}

//
// Substring.
CharView charview_substr (CharView src, size_t i, size_t j) {
    if (i > src.size) i = src.size;
    if (j > src.size) j = src.size;
    if (j < i) j = i;
    return (CharView) { .chars = src.chars + i, .size = j - i };
}

//
// Prefix.
CharView charview_prefix (CharView src, size_t i) {
    return charview_substr(src, 0, i);
}

//
// Suffix.
CharView charview_suffix (CharView src, size_t i) {
    return charview_substr(src, i, src.size);
}


//
// Removing.
//

//
// Character.
void charbuffer_rm_char (CharBuffer* cb, size_t i) {
    charbuffer_rm_substr(cb, i, i+1);
}

//
// Prefix.
void charbuffer_rm_prefix (CharBuffer* cb, size_t i) {
    charbuffer_rm_substr(cb, 0, i);
}

//
// Suffix.
void charbuffer_rm_suffix (CharBuffer* cb, size_t i) {
    charbuffer_rm_substr(cb, i, cb->size);
}

//
// Substring.
void charbuffer_rm_substr (CharBuffer* cb, size_t i, size_t j) {
    if (i >= j) return;
    if (i >= cb->size) return;
    if (j > cb->size) j = cb->size;

    size_t len = j - i;

    for (size_t x = i; x < cb->size - len + 1; ++x) {
        cb->chars[x] = cb->chars[x+len];
    }

    cb->size -= len;
}

#endif
