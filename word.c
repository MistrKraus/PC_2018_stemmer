//
// Created by kraus on 12.11.2018.
//

#include <stdlib.h>
#include <stdio.h>
#include "word.h"

/**
 * Free linked list node
 *
 * @param node linked list node
 */
void free_linked_list_node(ll_node **node) {
    if (!node)
        return;

    if ((*node)->word) {
        free((*node)->word);
        (*node)->word = NULL;
    }
    free(*node);
    *node = NULL;
}

/**
 * Free linked list
 *
 * @param init_root first node of linked list
 */
void free_linked_list(ll_node **init_root) {
    if (!init_root)
        return;

    ll_node *temp = NULL;

    while (*init_root) {
        temp = (*init_root)->next;
        free_linked_list_node(init_root);
        *init_root = temp;
    }
//    free(temp);
    *init_root = NULL;
}

/**
 * Add word into linked list
 *
 * @param head first node of linked list
 * @param word word to be added into linked list
 * @return Error level
 */
int add_node(ll_node **head, char *word) {
    if (!head || !word)
        return UNKNOWN_ARG_ERR;

    // creating new node
    ll_node *new_node = (ll_node *) malloc(sizeof(ll_node));
    if (!new_node)
        return OUT_OF_MEMORY_ERR;
    new_node->word = word;
    new_node->next = NULL;

    // finding end of linked list
    if ((*head)->word) {
        ll_node *temp = *head;
        while (temp->next)
            temp = temp->next;

        temp->next = new_node;

        return NO_ERR;
    }

    new_node->word = NULL;
    free(new_node);
    (*head)->word = word;

    return NO_ERR;
}

/**
 * Check if char valid and return it's lowercase
 *
 * @param c char as int value
 * @return Lowercase char as int value; 0 if invalid char; EOF if EOF
 */
int process_char(int c) {
    if (c == EOF)
        return EOF;

    if (c >= ASCII_a && c <= ASCII_z)
        return c;

    if (c >= ASCII_LOW_DIAC_MIN1 && c <= ASCII_LOW_DIAC_MAX1)
        return c;

    if (c >= ASCII_LOW_DIAC_MIN2 && c <= ASCII_LOW_DIAC_MAX2)
        return c;

    if (c >= ASCII_LOW_DIAC_MIN3 && c <= ASCII_LOW_DIAC_MAX3)
        return c;

    if (c >= ASCII_A && c <= ASCII_Z)
        return c + ASCII_SHIFT;

    if ((c >= ASCII_DIAC_MIN1 && c <= ASCII_DIAC_MAX1) || (c >= ASCII_DIAC_MIN3 && c <= ASCII_DIAC_MAX3))
        return c + ASCII_DIAC_SHIFHT1_3;

    if (c >= ASCII_DIAC_MIN2 && c <= ASCII_DIAC_MAX2)
        return c + ASCII_DIAC_SHIFHT2;

    return INVALID_CHAR;
}