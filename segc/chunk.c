#include <string.h>

#include "mempool.h"
#include "chunk.h"

chunk_t*
chunk_new_arr(word_t       w[],
              word_info_t* winfo[],
              size_t       n)
{
    //use malloc so that thread safe
    chunk_t* ch = malloc(sizeof(chunk_t));
    int i;
    ch->n = n;
    for(i = 0; i < n; i++)
    {
        strncpy(ch->w[i], w[i], sizeof(word_t));
        ch->winfo[i] = winfo[i];
    }
    return ch;
}

chunk_t*
chunk_new()
{
    chunk_t* ch = malloc(sizeof(chunk_t));
    //memset(ch, 0, sizeof(chunk_t));
    return ch;
}

void
chunk_free(chunk_t* ch)
{
    free(ch);
}

size_t
chunk_length(chunk_t* ch)
{
    int i;
    size_t len = 0;
    for(i = 0; i < ch->n; i++)
        len += ch->winfo[i]->length;
    return len;
}

double
chunk_average_length(chunk_t* ch)
{
    return 1.0 * chunk_length(ch) / ch->n;
}

double
chunk_varience(chunk_t* ch)
{
    double avg = chunk_average_length(ch);
    int i;
    double ret = 0.0;
    for(i = 0; i < ch->n; i++)
        ret += (ch->winfo[i]->length - avg) *
            (ch->winfo[i]->length - avg);
    return ret;
}

size_t
chunk_degree_of_MF(chunk_t* ch)
{
    size_t mf = 0;
    int i;
    for(i = 0; i < ch->n; i++)
        mf += ch->winfo[i]->freq;
    return mf;
}

