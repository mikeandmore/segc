/**
 * @file mempool.h
 * a memory pool for allocating objects
 * @author Mike
 */

#ifndef _MEMPOOL_H_
#define _MEMPOOL_H_

#include <stdint.h>
#include <stdlib.h>

#define THRESHOULD_SIZE 512

void         pool_init   (void);
void         pool_deinit (void);

void*      pool_alloc  (size_t size);
void         pool_free   (void* ptr);

#endif /* _MEMPOOL_H_ */
