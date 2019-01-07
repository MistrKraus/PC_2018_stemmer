//
// Created by kraus on 19.11.2018.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "trie.h"

/**
 * Create trie
 *
 * @return trie data structure
 */
node *create_trie() {
    int i;

    node *temp = (node *) malloc(sizeof(node));
    if (!temp) return NULL;

    for (i = 0; i < CHARSET_LEN; i++) {
        temp->subtries[i] = NULL;
        temp->frequency[i] = 0;
    }

    return temp;
}

/**
 * Insert word into trie
 *
 * @param root trie root node
 * @param word word to be inserted
 * @return Error level
 */
int insert_to_trie(node *root, char *word) {
    if (!root || !word) return NONEXISTING_TRIE_ERR;
    unsigned int idx = 0;
    int i, L;
    node *walk = root;

    L = strlen(word);
    for (i = 0; i < L; i++) {
        idx = (unsigned char) word[i];
        // if subtrie with required letter exist
        if (walk->subtries[idx]) {
            // if not last letter in word
            if (i < L - 1)
                walk = walk->subtries[idx];
            else
                walk->frequency[idx]++;
        } else {
            // if not last letter in word
            if (i < L - 1) {
                // create subtrie
                walk->subtries[idx] = create_trie();
                if (!walk->subtries[idx])
                    return OUT_OF_MEMORY_ERR;

                walk = walk->subtries[idx];
            } else
                walk->frequency[idx]++;
        }
    }

    return NO_ERR;
}

/**
 * Print all words in trie
 * Recursive functions
 *
 * @param root trie root
 * @param prefix prefix of word
 */
void dump_trie(node *root, char *prefix) {
    if (!root) return;

    int i;
    char str[strlen(prefix) + 2];

    for (i = ASCII_a; i < CHARSET_LEN; i++) {
        sprintf(str, "%s%c", prefix, i);
        if (root->frequency[i])
            printf("%s (%d)\n", str, root->frequency[i]);
        if (root->subtries[i])
            dump_trie(root->subtries[i], str);
    }
}

/**
 * Free trie
 * Recursive functions
 *
 * @param root trie node
 */
void free_trie(node *root) {
    if (!root) return;

    int i;
    for (i = 0; i < CHARSET_LEN; i++)
        if (root->subtries[i])
            free_trie(root->subtries[i]);

    free(root);
}

/**
 * Get first word in trie with given prefix
 * Recursive functions
 *
 * @param root trie node
 * @param prefix prefix of word
 * @return word's substring
 */
char *get_word(node *root, char *prefix) {
    if (!root) return "";

    int i;
    char *str = NULL;
    str = (char *) malloc(sizeof(char) * (strlen(prefix) + 2));

    if (!str)
        return NULL;

    for (i = ASCII_a; i < CHARSET_LEN; i++) {
        sprintf(str, "%s%c", prefix, i);
        if (root->frequency[i])
            return str;

        if (root->subtries[i]) {
            char *temp = get_word(root->subtries[i], str);
            free(str);
            return temp;
        }
    }

    free(str);
    return prefix;
}

/**
 * Get next word in trie with given word
 *
 * @param root trie node
 * @param word current word
 * @param prefix prefix of next word
 * @return
 */
char *get_next_word(node *root, char *word, char *prefix) {
    if (!root || !word[0]) return prefix;

    char *str = NULL;
    str = (char *) malloc(sizeof(char) * (strlen(prefix) + 2));
    if (!str)
        return NULL;

    sprintf(str, "%s%c", prefix, (uchar) word[0]);
    char *temp = NULL;
    temp = (char *) malloc(sizeof(char) * (strlen(prefix) + 2));
    char *w = NULL;

    if (!temp) {
        free(str);
        return NULL;
    }

    strcpy(temp, str);

    // go to subtrie with last letter of the word
    if (root->subtries[(uchar)word[0]]) {
        free(temp);
        temp = get_next_word(root->subtries[(uchar)word[0]], &word[1], str);
        if (strlen(word) == 1) {
            temp = get_word(root->subtries[(uchar) word[0]], str);
        }
    }

    if (!temp || !(strcmp(temp, str))) {
        if (temp)
            free(temp);

        temp = (char *) malloc(sizeof(char) * (strlen(prefix) + 2));
        if (!temp) {
            free(str);
            return NULL;
        }

        int i = (int) ((uchar) word[0]) + 1;
        for (i; i < CHARSET_LEN; i++) {
            sprintf(temp, "%s%c", prefix, i);
            if (root->frequency[i]) {
                free(str);
                return temp;
            }

            if (root->subtries[i]) {
                w = get_word(root->subtries[i], temp);
                free(str);
                free(temp);
                return w;
//                return get_word(root->subtries[i], temp);
            }
        }

        free(temp);
        free(str);
        return NULL;
    }

    if (strcmp(temp, word)) {
        free(str);
        return temp;
    }

    free(temp);
    free(str);
    return NULL;
}

/**
 * Get frequency of given word in trie
 *
 * @param root trie root node
 * @param word word which frequency is demanded
 * @return Frequency of word
 */
int get_frequency(node *root, char *word) {
    if (!root || !word)
        return -1;

    int i;

    // go to subtrie with last letter of the word
    for (i = 0; i < strlen(word) - 1; i++) {
        root = root->subtries[(uchar) word[i]];
    }

    return root->frequency[(uchar)word[strlen(word) - 1]];
}