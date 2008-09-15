#include "chunk.h"
#include "filter.h"

#define PICK_MAX                                  \
    chunk_t** mark = chunkarr;                    \
    size_t    current_size = 0;                   \
    int i;                                        \
    for(i = 0; i < (*nsize); i++)                 \
    {                                             \
        v = fn(chunkarr[i]);                      \
        if(cmpval == v)                           \
            mark[current_size++] = chunkarr[i];   \
        else if(cmpval op v)                      \
        {                                         \
            cmpval = v;                           \
            current_size = 1;                     \
            mark[0] = chunkarr[i];                \
        }                                         \
    }                                             \
    (*nsize) = current_size;                      \


void
mm_filter(chunk_t* chunkarr[],
          size_t*  nsize)
{
    size_t v, cmpval = 0;
#undef fn
#undef op
#define fn chunk_length
#define op <
    PICK_MAX
}

void
lawl_filter(chunk_t* chunkarr[],
            size_t*  nsize)
{
    double v, cmpval = 0;
#undef fn
#undef op
#define fn chunk_average_length
#define op <
    PICK_MAX
}

void
svw_filter(chunk_t* chunkarr[],
           size_t*  nsize)
{
    double v, cmpval = 1.0 * 0x0fffffff;
#undef fn
#undef op
#define fn chunk_varience
#define op >
    PICK_MAX
}

void
ldmf_filter(chunk_t* chunkarr[],
            size_t*  nsize)
{
    size_t v, cmpval = 0;
#undef fn
#undef op
#define fn chunk_degree_of_MF
#define op <
    PICK_MAX
}

