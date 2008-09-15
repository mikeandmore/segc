#ifndef _HASH_H_
#define _HASH_H_

typedef struct _hash_table_t hash_table_t;

hash_table_t*    hash_table_new        ();

void             hash_table_destroy    (hash_table_t* table);

void             hash_table_insert     (hash_table_t* table,
                                        const char*   w,
                                        void*         data);

void*            hash_table_search     (hash_table_t* table,
                                        const char*   w,
                                        size_t        wlen);


#endif /* _HASH_H_ */
