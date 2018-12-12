//
// Created by kraus on 19.11.2018.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "trie.h"
#include "word.h"

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

int insert_to_trie(node *root, char *word) {
    if (!root || !word) return ERR_NONEXISTING_TRIE;
    unsigned int idx = 0;
    int i, L;
    node *walk = root;

    L = strlen(word);
    for (i = 0; i < L; i++) {
        idx = (unsigned char) word[i];
        if (walk->subtries[idx]) {
            if (i < L - 1)
                walk = walk->subtries[idx];
            else
                walk->frequency[idx]++;
        } else {
            if (i < L - 1) {
                walk->subtries[idx] = create_trie();
                if (!walk->subtries[idx])
                    return ERR_OUT_OF_MEMORY;

                walk = walk->subtries[idx];
            } else
                walk->frequency[idx]++;
        }
    }

//    walk->frequency[idx]++;

    return EC;
}

void dump_trie(node *root, char *prefix) {
    if (!root) return;

    int i;
    char str[strlen(prefix) + 2];   // novy znak + ukoncovaci

    for (i = ASCII_a; i < CHARSET_LEN; i++) {
        sprintf(str, "%s%c", prefix, i);
        if (root->frequency[i])
            printf("%s (%d)\n", str, root->frequency[i]);
        if (root->subtries[i])
            dump_trie(root->subtries[i], str);
    }
}

int free_trie(node *root) {
    if (!root) return EC;

    int i;
    for (i = 0; i < CHARSET_LEN; i++) {
        if (root->subtries[i])
            free_trie(root->subtries[i]);
    }
    free(root);

    return EC;
}

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

    return prefix;
}

char *get_next_word(node *root, char *word, char *prefix) {
    if (!root || !word[0]) return prefix;

    char *str = NULL;
    str = (char *) malloc(sizeof(char) * (strlen(prefix) + 2));
    sprintf(str, "%s%c", prefix, (uchar) word[0]);
    char *temp = NULL;
    temp = (char *) malloc(sizeof(char) * (strlen(prefix) + 2));

//    if (!str || !temp)
    if (!str)
        return NULL;

    strcpy(temp, str);

    if (root->subtries[(uchar)word[0]]) {
        temp = get_next_word(root->subtries[(uchar)word[0]], &word[1], str);
        if (strlen(word) == 1)
            temp = get_word(root->subtries[(uchar)word[0]], str);
    }

    if (!temp || !(strcmp(temp, str))) {
        free(temp);

        temp = (char *) malloc(sizeof(char) * (strlen(prefix) + 2));
        if (!temp)
            return NULL;

//        if (root->frequency[word[0]] && root->subtries[word[0]]) {
//            sprintf(temp, "%s%c", prefix, (int) ((uchar)word[0]));
//            return get_word(root->subtries[word[0]], temp);
//        }

        int i = (int) ((uchar) word[0]) + 1;
//        int i = ASCII_a;
        for (i; i < CHARSET_LEN; i++) {
            sprintf(temp, "%s%c", prefix, i);
            if (root->frequency[i])
                return temp;

            if (root->subtries[i])
                return get_word(root->subtries[i], temp);
        }

        return NULL;
    }

    if (strcmp(temp, word))
        return temp;
    else
        return NULL;
}

int get_frequency(node *root, char *word) {
    if (!root || !word)
        return -1;

    int i;

    for (i = 0; i < strlen(word) - 1; i++) {
        root = root->subtries[(uchar) word[i]];
    }

    return root->frequency[word[strlen(word) - 1]];
}