#include "mempool.h"

void*    _pool_mem;
size_t   _pool_size;
size_t   _pool_max_size;

void*
pool_alloc(size_t size)
{
    if(size < THRESHOULD_SIZE &&
       size < _pool_max_size - _pool_size)
    {
        void* ret = _pool_mem + _pool_size;
        _pool_size += size;
        return ret;
    }
    return malloc(size);
}


void
pool_free(void* ptr)
{
    if(ptr < _pool_mem ||
       ptr >= _pool_mem + _pool_max_size)
        free(ptr);
}

void
pool_init()
{
    _pool_max_size = 2097152;//200k
    _pool_mem = malloc(_pool_max_size);
    _pool_size = 0;
}

void
pool_deinit()
{
    free(_pool_mem);
    _pool_size = 0;
}
