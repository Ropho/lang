#ifndef FUNC_H
#define FUNC_H

#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include "libr\libr_tree_diff.h"


#define RULE(rule)    if (*tree == nullptr)                                     \
                        (*tree) = rule (tree, index, head);                        \
                    else if ((*tree)->left == nullptr)                          \
                        (*tree)->left = rule (&(*tree)->left, index, head);        \
                    else if ((*tree)->right == nullptr)                         \
                        (*tree)->right = rule (&(*tree)->right, index, head);      \
                    else                                                        \
                        puts ("TILT")                                     


#ifndef TYPE 
#define TYPE
typedef struct node {

    int data = 0;

    char type = 0;

    node *left = nullptr;

    node *right = nullptr;

}node;
#endif


// enum TYPE {

//     NUMBER = 1,
    
//     OPERATOR = 2,

//     PARANTHESIS = 3,

//     KILLER = 4,

//     VAR = 5,
// };


enum OPERATORS {

    PLUS = 1,

    MINUS = 2,

    MULT = 3,

    DEL = 4,


};

#define SYNTAX_ERROR printf("ABORT HERE: %d\n", __LINE__); \
                     abort ();                        


size_t find_file_size (FILE *in);

char* fill_text (FILE* in, size_t file_size);



node *G (node **tree, size_t *index, node *head);
node *E (node **tree, size_t *index, node *head);
node *T (node **tree, size_t *index, node *head);
node* PW (node **tree, size_t *index, node *head);
node *P (node **tree, size_t *index, node *head);
node *ID (node **tree, size_t *index, node *head);
node *N (node **tree, size_t *index, node *head);


static void in_order_graph (node *node, FILE *out);

void graph (node* head);

#endif