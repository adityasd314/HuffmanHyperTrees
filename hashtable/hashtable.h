#pragma once
#ifndef _HASHTABLE_H
#define _HASHTABLE_H
#endif

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    char* key;
    void* value;
} ht_entry;

typedef struct hashtable {
    ht_entry* entries;
    size_t capacity;
    size_t length;
}hashtable;

#define INT_INPUT 0
#define NON_INT_INPUT 1

typedef hashtable* ht;
void hashtable_create(ht* htable);
void ht_destroy(ht* htable);
void* ht_get(ht* table, const char* key);
void ht_set(ht* table, char* key, void* value, int input_type, int lenvariable);
size_t ht_length(ht* table);

typedef struct {
    const char* key;
    void* value;
    ht* _table;
    size_t _index;
} hti;

hti ht_iterator(ht* table);
bool ht_next(hti* it);