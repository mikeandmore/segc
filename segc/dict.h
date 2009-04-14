#ifndef _DICT_H_
#define _DICT_H_

#include "word.h"

void      dict_init      (void);
void      dict_deinit    (void);

void      dict_load_words     (const char* filename);
void      dict_load_chars     (const char* filename);

word_info_t*  dict_search_word_info(const char*   text,
                                    size_t        tsize);
#endif /* _DICT_H_ */
