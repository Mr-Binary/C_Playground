#ifndef RBT_H_
#define RBT_H_

typedef enum {RED,BLACK} rbt_colour;
typedef struct rbt_node *rbt;
/*
bst b = bst_new();
struct bstnode *b = bst_new();
*/
extern rbt rbt_delete(rbt b, char *str);
extern rbt rbt_free(rbt b);
extern void rbt_inorder(rbt b, void f(char *str));
extern rbt rbt_insert(rbt b, char *str);
extern rbt rbt_new();
extern void rbt_preorder(rbt b, void f(char *str));
extern int rbt_search(rbt b, char *str);

#endif
