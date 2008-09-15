#include <string.h>

#include "word.h"
#include "mempool.h"

size_t
utf8_char_len(unsigned char ch)
{
    if(ch >= 0xC0 && ch <= 0xDF)
        return 2;
    if(ch >= 0xE0 && ch <= 0xEF)
        return 3;
    if(ch == 0)
        return 0;
    return 1;
}

size_t
utf8_len(const char* text)
{
    size_t len = 0;
    while(1)
    {
        char ch = (*text);
        if(ch == 0)
            break;
        text += utf8_char_len(ch);
        len++;
    }
    return len;
}

word_info_t*
make_word_info(const char* text,
               int         freq)
{
    size_t len = strlen(text);
    word_info_t* w = pool_alloc(sizeof(word_info_t));
    w->nbytes = len;
    w->freq = freq;
    w->length = utf8_len(text);
    return w;
}

int
is_chinese_punc(char* text)
{
    static char* ch_punc[] = {"，", "。", "：", "“", "”", "；", "-", "、", "《", "》"};
    static size_t ch_punc_size = sizeof(ch_punc) / sizeof(char*);

    int i;
    for(i = 0; i < ch_punc_size; i++)
        if(strcmp(text, ch_punc[i]) == 0)
            return 1;
    return 0;   
}
