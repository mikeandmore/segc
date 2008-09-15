#include <string.h>

#include "avltree.h"
#include "mempool.h"

typedef struct _avltree_node_t avltree_node_t;

struct _avltree_node_t
{
    char* key;
    void* value;
    avltree_node_t* left;
    avltree_node_t* right;
    avltree_node_t* parent;
    int height;
};

static inline int
recompute_height(avltree_node_t* node)
{
    int lh = node->left ? node->left->height : 0;
    int rh = node->right ? node->right->height : 0;

    return lh > rh ? lh + 1: rh + 1;
}

static inline size_t
recalculate_size(avltree_node_t* node)
{
    if(!node)
        return 0;

    size_t sz = 0;
    if(node->left)
        sz += recalculate_size(node->left);
    if(node->right)
        sz += recalculate_size(node->right);

    return sz + 1;
}

static inline avltree_node_t*
update_parent(avltree_node_t* parent,
              avltree_node_t* orig_v,
              avltree_node_t* new_v)
{
    if(parent)
    {
        if(parent->left == orig_v)
            parent->left = new_v;
        else
            parent->right = new_v;
    }
    return parent;
}


static avltree_node_t*
single_rotate_left(avltree_node_t* node)
{
    avltree_node_t* parent = node->parent;
    avltree_node_t* lchild = node->left;

    if((node->left = lchild->right))
        node->left->parent = node;
    
    lchild->right = node;
    node->parent = lchild;
    
    lchild->parent = parent;

    //recompute height
    node->height = recompute_height(node);
    lchild->height = recompute_height(lchild);
    
    parent = update_parent(parent, node, lchild);
    
    return lchild;
}

static avltree_node_t*
single_rotate_right(avltree_node_t* node)
{
    avltree_node_t* parent = node->parent;
    avltree_node_t* rchild = node->right;

    if((node->right = rchild->left))
        node->right->parent = node;

    rchild->left = node;
    node->parent = rchild;

    rchild->parent = parent;

    //recompute height
    node->height = recompute_height(node);
    rchild->height = recompute_height(rchild);
    
    parent = update_parent(parent, node, rchild);
    
    return rchild;    
}

static avltree_node_t*
double_rotate_left(avltree_node_t* node)
{
    avltree_node_t* parent  = node->parent;
    avltree_node_t* lchild  = node->left;
    avltree_node_t* lrchild = node->left->right;

    if((node->left = lrchild->right))
        node->left->parent = node;

    if((lchild->right = lrchild->left))
        lchild->right->parent = lchild;

    lrchild->left = lchild;
    lchild->parent = lrchild;
    lrchild->right = node;
    node->parent = lrchild;

    lrchild->parent = parent;

    //recompute height
    lchild->height = recompute_height(lchild);
    node->height = recompute_height(node);
    lrchild->height = recompute_height(lrchild);
    
    parent = update_parent(parent, node, lrchild);
    return lrchild;        
}

static avltree_node_t*
double_rotate_right(avltree_node_t* node)
{
    avltree_node_t* parent  = node->parent;
    avltree_node_t* rchild  = node->right;
    avltree_node_t* rlchild = node->right->left;

    if((node->right = rlchild->left))
        node->right->parent = node;

    if((rchild->left = rlchild->right))
        rchild->left->parent = rchild;

    rlchild->right = rchild;
    rchild->parent = rlchild;
    rlchild->left = node;
    node->parent = rlchild;

    rlchild->parent = parent;

    //recompute height
    rchild->height = recompute_height(rchild);
    node->height = recompute_height(node);
    rlchild->height = recompute_height(rlchild);

    parent = update_parent(parent, node, rlchild);
    return rlchild;
}


static avltree_node_t*
avl_tree_maintain(avltree_node_t* node)
{
    //maintain heighth
    int lh = node->left ? node->left->height : 0;
    int rh = node->right ? node->right->height : 0;
    if(lh - rh > 1)//need left rotate
    {
        int llh = node->left->left ? node->left->left->height : 0;
        int lrh = node->left->right ? node->left->right->height : 0;
        if(llh >= lrh)
            node = single_rotate_left(node);
        else
            node = double_rotate_left(node);
    }
    else if(lh - rh < -1)//need right rotate
    {
        int rrh = node->right->right ? node->right->right->height : 0;
        int rlh = node->right->left ? node->right->left->height : 0;
        if(rrh >= rlh)
            node = single_rotate_right(node);
        else
            node = double_rotate_right(node);
    }
    //recompute the heighth
    node->height = recompute_height(node);
    return node;
}

static inline avltree_node_t*
avl_tree_insert(avltree_node_t* node,
                avltree_node_t* parent,
                char*           key,
                void*           value)
{
    if(node == NULL)
    {
        node = pool_alloc(sizeof(avltree_node_t));
        node->key = pool_alloc(sizeof(char) * (strlen(key) + 1));
        strcpy(node->key, key);
        node->value = value;
        node->left = node->right = NULL; node->parent = parent;
        //set the default height
        node->height = 1;
        //printf("new node %s\n", key);
        return node;
    }

    //printf("%s %s %p %p %p %p %p\n", key, node->key, node, node->value, node->parent, node->left, node->right);
    int cmp = strcmp(key, node->key);
    if(cmp == 1)//key is smaller
        node->left = avl_tree_insert(node->left, node, key, value);
    else if(cmp == -1)//key is larger
        node->right = avl_tree_insert(node->right, node, key, value);
    node = avl_tree_maintain(node);

    return node;
}

void*
avltree_search(avltree_t*       root,
               const char*      key)
{
    if(!root)//null tree
        return 0;
    int c = strcmp(key, root->key);
    switch(c)
    {
    case 1://key is larger
        return avltree_search(root->left, key);
        break;
    case -1:
        return avltree_search(root->right, key);
        break;
    case 0:
        return root->value;
        break;
    }
    return NULL;
}

avltree_t*
avltree_insert(avltree_t* tree,
               char*      str,
               void*      value)
{
    return avl_tree_insert(tree, NULL, str, value);
}

void
avltree_destroy(avltree_t* tree)
{
    if(!tree)
        return;
    if(tree->left)
        avltree_destroy(tree->left);
    if(tree->right)
        avltree_destroy(tree->right);
    pool_free(tree->key);
    pool_free(tree->value);
    pool_free(tree);
}
