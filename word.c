//
// Created by kraus on 12.11.2018.
//

#include <stdlib.h>
#include "word.h"

/**
 * Free linked list node
 *
 * @param node linked list node
 */
void free_linked_list_node(root *node) {
    if (!node)
        return;

    if (node->word) {
        free(node->word);
        node->word = NULL;
    }
    free(node);
    node = NULL;
}

/**
 * Free linked list
 *
 * @param init_root first node of linked list
 */
void free_linked_list(root *init_root) {
    if (!init_root)
        return;

    root *temp = NULL;

    while (init_root) {
        temp = init_root->next_root;
        free_linked_list_node(init_root);
        init_root = temp;
    }
//    int x = 0;
//    free(init_root);

//    while (init_root->next_root) {
//        free(init_root->word);
//        init_root = init_root->next_root;
//    }
//
//    free(init_root->word);
//    free(init_root);
//    free(&init_root);
}