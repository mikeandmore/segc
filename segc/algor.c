#include <ctype.h>
#include <string.h>

#include "word.h"
#include "chunk.h"
#include "algor.h"
#include "filter.h"

#define MAX_MATCH_CACHE_SIZE 8

static chunk_t*         current_chunks[MAX_CHUNK_NUM];
static size_t           current_chunks_size = 0;

static chunk_t*         current_chunk;
static int              current_token_pos = -1;

typedef word_info_t*    match_result_t[MAX_WORD_LEN];
typedef size_t          match_result_size_t;

typedef struct _cache_t cache_t;

struct _cache_t
{
    match_result_t      content;
    match_result_size_t size;
    const char*         pos_key;
};

cache_t      match_cache[MAX_MATCH_CACHE_SIZE];

/* init all static stuff */
void
complex_algo_init()
{
    int i;
    for(i = 0; i < MAX_CHUNK_NUM; i++)
        current_chunks[i] = chunk_new();
    memset(match_cache, 0, sizeof(cache_t) * MAX_MATCH_CACHE_SIZE);
}

/* free all static stuff */
void
complex_algo_deinit()
{
    int i;
    for(i = 0; i < MAX_CHUNK_NUM; i++)
        chunk_free(current_chunks[i]);
}

static int
is_wide_char(char ch)
{
    return utf8_char_len(ch) > 1;
}

/* match the word start from text, store it into cache */
static size_t
match_word_to_cache(const char* text)
{
    static size_t cache_cur_pos = -1;
    int i;
    
    //first let's look them up in the cache
    for(i = 0; i < MAX_MATCH_CACHE_SIZE; i++)
        if(match_cache[i].pos_key == text)
            return i;
    
    /* insert the result into cache */
    
    //first got the insert pos
    cache_cur_pos++;
    if(cache_cur_pos == MAX_MATCH_CACHE_SIZE)
        cache_cur_pos = 0;
    
    //then reset the cache item
    match_cache[cache_cur_pos].size = 0;
    match_cache[cache_cur_pos].pos_key = text;
    
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
        if(info)
            match_cache[cache_cur_pos].
                content[match_cache[cache_cur_pos].size++] = info;
    }
    return cache_cur_pos;
}

/* search all chunks and push them into a list */
static void
search_chunks(const char* text,
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

        size_t idx = match_word_to_cache(text);
        size_t cc_size = match_cache[idx].size;

        //none matched
        if(cc_size == 0)
            goto push_cur;

        match_result_t content;
        memcpy(content, match_cache[idx].content, sizeof(match_result_t));
        for(i = 0; i < cc_size; i++)
        {
            word_info_t* info = content[i];
            strncpy(ch->w[step], text, info->nbytes);
            ch->w[step][info->nbytes] = 0;
            ch->winfo[step] = info;
            ch->n = step + 1;
            search_chunks(text + info->nbytes, step + 1, ch);
        }
    }
    return;
  push_cur:
    memcpy(current_chunks[current_chunks_size++], ch, sizeof(chunk_t));
}

/* refresh and select current chunk */
static void
refresh_chunks(const char* text)
{
    //start again
    current_chunks_size = 0;
    //search all the chunks
    chunk_t* tmp_ch = chunk_new();
    search_chunks(text, 0, tmp_ch);
    chunk_free(tmp_ch);

    
    chunk_t* free_list[MAX_CHUNK_NUM];
    memcpy(free_list, current_chunks, sizeof(chunk_t*) * MAX_CHUNK_NUM);
    
    //filter the result list
    FilterFunc farr[] = {mm_filter, lawl_filter, svw_filter, ldmf_filter};
    size_t fsz = 4;
    int i;
    for(i = 0; i < fsz; i++)
        farr[i](free_list, &current_chunks_size);
    current_chunk = free_list[0];
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
chinese_token(const char* text)
{
    if(current_token_pos == -1)
    {
        refresh_chunks(text);
        current_token_pos = 0;
    }
    
    size_t ret = current_chunk->winfo[current_token_pos]->nbytes;
    current_token_pos++;

    if(current_token_pos == current_chunk->n) // the last word in the chunk used up
        current_token_pos = -1;
    return ret;
}

/* return a one unicode token, for some strange character token */
static size_t
one_unicode_token(const char* text)
{
    if(text[0])
        return utf8_char_len(text[0]);
    return 0;
}

size_t
complex_algo_next_token(const char* text)
{
    if(!text[0])
        return 0;
    if(text[0] == '\n')
        return 1;
    if(current_token_pos >= 0)
        return chinese_token(text);

    size_t len = utf8_char_len(text[0]);
    if(len == 1)
        return latin_token(text);
    
    if(dict_search_word_info(text, len))
        return chinese_token(text);
    else
        return one_unicode_token(text);
}
