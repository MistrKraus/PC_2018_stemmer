//
// Created by kraus on 23.10.2018.
//

// param: C:\Users\kraus\Documents\PetrKraus\Skola\FAV\PC\stemmer\o_umeni_a_kulture_iii.txt -mls=3

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "word.h"
#include "learning.h"
#include "trie.h"

#define DEF_MLS 3

// return lowercase char
// if wrong char return 0
int process_char(int c) {
//    printf("%c%d\n", c, c);
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
        return c + ASCII_DIAC_SHIFHT1;

    if (c >= ASCII_DIAC_MIN2 && c <= ASCII_DIAC_MAX2)
        return c + ASCII_DIAC_SHIFHT2;

    return -1;
}

char *get_root(char *word1, char *word2, int mls) {
//    printf("%s - %s", word1, word2);
    if (!word1 || !word2 || mls < 1)
        return NULL;

    char *root = NULL;
    int i = 0, word1Len, word2Len, rootLen;
    word1Len = strlen(word1);
    word2Len = strlen(word2);

    if (word2Len > word1Len)
        rootLen = word2Len - (word2Len - word1Len);
    else
        rootLen = word2Len;

    while (rootLen >= mls) {
        for (i = 0; i <= word2Len - rootLen; i++) {
            root = strndup(&word2[i], rootLen);

            if (!root)
                continue;

            if (strstr(word1, root)) {
//                printf(" -> %s\n", root);
                return root;
            }
            free(root);
        }

        rootLen--;
    }
//    printf("\n");

    return NULL;
}

int save_roots(node *roots) {
    FILE* file = fopen(ROOT_FILE, "w");

    if (!file) {
        printf("File create error.\n");
        return ERR_OUT_OF_MEMORY;
    }

    char *w = get_word(roots, "");
    if (!w)
        return -1;

    int freq = get_frequency(roots, w);

    while (freq > 0) {
//        printf("%s %d\n", w, freq);
        fprintf(file, "%s %d\r\n", w, freq);

        w = get_next_word(roots, w, "");
        if (!w)
            break;
        freq = get_frequency(roots, w);
    }
    fclose(file);
}

int find_roots(node *words, int mls) {
    if (!words || mls < 0)
        return ERR_NONEXISTING_TRIE;

    printf("Searching for roots...\n");
    node *roots = create_trie();
    if (!roots) {
        printf("Error");
        return ERR_OUT_OF_MEMORY;
    }

    char *word1 = NULL;
    char *word2 = NULL;
    char *root = NULL;

    word1 = get_word(words, "");
    while (word1) {
        word2 = get_next_word(words, word1, "");
        while (word2) {
            root = get_root(word1, word2, mls);

            if (root) {
                insert_to_trie(roots, root);
//                free(root);
            }

            word2 = get_next_word(words, word2, "");
        }

        word1 = get_next_word(words, word1, "");
    }

    //ulozi do souboru
    save_roots(roots);

    dump_trie(roots, "");
    free_trie(roots);

    return EC;
}

node* load_words_mls(FILE *file, int mls) {
    // ASCII of char in file
    int c;
    // index of chars in string
    int n;
    // number of words in array
    size_t word_num = 0;

    node *trie = create_trie();
    if (!trie)
        return trie;

    c = fgetc(file);

    // TODO remake char loading/processing
    while (c != EOF && word_num < 100) {
        char w[WORD_LEN];

        n = 0;
        c = process_char(c);
        while (c != -1) {
            w[n++] = (char) c;

            c = fgetc(file);
            c = process_char(c);
        }

//        printf("%s\n", w);
        // terminate with the null character
        w[n] = '\0';

        // Next valid char
        while (c < ASCII_A) {
            c = fgetc(file);
        }

        // word length is lower then mls do not save
        if (n < mls)
            continue;

        switch (insert_to_trie(trie, w)) {
            case EC:
                // add number of currently loaded words
                word_num++;
                break;
            case ERR_NONEXISTING_TRIE:
                printf("Error: Non existing data structure.\n");
                return trie;
            case ERR_OUT_OF_MEMORY:
                printf("Error: Out of memory.\n");
                return trie;
            default:
                printf("Unknown error.\n");
                return trie;
        }
    }
    printf("-> All words loaded\n");

    fclose(file);

    return trie;
}

int learn_mls(FILE *file, int mls) {
    node *words = NULL;
    words = load_words_mls(file, mls);

    if (!words) {
        printf("Error");
        return 1;
    }

    dump_trie(words, "");

    char *word = get_word(words, "");
    while (word) {
        printf("%s\n", word);
        word = get_next_word(words, word, "");
    }

    return 0;

    printf("-> Roots\n");
    find_roots(words, mls);

    free_trie(words);
    printf("Trie is free\n");
}

int learn(FILE *file) {
    learn_mls(file, DEF_MLS);
}