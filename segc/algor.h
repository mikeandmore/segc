#ifndef _ALGOR_H_
#define _ALGOR_H_

#include "dict.h"
#include "word.h"

#define MAX_CHUNK_NUM 32

void    complex_algo_init      ();

void    complex_algo_deinit    ();

size_t  complex_algo_next_token(const char* text);

#endif /* _ALGOR_H_ */
