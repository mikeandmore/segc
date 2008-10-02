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
    algor_t* algo = algor_new(buf);

    printf("read buf (leng = %d)\n", len);
    clock_t s = clock();
    size_t n = 0;
    size_t psize = 0;
    while((psize = algor_get_next_token(algo)))
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
    algor_destroy(algo);
    free(str);
    dict_deinit();
    pool_deinit();
    return 0;
}
