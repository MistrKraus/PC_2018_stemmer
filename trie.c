//
// Created by kraus on 19.11.2018.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "trie.h"
#include "word.h"

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
    node *walk = root;

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
    }
}

char *getWord(node *root, char prefix[]) {
    if (!root) return "";

    int i;
    char *str = NULL;
    str = (char *) malloc(sizeof(char) * (strlen(prefix) + 2));

    if (!str)
        return NULL;

    for (i = 0; i < CHARSET_LEN; i++) {
        sprintf(str, "%s%c", prefix, i);
        if (root->frequency[i]) {
//            printf("%s\n", str);
//            char *word[strlen(str)];
//            strcpy(word, str);
            return str;
        }

        if (root->subtries[i]) {
            char *temp = getWord(root->subtries[i], str);
            free(str);
            return temp;
        }
    }
}

node *getNextWordRoot(node *root, char* word, char *prefix) {
//    if (!root) return root;//prefix;
//
//    char str[strlen(prefix) + 2];
//    prefix[strlen(prefix - 1)] = word[0];
//
//    if (root->subtries[word[0]]) {
//        return getNextWordRoot(root->subtries[word[0]], &word[1], str);
//    } else {
//        int i = (int) word[0];
//        for (i; i < CHARSET_LEN; i++) {
//            if (root->frequency[i])
//                // nalezeno slovo!!! vrátit
//                printf("%s", str);
//
//            if (root->subtries[i])
//                return getWord(root, prefix);
//        }
//    }
}

char *getNextWord(node *root, char* word, char prefix[]) {
    if (!root || !word[0]) return prefix;

    char *str = NULL;
    str = (char *) malloc(sizeof(char) * (strlen(prefix) + 2));
    sprintf(str, "%s%c", prefix, (uchar) word[0]);
    char *temp = NULL;
    temp = (char *) malloc(sizeof(char) * (strlen(prefix) + 2));

    if (!str || !temp)
        return NULL;

    strcpy(temp, str);

    if (root->subtries[word[0]]) {
        temp = getNextWord(root->subtries[word[0]], &word[1], str);
    }

    if (!temp || (strlen(temp) == strlen(str))) {
        temp = (char *) malloc(sizeof(char) * (strlen(prefix) + 2));
        if (!temp)
            return NULL;

        int i = (int) ((uchar) word[0]) + 1;
        for (i; i < CHARSET_LEN; i++) {
            sprintf(temp, "%s%c", prefix, i);
            if (root->frequency[i]) {
                // nalezeno slovo!!! vrátit
                return temp;
            } else {
                if (root->subtries[i])
                    return getWord(root->subtries[i], temp);
            }
        }

        return NULL;
    }

    return temp;
}
