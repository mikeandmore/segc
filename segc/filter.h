/**
 * @file filter.h
 * implement all kinds of filter to filter the chunk array
 * @author Mike
 */
#ifndef _FILTER_H_
#define _FILTER_H_

#include "chunk.h"
#include "word.h"

typedef void (*FilterFunc) (chunk_t*  chunkarr[],
                            size_t*   nsize);

void  mm_filter    (chunk_t* chunkarr[],
                    size_t*  nsize);

void  lawl_filter  (chunk_t* chunkarr[],
                    size_t*  nsize);

void  svw_filter   (chunk_t* chunkarr[],
                    size_t*  nsize);

void  ldmf_filter  (chunk_t* chunkarr[],
                    size_t*  nsize);

#endif /* _FILTER_H_ */
