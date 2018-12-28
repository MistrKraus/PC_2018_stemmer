//
// Created by kraus on 12.11.2018.
//

#include <stdlib.h>
#include "word.h"

void free_likend_list(root *init_root) {
    if (!init_root)
        return;
    root *temp;

    while (init_root->next_root)
        init_root = init_root->next_root;

    while (init_root->prev_root) {
        free(init_root->next_root->word);
        free(init_root->next_root);
        init_root = init_root->prev_root;
    }

    free(init_root->word);
    free(&init_root);
}