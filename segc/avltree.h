#ifndef _AVLTREE_H_
#define _AVLTREE_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct _avltree_node_t avltree_t;

avltree_t*       avltree_insert     (avltree_t*       tree,
                                     char*            str,
                                     void*            value);

void*            avltree_search     (avltree_t*       tree,
                                     const char*      str);

void             avltree_destroy    (avltree_t*       tree);

#endif /* _AVLTREE_H_ */

