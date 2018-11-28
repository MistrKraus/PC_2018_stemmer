//
// Created by kraus on 19.11.2018.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "trie.h"

node *createTrie() {
    int i;

    node *temp = (node *) malloc(sizeof(node));
    if (!temp) return NULL;

    for (i = 0; i < CHARSET_LEN; i++) {
        temp->subtries[i] = NULL;
        temp->frequency[i] = 0;
    }

    return temp;
}

int insertToTrie(node *root, char *word) {
    if (!root) return ERR_NONEXISTENT_TRIE;
    unsigned int idx = 0;
    int i, L;
    node *walk = root, *temp;

    L = strlen(word);

    for (i = 0; i < L; i++) {
        idx = (unsigned char) word[i];
        if (walk->subtries[idx]) {
            walk = walk->subtries[idx];
        } else {
            if (i < L - 1) {
                walk->subtries[idx] = createTrie();
                if (!walk->subtries[idx])
                    return ERR_OUT_OF_MEMORY;

                walk = walk->subtries[idx];
            }
        }
    }

    walk->frequency[idx]++;

    return EC;
}

void dumpTrie(node *root, char prefix[]) {
    if (!root) return;

    int i;
    char str[strlen(prefix) + 2];   // novy znak + ukoncovaci

    for (i = 0; i < CHARSET_LEN; i++) {
        sprintf(str, "%s%c", prefix, i);
        if (root->frequency[i])
            printf("%s (%d)\n", str, root->frequency[i]);
        if (root->subtries[i])
            dumpTrie(root->subtries[i], str);

//        if (root->subtries[i]) {
////            printf("%c", i);
//            sprintf(str, "%s%c", prefix, i);
//            dumpTrie(root->subtries[i], str);
//        } else {
//            if (root->frequency[i])
//                printf("%s%c (%d)\n", prefix, i, root->frequency[i]);
//        }
    }
}