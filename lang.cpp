#include "func.h"

char *s = nullptr;
                                        //TO DO:
                                        // 1)create x = 1
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
