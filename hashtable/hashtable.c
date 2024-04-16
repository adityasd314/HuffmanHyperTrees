#include "hashtable.h"
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 1000
int count = 0;

void hashtable_create(ht* htable) {
    ht table = malloc(sizeof(hashtable));
    if (table == NULL) {
        (*htable) = NULL;
        return;
    }
    table->length = 0;
    table->capacity = INITIAL_CAPACITY;

    table->entries = calloc(table->capacity, sizeof(ht_entry));
    if (table->entries == NULL) {
        free(table);
        (*htable) = NULL;
        return;
    }
    (*htable) = table;
}

void ht_destroy(ht* htable) {

    for (size_t i = 0; i < (*htable)->capacity; i++) {
        if ((*htable)->entries[i].key != NULL) {
            free((void*)(*htable)->entries[i].key);
        }
    }

    free((*htable)->entries);
    free((*htable));
    (*htable) = NULL;
}

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

// https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
static uint64_t hash_key(const char* key) {
    uint64_t hash = FNV_OFFSET;
    for (const char* p = key; *p; p++) {
        hash ^= (uint64_t)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }
    return hash;
}

void* ht_get(ht* htable, const char* key) {
    uint64_t hash = hash_key(key);
    size_t index = (size_t)(hash & (uint64_t)((*htable)->capacity - 1));

    while ((*htable)->entries[index].key != NULL) {
        if (strcmp(key, (*htable)->entries[index].key) == 0) {
            return (*htable)->entries[index].value;
        }
        index++;
        if (index >= (*htable)->capacity) {
            index = 0;
        }
    }
    return NULL;
}

// Internal function to set an entry (without expanding table).
void ht_set_entry(ht_entry* entries, size_t capacity,
       char* key, void* value, size_t* plength, int input_type, int lenvariable) {
    uint64_t hash = hash_key(key);
    size_t index = (size_t)(hash & (uint64_t)(capacity - 1));
    while (entries[index].key != NULL) {
        if (strcmp(key, entries[index].key) == 0) {
            if (input_type == INT_INPUT){
                *(int *)(entries[index].value) += *(int *)(value);
            }else{
                free(entries[index].value);
                entries[index].value = NULL;
                entries[index].value = (void *)malloc(sizeof(char)*(int)strlen((char *)value));
                if (lenvariable == 0){
                    strncpy((char *)entries[index].value, (char *)value, input_type);
                }else{
                    strcpy((char *)entries[index].value, (char *)value);
                }
            }
            return;
        }
        index++;
        if (index >= capacity) {
            index = 0;
        }
    }

    if (plength != NULL) {
        key = strdup(key);
        if (key == NULL) {
            return ;
        }
        (*plength)++;
    }
    
    if (lenvariable == 0){
        entries[index].key = (char *)(malloc(sizeof(char)*strlen((char *)key)));
        strcpy(entries[index].key, (char *)key);
    }else{
        // printf("%d\n", input_type);
        // printf("%s - %lu\n",key,strlen(key));
        entries[index].key = (char *)(malloc(sizeof(char)*input_type));
        int i = 0;
        for (i = 0 ; i < input_type ; i++){
            entries[index].key[i] = key[i];
            // printf("%lu\n", strlen(entries[index].key));
        }
        //printf("%s : ",entries[index].key);
    }
    if (input_type == INT_INPUT){
        entries[index].value = (void *)malloc(sizeof(int));
        *(int *)(entries[index].value) = *(int *)(value);
    }else{
        free(entries[index].value);
        entries[index].value = NULL;
        if (lenvariable == 0){
            entries[index].value = (char *)malloc(sizeof(char)*input_type);
            strncpy((char *)entries[index].value, (char *)value, input_type);
        }
        else {
            entries[index].value = (char *)malloc(sizeof(char)*strlen(value));
            //printf("%s : ",entries[index].key);
            //printf("%s  - %lu : ",(char *)entries[index].key, strlen((char *)entries[index].key));
            strcpy((char *)entries[index].value, (char *)value);
            //printf("%s : ",entries[index].key);
        }
        // if (lenvariable == 1){
        //     printf("%s : ",entries[index].key);
        //     printf("%s  - %lu\n",(char *)entries[index].value, strlen((char *)entries[index].key));
        // }
        // printf("%d\n",count);
        // printf("%s\n",(char *)value);
    }
    return;
}

// Expand hash table to twice its current size. Return true on success,
// false if out of memory.
static bool ht_expand(ht* htable, int input_type, int lenvariable) {
    // Allocate new entries array.
    size_t new_capacity = (*htable)->capacity * 2;
    if (new_capacity < (*htable)->capacity) {
        return false;  // overflow (capacity would be too big)
    }
    ht_entry* new_entries = calloc(new_capacity, sizeof(ht_entry));
    if (new_entries == NULL) {
        return false;
    }

    for (size_t i = 0; i < (*htable)->capacity; i++) {
        ht_entry entry = (*htable)->entries[i];
        if (entry.key != NULL) {
            ht_set_entry(new_entries, new_capacity, entry.key,
                         entry.value, NULL, input_type, lenvariable);
        }
    }

    free((*htable)->entries);
    (*htable)->entries = new_entries;
    (*htable)->capacity = new_capacity;
    return true;
}

void ht_set(ht* htable, char* key, void* value, int input_type, int lenvariable) {
    // if (input_type != INT_INPUT){
    //     printf("%s : ",key);
    //     printf("%s\n",(char *)value);
    // }
    assert(value != NULL);
    if (value == NULL) {
        return ;
    }

    if ((*htable)->length >= (*htable)->capacity / 2) {
        if (!ht_expand(htable,input_type, lenvariable)) {
            return ;
        }
    }

    ht_set_entry((*htable)->entries, (*htable)->capacity, key, value,
                        &(*htable)->length, input_type, lenvariable);
}

size_t ht_length(ht* htable) {
    return (*htable)->length;
}

hti ht_iterator(ht* htable) {
    hti it;
    it._table = htable;
    it._index = 0;
    return it;
}

bool ht_next(hti* it) {
    // Loop till we've hit end of entries array.
    ht* htable = it->_table;
    while (it->_index < (*htable)->capacity) {
        size_t i = it->_index;
        it->_index++;
        if ((*htable)->entries[i].key != NULL) {
            // Found next non-empty item, update iterator key and value.
            ht_entry entry = (*htable)->entries[i];
            it->key = entry.key;
            it->value = entry.value;
            return true;
        }
    }
    return false;
}