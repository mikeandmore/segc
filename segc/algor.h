#ifndef _ALGOR_H_
#define _ALGOR_H_

#include "dict.h"
#include "word.h"

#define MAX_CHUNK_NUM 32

typedef struct _algor_t algor_t;

algor_t*       algor_new        (const char* target_string);
void           algor_destroy    (algor_t*    al);

size_t         algor_get_next_token(algor_t* al);

#endif /* _ALGOR_H_ */
