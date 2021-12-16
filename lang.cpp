#include "func.h"

char *s = nullptr;

size_t token_ctor (node *head, char *s, size_t size) {

    size_t index = 0;

    while (1) {

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

            head[index].type = VARIABLE;
            head[index].data = 0;

            while (1) {
                        // printf ("%c", *s);
                head[index].data = *s ;
                if (isalpha (*(s + 1)) == 0)
                    break;
                else
                    ++s;
            }
        }
        else {
            head[index].type = OPERATION;
            head[index].data = *s;
        }

        ++index;
        ++s;
    }
    return index + 1;
}

                                        //TO DO:
                                        // 1) create tree (all func)
                                        // 2)multiple words (sentences in the file)

int main (void) {
        
    FILE* in = fopen ("open.txt", "rb");

    size_t file_size = find_file_size (in);

    s = (char*)calloc(file_size, sizeof (char));

    s = fill_text (in, file_size);

    printf ("%s\n", s);

    node head[file_size] = {};

    size_t num = token_ctor (head, s, file_size);

    for (int i = 0; i < num; ++i) {
        printf ("%d ", head[i].data);
    }

    puts ("\nALL GOOD");

    size_t counter = 0;
    
    node *tree = nullptr;
    tree = G (&tree, &counter, head);

    graph (tree);

    }
    // int res = getG ();

    // printf ("%d\n", res);
