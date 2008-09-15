#include <stdio.h>
#include <time.h>
#include <string.h>

#include "dict.h"
#include "mempool.h"
#include "chunk.h"
#include "filter.h"
#include "algor.h"

int
main()
{
    pool_init();
    dict_init();
    dict_load_words("../dics/words.dic");
    dict_load_chars("../dics/chars.dic");

#if 0
    word_t wa1[2] = {"你好", "世界"};
    word_t wa2[2] = {"你好世", "界"};
    word_info_t* info1[3];
    word_info_t* info2[3];

    int i;
    for(i = 0; i < 2; i++)
    {
        info1[i] = make_word_info(wa1[i], 0);
        info2[i] = make_word_info(wa2[i], 0);
    }
    info2[1]->freq = 100;
 
    chunk_t* ch1 = chunk_new(wa1, info1, 2);
    chunk_t* ch2 = chunk_new(wa2, info2, 2);
    chunk_t* charr[2] = {ch1, ch2};
    size_t n = 2;
    ldmf_filter(charr, &n);
    printf("%d\n", n);
    chunk_free(ch1);
    chunk_free(ch2);
#endif

    FILE* fp = fopen("input.txt", "r");
    if(!fp)
    {
        printf("Can't open testcase\n");
        return -1;
    }
    size_t buflen = 25000000;
    char* str = malloc(buflen);
    char* buf = str;
    int len = fread(buf, 1, buflen, fp);
    buf[len] = 0;
    complex_algo_init();

    printf("read buf (leng = %d)\n", len);
    clock_t s = clock();
    size_t n = 0;
    size_t psize = 0;
    while((psize = complex_algo_next_token(buf)))
    {
        /* char str[psize + 1]; */
        /* strncpy(str, buf, psize); */
        /* str[psize] = 0; */
        /* printf("%s %d\n", str, psize); */
        buf += psize;
        n++;
    }
    printf("finished %d words in %ld ms\n", n, (clock() - s) / 1000);
    printf("%d\n", buf - str);
    
    free(str);
    complex_algo_deinit();
    dict_deinit();
    pool_deinit();
    return 0;
}
