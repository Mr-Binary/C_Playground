#ifndef BST_H_
#define BST_H_

/*DON'T NEED TO CODE THIS*/

typedef struct bstnode *bst;
/*
bst b = bst_new();
struct bstnode *b = bst_new();
*/
extern bst bst_free(bst b);
extern void bst_inorder(bst b, void f(char *str));
extern bst bst_insert(bst b, char *str);
extern bst bst_new();
extern void bst_preorder(bst b, void f(char *str));
extern int bst_search(bst b, char *str);

#endif
