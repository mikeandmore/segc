#ifndef _WORD_H_
#define _WORD_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_WORD_LEN 4

typedef char word_t[64];
typedef struct _word_info_t word_info_t;

struct _word_info_t
{
    uint8_t nbytes; /* number of bytes */
    uint8_t length; /* number of chinese chars */
    int     freq;   /* freq of the word */
};

word_info_t*   make_word_info   (const char* text,
                                 int         freq);

size_t         utf8_char_len    (unsigned char ch);

size_t         utf8_len         (const char* text);

int            is_chinese_punc  (char* text);


#endif /* _WORD_H_ */
