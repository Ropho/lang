#include "func.h"

extern char *s;


node* P (node **tree, size_t *index, node *head) {

    if (head[*index].type == PARANTHESIS && head[*index].data == '(') {
        
        ++*index;
        
        RULE(E);
        
        if (head[*index].type == PARANTHESIS && head[*index].data == ')') {
            ++*index;
            return *tree;
        }
        else 
            SYNTAX_ERROR
    }
    else if (head[*index].type == NUMBER) {
        RULE(N);
        ++*index;
        return *tree;
    }
    else if (head[*index].type == VARIABLE) {
        RULE(ID);
        ++*index;
        return *tree;
    }
    else {
        puts ("ERROR");
        abort();
    }
}


node *ID (node **tree, size_t *index, node *head) {

    if (head[*index].type == VARIABLE) 
        printf ("ID: %d\n", *index);
    else {
        printf ("ID abort: %d\n", *index);
        SYNTAX_ERROR;
    }

    return &head[*index];
}

node* PW (node **tree, size_t *index, node *head) {

    printf ("T: %d\n", *index);

    RULE(P);

    // ++*index;

    while (head[*index].type == OPERATION && head[*index].data == '^') {
        
        node *tmp = *tree;
        *tree = &head[*index];
        
        if ((*tree)->left == nullptr)
            (*tree)->left = tmp;
        else if ((*tree)->right == nullptr)
            (*tree)->right = tmp;

        ++*index;
        
        if ((*tree)->left == nullptr)
            (*tree)->left = P (&(*tree)->left, index, head);
        else if ((*tree)->right == nullptr)
            (*tree)->right = P (&(*tree)->right, index, head);

        // ++*index;
    }
    
    //??
    return *tree;
}


node* T (node **tree, size_t *index, node *head) {

    printf ("T: %d\n", *index);

    RULE(PW);

    // ++*index;

    while (head[*index].type == OPERATION && (head[*index].data == '*' || head[*index].data == '/')) {
        
        node *tmp = *tree;
        *tree = &head[*index];
        
        if ((*tree)->left == nullptr)
            (*tree)->left = tmp;
        else if ((*tree)->right == nullptr)
            (*tree)->right = tmp;

        ++*index;
        
        if ((*tree)->left == nullptr)
            (*tree)->left = PW (&(*tree)->left, index, head);
        else if ((*tree)->right == nullptr)
            (*tree)->right = PW (&(*tree)->right, index, head);

        // ++*index;
    }
    
    //??
    return *tree;
}


node *N (node **tree, size_t *index, node *head) {

    if (head[*index].type == NUMBER) 
        printf ("N: %d\n", *index);
    else {
        printf ("N abort: %d\n", *index);
        SYNTAX_ERROR;
    }

    return &head[*index];
}


node* E (node **tree, size_t *index, node *head) {

    if (*tree == nullptr)
        (*tree) = T (tree, index, head);
    else if ((*tree)->left == nullptr)
        (*tree)->left = T (&(*tree)->left, index, head);
    else if ((*tree)->right == nullptr)
        (*tree)->right = T (&(*tree)->right, index, head);
    else 
        puts ("TILT 127");

    // ++*index;

    while (head[*index].type == OPERATION && (head[*index].data == '+' || head[*index].data == '-')) {
    printf ("E: %d\n", *index);

        node *tmp = *tree;
        *tree = &head[*index];
        
        if ((*tree)->left == nullptr)
            (*tree)->left = tmp;
        else if ((*tree)->right == nullptr)
            (*tree)->right = tmp;

        ++*index;
        
        if ((*tree)->left == nullptr)
            (*tree)->left = T (&(*tree)->left, index, head);
        else if ((*tree)->right == nullptr)
            (*tree)->right = T (&(*tree)->right, index, head);
        else {
            printf ("%d", __LINE__);
            abort();
        }
        // ++*index;
    }
    
    //??
    return *tree;
}

node *G (node **tree, size_t *index, node *head) {

    node* nd = E (tree, index, head);

    if (head[*index].data == '$')
        return nd;
    else {
        printf ("G abort: %d\n", *index);
        SYNTAX_ERROR;
    }
}


size_t find_file_size (FILE *in) {

    assert (in != nullptr);

    int size_of_file = 0;

    fseek (in, 0, SEEK_END);

    size_of_file = ftell (in);

    fseek (in, 0, SEEK_SET);

    return size_of_file;
}

void graph (node* head) {

    assert (head != nullptr);

    FILE *out = fopen ("demo.txt", "w");
    assert (out != nullptr);

    fputs ("digraph structs {\n", out);
    in_order_graph (head, out);
    fputs ("}", out);

    fclose (out);

    system (".\\graphviz\\bin\\dot -Tpng demo.txt -o graph.png");

    system ("graph.png");


    return;
}


static void in_order_graph (node *node, FILE *out) {
    
    assert (node != nullptr);
    assert (out  != nullptr);

        if (node->type == NUMBER) {
            fprintf (out, "%d [shape=oval];\n", node);
            fprintf (out, "%d [style=filled,color=\"hotpink\"];\n", node);
            fprintf (out, "%d [label=\"%d\"]\n", node, node->data);
        }
        else if (node->type == OPERATION) {
            fprintf (out, "%d [shape=record];\n", node);
            fprintf (out, "%d [style=filled,color=\"green\"];\n", node);
            fprintf (out, "%d [label=\"%c\"]\n", node, node->data);
        }
        else if (node->type == VARIABLE) {
            fprintf (out, "%d [shape=diamond];\n", node);
            fprintf (out, "%d [style=filled,color=\"cyan2\"];\n", node);
            fprintf (out, "%d [label=\"%c\"]\n", node, node->data);
        }
        else if (node->type == FUNCTION_NAME) {
            fprintf (out, "%d [shape=star];\n", node);
            fprintf (out, "%d [style=filled,color=\"red\"];\n", node);
            fprintf (out, "%d [label=\"%d\"]\n", node, node->data);
        }
        else {
            fprintf (out, "%d [shape=oval];\n", node);
            fprintf (out, "%d [style=filled,color=\"hotpink\"];\n", node);
            fprintf (out, "%d [label=\"%c\"]\n", node, node->data); 
        }

    if (node->left != nullptr) {
        fprintf (out, "\"%d\" -> \"%d\";\n", node, node->left);

        if (node->left->type == NUMBER) {
            fprintf (out, "%d [shape=oval];\n", node->left);
            fprintf (out, "%d [style=filled,color=\"hotpink\"];\n", node->left);
            fprintf (out, "%d [label=\"%d\"]\n", node->left, node->left->data);
        }
        else if (node->left->type == OPERATION) {
            fprintf (out, "%d [shape=record];\n", node->left);
            fprintf (out, "%d [style=filled,color=\"green\"];\n", node->left);
            fprintf (out, "%d [label=\"%c\"]\n", node->left, node->left->data);
        }
        else if (node->left->type == VARIABLE) {
            fprintf (out, "%d [shape=diamond];\n", node->left);
            fprintf (out, "%d [style=filled,color=\"cyan2\"];\n", node->left);
            fprintf (out, "%d [label=\"%c\"]\n", node->left, node->left->data);
        }
        else if (node->left->type == FUNCTION_NAME) {
            fprintf (out, "%d [shape=star];\n", node->left);
            fprintf (out, "%d [style=filled,color=\"red\"];\n", node->left);
            fprintf (out, "%d [label=\"%d\"]\n", node->left, node->left->data);
        }
        else {
            fprintf (out, "%d [shape=oval];\n", node->left);
            fprintf (out, "%d [style=filled,color=\"hotpink\"];\n", node->left);
            fprintf (out, "%d [label=\"%c\"]\n", node->left, node->left->data); 
        }
        fprintf (out, "\n");
        in_order_graph (node->left, out);
    }

    if (node->right != nullptr) {
        fprintf (out, "\"%d\" -> \"%d\";\n", node, node->right);

        if (node->right->type == NUMBER) {
            fprintf (out, "%d [shape=oval];\n", node->right);
            fprintf (out, "%d [style=filled,color=\"hotpink\"];\n", node->right);
            fprintf (out, "%d [label=\"%d\"]\n", node->right, node->right->data);
        }
        else if (node->right->type == OPERATION) {
            fprintf (out, "%d [shape=record];\n", node->right);
            fprintf (out, "%d [style=filled,color=\"green\"];\n", node->right);
            fprintf (out, "%d [label=\"%c\"]\n", node->right, node->right->data);
        }
        else if (node->right->type == VARIABLE) {
            fprintf (out, "%d [shape=diamond];\n", node->right);
            fprintf (out, "%d [style=filled,color=\"cyan2\"];\n", node->right);
            fprintf (out, "%d [label=\"%c\"]\n", node->right, node->right->data);
        }
        else if (node->right->type == FUNCTION_NAME) {
            fprintf (out, "%d [shape=star];\n", node->right);
            fprintf (out, "%d [style=filled,color=\"red\"];\n", node->right);
            fprintf (out, "%d [label=\"%d\"]\n", node->right, node->right->data);
        }
        else {
            fprintf (out, "%d [shape=oval];\n", node->right);
            fprintf (out, "%d [style=filled,color=\"hotpink\"];\n", node->right);
            fprintf (out, "%d [label=\"%c\"]\n", node->right, node->right->data); 
        }        fprintf (out, "\n");
        in_order_graph (node->right, out);

    }
    
    return;
}


char* fill_text (FILE* in, size_t file_size) {

    assert(in != nullptr);

    char* text_array = (char*) calloc(file_size + 1, sizeof(char));

    if (text_array) {
        fread(text_array, sizeof(char), file_size, in);
        *(text_array + file_size) = '\n';
        //(text_array + file_size + 1) ='\n';
        fseek(in, 0, SEEK_SET);
    }
    else {
        fseek(in, 0, SEEK_SET);
        return nullptr;
    }

    return text_array;
}
