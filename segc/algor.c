#include <ctype.h>
#include <string.h>

#include "word.h"
#include "chunk.h"
#include "algor.h"
#include "filter.h"

#define MAX_MATCH_CACHE_SIZE 8

/* match result define */
typedef word_info_t*    match_result_t[MAX_WORD_LEN];
typedef size_t          match_result_size_t;

/* cache define */
typedef struct _cache_t cache_t;

struct _cache_t
{
    match_result_t      content;
    match_result_size_t size;
    const char*         pos_key;
};


struct _algor_t
{
    /* all the unfiltered chunks */
    chunk_t*         current_chunks[MAX_CHUNK_NUM];
    size_t           current_chunks_size;

    /* current working/filtered chunk */
    chunk_t*         current_chunk;
    int              current_token_pos;

    /* matching cache for one position of the target string */
    cache_t      match_cache[MAX_MATCH_CACHE_SIZE];
    size_t       current_cache_pos;
    
    const char*  target_string;
};

algor_t*
algor_new(const char* target_string)
{
    algor_t* al = malloc(sizeof(algor_t));
    int i;

    al->current_chunks_size = 0;
    
    for(i = 0; i < MAX_CHUNK_NUM; i++)
        al->current_chunks[i] = chunk_new();
    al->current_token_pos = -1;

    memset(al->match_cache, 0, sizeof(cache_t) * MAX_MATCH_CACHE_SIZE);
    al->current_cache_pos = -1;

    al->target_string = target_string;
    return al;
}

void
algor_destroy(algor_t* al)
{
    int i;
    for(i = 0; i < MAX_CHUNK_NUM; i++)
        chunk_free(al->current_chunks[i]);
    free(al);
}

static int
is_wide_char(char ch)
{
    return utf8_char_len(ch) > 1;
}

/* match the word start from text, store it into cache */
static size_t
match_word_to_cache(algor_t* al,
                    const char* text)
{
    int i;
    
    //first let's look them up in the cache
    for(i = 0; i < MAX_MATCH_CACHE_SIZE; i++)
        if(al->match_cache[i].pos_key == text)
            return i;
    
    /* insert the result into cache */
    
    //first got the insert pos
    al->current_cache_pos++;
    if(al->current_cache_pos == MAX_MATCH_CACHE_SIZE)
        al->current_cache_pos = 0;
    
    //then reset the cache item
    al->match_cache[al->current_cache_pos].size = 0;
    al->match_cache[al->current_cache_pos].pos_key = text;
    
    //last search winfo
    size_t text_len = 0;
    i = 0;
    while(i <= MAX_WORD_LEN)
    {
        i++;
        size_t ch_len = utf8_char_len(text[text_len]);
        
        if(ch_len == 1 || ch_len == 0)
            continue;

        text_len += ch_len;
        word_info_t* info = dict_search_word_info(text, text_len);
        //if ok, put it into cache
        //because al->match_cache[al->current_cache_pos] is our current cache item
        //so should be its .content its .size should get updated
        if(info)
            al->match_cache[al->current_cache_pos].
                content[al->match_cache[al->current_cache_pos].size++] = info;
    }
    return al->current_cache_pos;
}

/* search all chunks and push them into a list */
static void
search_chunks(algor_t*    al,
              const char* text,
              size_t      step,
              chunk_t*    ch)
{
    if(step == CHUNK_WORD_SIZE)
        goto push_cur;
    else
    {
        int i;
        if(!is_wide_char(text[0]))
            goto push_cur;

        size_t idx = match_word_to_cache(al, text);
        size_t cc_size = al->match_cache[idx].size;

        //none matched
        if(cc_size == 0)
            goto push_cur;

        match_result_t content;
        memcpy(content, al->match_cache[idx].content, sizeof(match_result_t));
        for(i = 0; i < cc_size; i++)
        {
            word_info_t* info = content[i];
            strncpy(ch->w[step], text, info->nbytes);
            ch->w[step][info->nbytes] = 0;
            ch->winfo[step] = info;
            ch->n = step + 1;
            search_chunks(al, text + info->nbytes, step + 1, ch);
        }
    }
    return;
    
  push_cur:
    memcpy(al->current_chunks[al->current_chunks_size++], ch, sizeof(chunk_t));
    return;
}

/* refresh and select current chunk */
static void
refresh_chunks(algor_t* al)
{
    const char* text = al->target_string;
    //start again
    al->current_chunks_size = 0;
    //search all the chunks
    chunk_t* tmp_ch = chunk_new();
    search_chunks(al, text, 0, tmp_ch);
    chunk_free(tmp_ch);

    chunk_t* free_list[MAX_CHUNK_NUM];
    memcpy(free_list, al->current_chunks, sizeof(chunk_t*) * MAX_CHUNK_NUM);
    
    //filter the result list
    FilterFunc farr[] = {mm_filter, lawl_filter, svw_filter, ldmf_filter};
    size_t fsz = 4;
    int i;
    for(i = 0; i < fsz; i++)
        farr[i](free_list, &al->current_chunks_size);
    al->current_chunk = free_list[0];
}

/* return a latin token */
static size_t
latin_token(const char* text)
{
    size_t i = 0;
    while(text[i])
    {
        if(!isalpha(text[i]) && !isdigit(text[i]))
            break;
        i++;
    }
    if(i == 0 && text[0])
        return 1;
    return i;
}

/* return a chinese token */
static size_t
chinese_token(algor_t* al)
{
    if(al->current_token_pos == -1)
    {
        refresh_chunks(al);
        al->current_token_pos = 0;
    }
    size_t ret = al->current_chunk->winfo[al->current_token_pos]->nbytes;
    al->current_token_pos++;

    // the last word in the chunk used up
    if(al->current_token_pos == al->current_chunk->n)
        al->current_token_pos = -1;
    return ret;
}

/* return a one unicode token, for some strange character token */
static size_t
one_unicode_token(algor_t* al)
{
    if(al->target_string[0])
        return utf8_char_len(al->target_string[0]);
    return 0;
}

static size_t
complex_algo_next_token(algor_t* al)
{
    const char* text = al->target_string;
    if(!text[0])
        return 0;
    if(text[0] == '\n')
        return 1;
    if(al->current_token_pos >= 0)
        return chinese_token(al);

    size_t len = utf8_char_len(text[0]);
    if(len == 1)
        return latin_token(al->target_string);
    
    if(dict_search_word_info(text, len))
        return chinese_token(al);
    else
        return one_unicode_token(al);
}

size_t
algor_get_next_token(algor_t* al)
{
    size_t tok = complex_algo_next_token(al);
    al->target_string += tok;
    return tok;
}
