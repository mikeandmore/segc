#ifndef _CHUNK_H_
#define _CHUNK_H_

#include "word.h"

#define CHUNK_WORD_SIZE 3

typedef struct _chunk_t chunk_t;

struct _chunk_t
{
    word_t        w[CHUNK_WORD_SIZE]; /* words array */
    word_info_t*  winfo[CHUNK_WORD_SIZE];
    size_t        n; /* how many words are here */
};

chunk_t*  chunk_new_arr          (word_t       w[],
                                  word_info_t* winfo[],
                                  size_t       n);

chunk_t*  chunk_new              ();

void      chunk_free             (chunk_t* ch);

size_t    chunk_length           (chunk_t* ch);
double    chunk_average_length   (chunk_t* ch);
double    chunk_varience         (chunk_t* ch);
size_t    chunk_degree_of_MF     (chunk_t* ch);

#endif /* _CHUNK_H_ */
