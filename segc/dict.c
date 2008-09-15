#include <stdint.h>
#include <string.h>

#include "dict.h"
#include "word.h"

#ifdef AVLTREE
#include "avltree.h"

static avltree_t* root;
#else
#include "hash.h"

static hash_table_t* root;
#endif

void
dict_init()
{
#ifdef AVLTREE
    root = NULL;
#else
    root = hash_table_new();
#endif
}

void
dict_load_words(const char* filename)
{
    FILE* fp = fopen(filename, "r");
    if(fp)
    {
        word_t w;
        while(1)
        {
            if(fscanf(fp, "%s", w) < 0)
                break;
#ifdef AVLTREE
            root = avltree_insert(root, w, make_word_info(w, 0));
#else
            hash_table_insert(root, w, make_word_info(w, 0));
#endif
        }
        fclose(fp);
    }
}

void
dict_load_chars(const char* filename)
{
    FILE* fp = fopen(filename, "r");
    if(fp)
    {
        word_t w;
        int freq;
        while(1)
        {
            if(fscanf(fp, "%s %d", w, &freq) < 0)
                break;
#ifdef AVLTREE
            root = avltree_insert(root, w, make_word_info(w, freq));
#else
            hash_table_insert(root, w, make_word_info(w, 0));
#endif
        }
        fclose(fp);
    }
    
}

void
dict_deinit()
{
#ifdef AVLTREE
    avltree_destroy(root);
#else
    hash_table_destroy(root);
#endif
    root = NULL;
}

word_info_t*
dict_search_word_info(const char* text,
                      size_t      tsize)
{
#ifdef AVLTREE
    char* buf = (char*)text;
    char ch = buf[tsize];
    buf[tsize] = 0;
    word_info_t* ret = (word_info_t*)avltree_search(root, text);
    buf[tsize] = ch;
#else
    word_info_t* ret = (word_info_t*)hash_table_search(root, text, tsize);
#endif
    return ret;
}
