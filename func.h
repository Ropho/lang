#ifndef FUNC_H
#define FUNC_H

#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include "libr\libr_tree_diff.h"


// enum TYPE {

//     NUMBER = 1,
    
//     OPERATOR = 2,

//     PARANTHESIS = 3,

//     KILLER = 4,

//     VAR = 5,
// };


// enum OPERATORS {

//     PLUS = 1,

//     MINUS = 2,

//     MULT = 3,

//     DEL = 4,


// };

#define SYNTAX_ERROR {printf("ABORT HERE: %d\n", __LINE__); \
                     abort ();}                        


size_t find_file_size (FILE *in);

char* fill_text (FILE* in, size_t file_size);



node *G     (node **tree, size_t *index, node *head);
node *S     (node **tree, size_t *index, node *head);
node *E     (node **tree, size_t *index, node *head);
node *T     (node **tree, size_t *index, node *head);
node *PW    (node **tree, size_t *index, node *head);
node *P     (node **tree, size_t *index, node *head);
node *FUNC  (node **tree, size_t *index, node *head);
node *SRAV (node **tree, size_t *index, node *head);
node *ID    (node **tree, size_t *index, node *head);
node *N     (node **tree, size_t *index, node *head);


void in_order_graph (node *node, FILE *out);
size_t token_ctor (node *head, char *s, size_t size);
void skip_symbols (char **s);
void graph (node* head);

bool check_func (int a);
bool check_keywords (int a);
bool check_operations (int a);
#endif