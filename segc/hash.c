#include <string.h>

#include "mempool.h"
#include "hash.h"
#include "word.h"

#define INIT_SIZE 262147
#define MAX_DENSITY 5

static size_t primes[] =
{
    524288 + 21,
    1048576 + 7,
    2097152 + 17,
    4194304 + 15,
    8388608 + 9,
    16777216 + 43,
    33554432 + 35,
    67108864 + 15,
    134217728 + 29,
    268435456 + 3,
    536870912 + 11,
    1073741824 + 85,
};

typedef struct _hash_entry_t hash_entry_t;

struct _hash_entry_t
{
    char          key[64];
    void*         data;
    hash_entry_t* next;
};

struct _hash_table_t
{
    hash_entry_t** arr;
    size_t         capacity;
    size_t         size;
};

static int
get_new_size(size_t size)
{
    int i;
    for(i = 0; i < sizeof(primes) / sizeof(size_t); i++)
        if(primes[i] > size)
            return primes[i];
    return size;
}

static inline size_t
hash(const char* str,
     size_t      len)
{
    size_t key = 0;
    while(len--)
    {
        key += *(str++);
        key += (key << 10);
        key ^= (key >> 6);
    }
    key += (key << 3);
    key ^= (key >> 11);
    key += (key << 15);
    return key;
}

static void
rehash(hash_table_t* table)
{
    size_t newsize = get_new_size(table->capacity);
    hash_entry_t** newt = malloc(sizeof(hash_entry_t*) * newsize);
    memset(newt, 0, sizeof(hash_entry_t*) * newsize);
    hash_entry_t* entry, *next;
    int i;
    for(i = 0; i < table->capacity; i++)
    {
        entry = table->arr[i];
        while(entry)
        {
            int hash_val = hash(entry->key, strlen(entry->key));
            hash_val %= newsize;
            next = entry->next;
            entry->next = newt[hash_val];
            newt[hash_val] = entry;
            entry = next;
        }
    }
    free(table->arr);
    table->arr = newt;
    table->capacity = newsize;
}

hash_table_t*
hash_table_new()
{
    hash_table_t* table = malloc(sizeof(hash_table_t));
    table->arr = malloc(sizeof(hash_entry_t*) * INIT_SIZE);
    memset(table->arr, 0, sizeof(hash_entry_t*) * INIT_SIZE);
    table->capacity = INIT_SIZE;
    table->size = 0;
    return table;
}

void
hash_table_insert(hash_table_t* table,
                  const char*   w,
                  void*         data)
{
    table->size++;
    if(table->capacity / table->size < MAX_DENSITY)
        rehash(table);
    int val = hash(w, strlen(w)) % table->capacity;
    hash_entry_t* ent = table->arr[val];
    if(!ent)
    {
        table->arr[val] = pool_alloc(sizeof(hash_entry_t));
        table->arr[val]->next = 0;
        table->arr[val]->data = data;
        strcpy(table->arr[val]->key, w);
    }
    else
    {
        while(ent->next)
            ent = ent->next;
        ent->next = pool_alloc(sizeof(hash_entry_t));
        ent->next->next = 0;
        ent->next->data = data;
        strcpy(ent->next->key, w);
    }
}

void*
hash_table_search(hash_table_t* table,
            const char*   w,
            size_t        wlen)
{
    int val = hash(w, wlen) % table->capacity;
    hash_entry_t* ent = table->arr[val];

    while(ent)
    {
        if(strncmp(ent->key, w, wlen) == 0)
            return ent->data;
        ent = ent->next;
    }
    return NULL;
}

void
hash_table_destroy(hash_table_t* table)
{
    int i;
    for(i = 0; i < table->capacity; i++)
    {
        hash_entry_t* ent = table->arr[i];
        while(ent)
        {
            hash_entry_t* tmp = ent->next;
            pool_free(ent);
            ent = tmp;
        }
    }
    free(table->arr);
    free(table);
}
