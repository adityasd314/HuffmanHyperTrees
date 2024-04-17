#include "hashtable.h"
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 1000
int count = 0;

void hashtable_create(ht *htable)
{
    ht table = (ht)malloc(sizeof(hashtable));
    if (table == NULL)
    {
        (*htable) = NULL;
        return;
    }
    table->length = 0;
    table->capacity = INITIAL_CAPACITY;

    table->entries = calloc(table->capacity, sizeof(ht_entry));
    if (table->entries == NULL)
    {
        free(table);
        (*htable) = NULL;
        return;
    }
    (*htable) = table;
}

void ht_destroy(ht *htable)
{

    for (size_t i = 0; i < (*htable)->capacity; i++)
    {
        if ((*htable)->entries[i].key != NULL)
        {
            free((void *)(*htable)->entries[i].key);
        }
    }

    free((*htable)->entries);
    free((*htable));
    (*htable) = NULL;
}
// https://stackoverflow.com/questions/37580741/seemingly-easy-fnv1-hashing-implementation-results-in-a-lot-of-collisions
#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

// https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
// Non-cryptographic hash function used for hash table key hashing.
// This is the FNV-1a hash function.
// https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
// Advantages
// 1. Fast
// 2. Simple
// 3. Good distribution
// 4. less resourse intensive
// Disadvantages
// 1. Not cryptographic
// 2. Not collision resistan
// 3. Not avalanche effect : small changes in input result in small changes in output
// Why FNV
// why FNV might be considered suitable for certain scenarios:
// Simplicity and Speed: FNV is known for its simplicity and speed. It can quickly hash data without imposing a significant computational overhead. This makes it suitable for non-cryptographic applications where speed is essential.
// Avalanche Effect: While FNV may not offer the same level of security as cryptographic hash functions like SHA-256, it still exhibits a reasonable avalanche effect. For many non-cryptographic applications, this level of dispersion is sufficient to minimize collisions.
// Ease of Implementation: FNV is easy to implement and understand, requiring only a few lines of code. This simplicity makes it a popular choice for applications where a lightweight hash function is needed.

// Time complexity:
//   Best case: O(1)
//   Worst case: O(n)
//   Average case: O(n)
// Where 'n' is the length of the input string 'key'.
static uint64_t hash_key(const char *key)
{
    uint64_t hash = FNV_OFFSET;
    for (const char *p = key; *p; p++)
    {
        hash ^= (uint64_t)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }
    return hash;
}

// Time complexity:
//   Best case: O(1) - when the desired key is located at the calculated index.
//   Worst case: O(n) - when there are collisions and the function needs to linearly search for the key.
//   Average case: O(1 + k), where k => average number of elements per bucket.

void *ht_get(ht *htable, const char *key)
{
    uint64_t hash = hash_key(key);
    size_t index = (size_t)(hash & (uint64_t)((*htable)->capacity - 1));

    while ((*htable)->entries[index].key != NULL)
    {
        if (strcmp(key, (*htable)->entries[index].key) == 0)
        {
            return (*htable)->entries[index].value;
        }
        index++;
        if (index >= (*htable)->capacity)
        {
            index = 0;
        }
    }
    return NULL;
}

// Internal function to set an entry (without expanding table).
/*
 * Time Complexity:
 *   - Hashing Operation: O(1)
 *   - Collision Resolution: O(n) (worst case), average case depends on hash function and table size
 *   - Memory Allocation: O(1) to O(log n) (depends on allocator)
 *   - String Manipulation:
 *       - strlen: O(n)
 *       - strcpy/strncpy: O(m) // length of source string
 *   - Overall:
 *       - Best Case: O(1)
 *       - Average Case: O(1) to O(n)
 *       - Worst Case: O(n)
 */
void ht_set_entry(ht_entry *entries, size_t capacity,
                  char *key, void *value, size_t *plength, int input_type, int lenvariable){
    // len = 0 charToCode
    // len = 1 codeToChar
    uint64_t hash = hash_key(key);
    size_t index = (size_t)(hash & (uint64_t)(capacity - 1));
    while (entries[index].key != NULL)
    {
        if (strcmp(key, entries[index].key) == 0)
        {
            if (input_type == INT_INPUT)
            {
                *(int *)(entries[index].value) += *(int *)(value);
            }
            else
            {
                free(entries[index].value);
                entries[index].value = NULL;
                entries[index].value = (void *)malloc(sizeof(char) * (int)strlen((char *)value));
                if (lenvariable == 0)
                {
                    // charToCode value is a string length given by input_type
                    strncpy((char *)entries[index].value, (char *)value, input_type);
                }
                else
                {
                    // codeToChar value is a character
                    strcpy((char *)entries[index].value, (char *)value);
                }
            }
            return;
        }
        index++;
        if (index >= capacity)
        {
            index = 0;
        }
    }

    if (plength != NULL)
    {
        key = strdup(key);
        if (key == NULL)
        {
            return;
        }
        (*plength)++;
    }

    if (lenvariable == 0)
    {
        // charToCode value is a string length given by input_type key is character
        entries[index].key = (char *)(malloc(sizeof(char) * strlen((char *)key)));
        strcpy(entries[index].key, (char *)key);
    }
    else
    {
        // printf("%d\n", input_type);
        // printf("%s - %lu\n",key,strlen(key));
        // codeToChar value is a string size given by input_type key
        entries[index].key = (char *)(malloc(sizeof(char) * input_type));
        int i = 0;
        for (i = 0; i < input_type; i++)
        {
            entries[index].key[i] = key[i];
            // printf("%lu\n", strlen(entries[index].key));
        }
        // printf("%s : ",entries[index].key);
    }
    // let us set value of key
    if (input_type == INT_INPUT)
    {
        entries[index].value = (void *)malloc(sizeof(int));
        *(int *)(entries[index].value) = *(int *)(value);
    }
    else
    {
        free(entries[index].value);
        entries[index].value = NULL;
        if (lenvariable == 0)
        {
            // charToCode value is a string length given by input_type
            entries[index].value = (char *)malloc(sizeof(char) * input_type);
            strncpy((char *)entries[index].value, (char *)value, input_type);
        }

        else
        {
            // codeToChar value is a character
            entries[index].value = (char *)malloc(sizeof(char) * strlen(value));
            // printf("%s : ",entries[index].key);
            // printf("%s  - %lu : ",(char *)entries[index].key, strlen((char *)entries[index].key));
            strcpy((char *)entries[index].value, (char *)value);
            // printf("%s : ",entries[index].key);
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
/*
 * Time Complexity:
 *   - Memory Allocation: O(1), 
 *   - Loop: O(m), where m is the current capacity of the hash table.
 *   - Overall: O(m)
 */
static bool ht_expand(ht *htable, int input_type, int lenvariable)
{
    // Allocate new entries array.
    size_t new_capacity = (*htable)->capacity * 2;
    if (new_capacity < (*htable)->capacity)
    {
        return false; // overflow (capacity would be too big)
    }
    ht_entry *new_entries = calloc(new_capacity, sizeof(ht_entry));
    if (new_entries == NULL)
    {
        return false;
    }

    for (size_t i = 0; i < (*htable)->capacity; i++)
    {
        ht_entry entry = (*htable)->entries[i];
        if (entry.key != NULL)
        {
            ht_set_entry(new_entries, new_capacity, entry.key,
                         entry.value, NULL, input_type, lenvariable);
        }
    }

    free((*htable)->entries);
    (*htable)->entries = new_entries;
    (*htable)->capacity = new_capacity;
    return true;
}


void ht_set(ht *htable, char *key, void *value, int input_type, int lenvariable)
{
    // if (input_type != INT_INPUT){
    //     printf("%s : ",key);
    //     printf("%s\n",(char *)value);
    // }
    assert(value != NULL);
    if (value == NULL)
    {
        return;
    }

    if ((*htable)->length >= (*htable)->capacity / 2)
    {
        if (!ht_expand(htable, input_type, lenvariable))
        {
            return;
        }
    }

    ht_set_entry((*htable)->entries, (*htable)->capacity, key, value,
                 &(*htable)->length, input_type, lenvariable);
}

size_t ht_length(ht *htable)
{
    return (*htable)->length;
}

hti ht_iterator(ht *htable)
{
    hti it;
    it._table = htable;
    it._index = 0;
    return it;
}
/*
 * Time Complexity:
 *   - Best Case: O(1) - Occurs when the next non-empty item is found immediately.
 *   - Average Case: O(n/2) - Iterates over approximately half of the entries.
 *   - Worst Case: O(n) - Occurs when the next non-empty item is found at the end or not found.
 */
bool ht_next(hti *it)
{
    // Loop till we've hit end of entries array.
    ht *htable = it->_table;
    while (it->_index < (*htable)->capacity)
    {
        size_t i = it->_index;
        it->_index++;
        if ((*htable)->entries[i].key != NULL)
        {
            // Found next non-empty item, update iterator key and value.
            ht_entry entry = (*htable)->entries[i];
            it->key = entry.key;
            it->value = entry.value;
            return true;
        }
    }
    return false;
}