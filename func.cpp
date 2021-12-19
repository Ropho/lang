#include "func.h"

extern char *s;


#define RULE(rule)    if (*tree == nullptr)                                     \
                        (*tree) = rule (tree, index, head);                     \
                      $                              


#define $   else {                      \
            printf ("%d", __LINE__);    \
            }                        


node *new_node () {
    node *node = (struct node*)calloc(1, sizeof (struct node));
    return node;
}


node *ASSN (node **tree, size_t *index, node *head) {

    printf ("ASSN: %d\n", *index);

    if (*tree != nullptr)
        abort ();

    *tree = &head[*index + 1];

    (*tree)->left = &head[*index];

    *index += 2;

    (*tree)->right = P (&(*tree)->right, index, head);

    return *tree;
}


node *COND (node **tree, size_t *index, node *head) {

    printf ("COND: %d\n", *index);
    
    if (head[*index].data != CALL && head[*index].data != RETURN) {
        node *tmp = new_node ();
        tmp = *tree;
        *tree = &head[*index];
        (*tree)->left = tmp;

        ++*index;
        // printf ("%d", head[*index].data);

        if (head[*index].data != '[')
            SYNTAX_ERROR
        ++*index;

        (*tree)->left  = E (&(*tree)->left,  index, head);
    printf ("EQEWQ:%d\n", head[*index].data);
        if (check_operations (head[*index].data) == 0)
            SYNTAX_ERROR

        (*tree)->left = SRAV (&(*tree)->left, index, head);
        ++*index;
        // printf ("\n%d\n", head[*index].data);

        (*tree)->left->right  = E (&(*tree)->left->right,  index, head);
        // puts ("OK");
        if (head[*index].data != ']')
            SYNTAX_ERROR
        ++*index;

        printf ("\n%d %d\n", __LINE__, head[*index].data);
        (*tree)->right = S (&(*tree)->right, index, head);

        printf ("AFTER {} %d\n", head[*index].data);
    }
    else {
        (*tree) = &head[*index];
        ++*index;
        (*tree)->left = &head[*index];
        ++*index;

        if (head[*index].data != '(')
            SYNTAX_ERROR
        ++*index;
        //ÀÐÃÓÌÅÍÒ!
        if (head[*index].data != ')')
            SYNTAX_ERROR
        ++*index;
    }
    // if ((*tree)->right->right == nullptr)
    //     abort ();
    // (*tree)->right->right  = E (&(*tree)->right->right,  index, head);

    // printf ("AFTER E: %d %d\n", __LINE__, head[*index+1].data);
    return *tree;
}


node *SRAV (node **tree, size_t *index, node *head) {

    node *tmp = new_node ();
    tmp = *tree;
    *tree = &head[*index];
    (*tree)->left = tmp;

    return *tree;
}


node *P (node **tree, size_t *index, node *head) {

    printf ("P: %d\n", *index);

    if (head[*index].type == PARANTHESIS && head[*index].data == '(') {
        
        ++*index;
        
        RULE(E);
        
        if (head[*index].type == PARANTHESIS && head[*index].data == ')') {
            ++*index;
            return *tree;
        }
        else {
            puts ("ABORT in )");
            SYNTAX_ERROR
        }
    }
    else if (head[*index].type == NUMBER) {
        RULE(N);
        ++*index;
        return *tree;
    }
    else if (head[*index].type == VARIABLE && head[*index + 1].data == '=') {
        RULE(ASSN);
        // ++*index;
        return *tree;
    }
    // else if (head[*index].type == VARIABLE && check_operations (head[*index + 1].data)) {
    //     RULE(SRAV);
    //     ++*index;
    //     return *tree;
    // }

    else if (head[*index].type == VARIABLE) {
        RULE(ID);
        ++*index;
        return *tree;
    }
    else if (head[*index].type == FUNCTION) {
        RULE(FUNC);
        ++*index;
        (*tree)->right = P (&(*tree)->right, index, head);
        return *tree;
    }
    else if (head[*index].type == CONDITION) {
        RULE(COND);
        // ++*index;
        return *tree;
    }
    else {
        puts ("ERROR");
        abort();
    }
}


node *FUNC (node **tree, size_t *index, node *head) {

    if (head[*index].type == FUNCTION) {
        (*tree) = &head[*index];
        ++*index;
        
        if (head[*index].type != FUNCTION_NAME) {
            printf ("FUNC_NAME abort: %d\n", *index);
            SYNTAX_ERROR;
        }

        if ((*tree)->left == nullptr)
            (*tree)->left = &head[*index];
        $
    }

    else {
        printf ("FUNC abort: %d\n", *index);
        SYNTAX_ERROR;
    }

    return *tree;
}


node *ID (node **tree, size_t *index, node *head) {

    if (head[*index].type == VARIABLE) {
        printf ("ID: %d\n", *index);
    }
    else {
        printf ("ID abort: %d\n", *index);
        SYNTAX_ERROR;
    }
    return &head[*index];
}


node *PW (node **tree, size_t *index, node *head) {

    printf ("PW: %d\n", *index);

    RULE(P);

    // ++*index;

    while (head[*index].type == OPERATION && head[*index].data == '^') {
        
        node *tmp = new_node ();
        tmp = *tree;
        *tree = &head[*index];
        
        if ((*tree)->left == nullptr)
            (*tree)->left = tmp;
        // else if ((*tree)->right == nullptr)
        //     (*tree)->right = tmp;
        $

        ++*index;
        
        // if ((*tree)->left == nullptr)
        //     (*tree)->left = P (&(*tree)->left, index, head);
        if ((*tree)->right == nullptr)
            (*tree)->right = P (&(*tree)->right, index, head);
        $
        // ++*index;
    }
    
    return *tree;
}


node *T (node **tree, size_t *index, node *head) {

    printf ("T: %d\n", *index);

    RULE(PW);

    // ++*index;

    while (head[*index].type == OPERATION && (head[*index].data == '*' || head[*index].data == '/')) {
        
        node *tmp = new_node ();
        tmp = *tree;
        *tree = &head[*index];
        
         if ((*tree)->left == nullptr)
            (*tree)->left = tmp;
        // if ((*tree)->right == nullptr)
        //     (*tree)->right = tmp;
        $

        ++*index;
        
        // if ((*tree)->left == nullptr)
        //     (*tree)->left = PW (&(*tree)->left, index, head);
        if ((*tree)->right == nullptr)
            (*tree)->right = PW (&(*tree)->right, index, head);
        $
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

    RULE(T);
    // else if ((*tree)->left == nullptr)
    //     (*tree)->left = T (&(*tree)->left, index, head);
    // else if ((*tree)->right == nullptr)
    //     (*tree)->right = T (&(*tree)->right, index, head);

    // ++*index;

    while (head[*index].type == OPERATION && (head[*index].data == '+' || head[*index].data == '-')) {
        printf ("E: %d\n", *index);

        node *tmp = new_node ();
        tmp = *tree;
        *tree = &head[*index];
        
        if ((*tree)->left == nullptr)
            (*tree)->left = tmp;
        // else if ((*tree)->right == nullptr)
        //     (*tree)->right = tmp;
        $

        ++*index;
        
        if ((*tree)->right == nullptr)
            (*tree)->right = T (&(*tree)->right, index, head);
        $
        // ++*index;
    }
    
    return *tree;
}


node *FUCK (node **tree, size_t *index, node *head) {
// puts ("FUCK HERR");
    if (head[*index].type == CONDITION && head[*index].data == MAIN) {

        head[*index].type = VARIABLE;
        (*tree) = new_node();
        (*tree)->type = CONDITION;
        strcpy ((*tree)->id, "CALL");
        
        (*tree)->left = &head[*index];

        ++*index;

        (*tree)->right = S (&(*tree)->right, index, head);

    }
    else if (head[*index].type == CONDITION && head[*index].data == DEFINE) {
        printf ("DUNCTION: %d\n", *index);
        (*tree) = &head[*index];
        ++*index;

        (*tree)->left = &head[*index];

        ++*index;

        if (head[*index].data != '(')
            SYNTAX_ERROR
        ++*index;
        // ÀÐÃÓÌÅÍÒ!!!
        if (head[*index].data != ')')
            SYNTAX_ERROR
        
        ++*index;
        (*tree)->right = S (&(*tree)->right, index, head);  
    }

    else {
        printf ("%d", head[*index].data);
        (*tree) = E ((tree), index, head);
    }
    return *tree;
}


node *G (node **tree, size_t *index, node *head) {

    node* nd = S (tree, index, head);

    if (head[*index].data == '$')
        return nd;
    else {
        printf ("G abort: %d\n", head[*index].data);
        SYNTAX_ERROR;
    }
}


node *S (node **tree, size_t *index, node *head) {

    if (head[*index].data == '{') {
        ++*index;
        
        *tree = new_node ();
        (*tree)->type = SENTENCE;
        strcpy ((*tree)->id, "SENTENCE");

        (*tree)->left = new_node ();
        (*tree)->left->type = KILLER;
        strcpy ((*tree)->left->id, "ya ustal");
        (*tree)->right = FUCK (&(*tree)->right, index, head);
    }
    else
        SYNTAX_ERROR

    printf ("S HERE: %d %d\n", head[*index - 1].data, head[*index].data);
    // int num_brackets = 1;

    while (head[*index].data == '?' && head[*index + 1].data != '}') {
        // puts ("OK");

        node *tmp = new_node ();
        tmp->left = *tree;
        *tree = tmp;
        (*tree)->type = SENTENCE;
        strcpy ((*tree)->id, "SENTENCE");

        // puts ("OK");
        *index += 1;
        printf ("%d: %d\n", __LINE__,head[*index].data);
        
        if ((*tree)->right != nullptr) {
            printf ("ABORT S: %d\n", *index);
            abort ();
        }

        // printf ("%d\n", head[*index].data);
        (*tree)->right = FUCK (&(*tree)->right, index, head);

        if (head[*index].data != '?')
            SYNTAX_ERROR
    }

    if (head[*index].data != '?' || head[*index + 1].data != '}')
        SYNTAX_ERROR

    ++*index;
    printf ("%d: %d\n", __LINE__, head[*index].data);

    // printf ("NUM_BRACKETS %d\n", num_brackets);
    *index += 1;
    return *tree;
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


void print_in_order_graph (node *node, FILE *out) {

    assert (node != nullptr);
    assert (out  != nullptr);

    if (node->type == NUMBER) {
        fprintf (out, "\"%p\" [shape=oval];\n", node);
        fprintf (out, "\"%p\" [style=filled,color=\"hotpink\"];\n", node);
        fprintf (out, "\"%p\" [label=\"%d\"]\n", node, node->data);
    }
    else if (node->type == OPERATION) {
        fprintf (out, "\"%p\" [shape=record];\n", node);
        fprintf (out, "\"%p\" [style=filled,color=\"green\"];\n", node);
        fprintf (out, "\"%p\" [label=\"%s\"]\n", node, node->id);
    }
    else if (node->type == VARIABLE) {
        fprintf (out, "\"%p\" [shape=diamond];\n", node);
        fprintf (out, "\"%p\" [style=filled,color=\"cyan2\"];\n", node);
        fprintf (out, "\"%p\" [label=\"%s\"]\n", node, node->id);
    }
    else if (node->type == FUNCTION_NAME) {
        fprintf (out, "\"%p\" [shape=star];\n", node);
        fprintf (out, "\"%p\" [style=filled,color=\"red\"];\n", node);
        fprintf (out, "\"%p\" [label=\"%d\"]\n", node, node->data);
    }
    else if (node->type == FUNCTION) {
        fprintf (out, "\"%p\" [shape=oval];\n", node);
        fprintf (out, "\"%p\" [style=filled,color=\"hotpink\"];\n", node);
        fprintf (out, "\"%p\" [label=\"%s\"]\n", node, node->id);
    }
    else if (node->type == SENTENCE || node->type == KILLER) {
        fprintf (out, "\"%p\" [shape=rectangle];\n", node);
        fprintf (out, "\"%p\" [style=filled,fontcolor = white,color=\"black\"];\n", node);
        fprintf (out, "\"%p\" [label=\"%s\"]\n", node, node->id);   
    }
    else if (node->type == CONDITION) {
        fprintf (out, "\"%p\" [shape=parallelogram];\n", node);
        fprintf (out, "\"%p\" [style=filled,fontcolor = black,color=\"red\"];\n", node);
        fprintf (out, "\"%p\" [label=\"%s\"]\n", node, node->id);   
    }
    else {
        fprintf (out, "\"%p\" [shape=oval];\n", node);
        fprintf (out, "\"%p\" [style=filled,color=\"hotpink\"];\n", node);
        fprintf (out, "\"%p\" [label=\"%c\"]\n", node, node->data); 
    }
}


void in_order_graph (node *node, FILE *out) {
    
    assert (node != nullptr);
    assert (out  != nullptr);

    print_in_order_graph (node, out);

    if (node->left != nullptr) {
        fprintf (out, "\"%p\" -> \"%p\";\n", node, node->left);

        print_in_order_graph (node->left, out);

        fprintf (out, "\n");
        in_order_graph (node->left, out);
    }

    if (node->right != nullptr) {
        fprintf (out, "\"%p\" -> \"%p\";\n", node, node->right);

        print_in_order_graph (node->right, out);

        fprintf (out, "\n");
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


bool check_func (int a) {
    switch (a) {
        case LN:
            return 1;
        break;

        case SIN:
            return 1;
        break;

        case COS:
            return 1;
        break;

        default:
            return 0;
        break;
    }
}


bool check_operations (int a) {
    switch (a) {
        case ASS:
            return 1;
        break;
        
        case LP:
            return 1;
        break;

        case RP:
            return 1;
        break;

        case OPER_EQ:
            return 1;
        break;
        
        case OPER_NE:
            return 1;
        break;

        case OPER_LE:
            return 1;
        break;

        case OPER_L:
            return 1;
        break;

        case OPER_G:
            return 1;
        break;

        case OPER_GE:
            return 1;
        break;

        default:
            return 0;
        break;
    }
}


bool check_keywords (int a) {
    switch (a) {
        case IF:
            return 1;
        break;

        case WHILE:
            return 1;
        break;

        case MAIN:
            return 1;
        break;
        
        case DEFINE:
            return 1;
        break;

        case CALL:
            return 1;
        break;

        case RETURN:
            return 1;
        break;

        default:
            return 0;
        break;
    }
}


void skip_symbols (char **s) {

    assert (*s != nullptr);

    while (1) {
        if (**s != ' ' && **s != '\t' && **s != '\n' && **s != '\r')
            break;
            ++(*s);
    }
}


size_t token_ctor (node *head, char *s, size_t size) {

    size_t index = 0;

    while (1) {
        skip_symbols (&s);
        // printf ("%c", *s);
        if (isdigit (*s)) {

            head[index].type = NUMBER;
            head[index].data = 0;

            while (1) {
                        // printf ("%c", *s);
                head[index].data = head[index].data * 10 + *s - '0';
                if (isdigit (*(s + 1)) == 0)
                    break;
                else
                    ++s;
            }
        }
        else if (*s == ')' || *s == '(') {

            head[index].type = PARANTHESIS;
            head[index].data = *s;
        }
        else if (*s == '$') {
            head[index].type = KILLER;
            head[index].data = *s;
            break;
        }
        else if (isalpha (*s)) {

            head[index].type  = VARIABLE;
            head[index].id[0] = '\0';
            head[index].data  = 0;

                
            while (1) {
                        // printf ("%c", *s);
                head[index].id[strlen(head[index].id) + 1] = '\0';
                head[index].id[strlen(head[index].id)] = *s;
                head[index].data += *s;

                if (isalpha (*(s + 1)) == 0)
                    break;
                else
                    ++s;
            }
            if (head[index - 1].type == CONDITION && head[index - 1].data == DEFINE && head[index - 1].data == CALL) {

                head[index].type = FUNCTION_NAME;

            }

            if (check_func (head[index].data)) {
                
                head[index + 1] = head[index];
                head[index + 1].type = FUNCTION_NAME;

                head[index].type = FUNCTION;
                head[index].data = -1;
                strcpy (head[index].id, "FUNC"); 
                ++index;
            }
            else if (check_keywords (head[index].data)) {

                head[index].type = CONDITION;
                
                if (head[index].data == IF)
                    strcpy (head[index].id, "IF"); 
                else if (head[index].data == WHILE)
                    strcpy (head[index].id, "WHILE");
                else if (head[index].data == MAIN)
                    strcpy (head[index].id, "MAIN");
                else if (head[index].data == DEFINE) 
                    strcpy (head[index].id, "DEFINE");
                else if (head[index].data == CALL)
                    strcpy (head[index].id, "CALL");
                else {
                    SYNTAX_ERROR
                }
            }
        }
        else {
            head[index].type = OPERATION;
            head[index].data  = 0;
                
            while (1) {
                head[index].id[strlen(head[index].id) + 1] = '\0';
                head[index].id[strlen(head[index].id)] = *s;
                head[index].data += *s;

                if ((*(s + 1)) != '=')
                    break;
                else
                    ++s;
            }
        }

        index++;
        ++s;
    }
    return index + 1;
}


