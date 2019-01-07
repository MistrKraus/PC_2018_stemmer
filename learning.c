//
// Created by kraus on 23.10.2018.
//

// param: C:\Users\kraus\Documents\PetrKraus\Skola\FAV\PC\stemmer\o_umeni_a_kulture_iii.txt -msl=3

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "learning.h"

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

/**
 * Search "root" by comparing word1 and word2
 *
 * @param word1
 * @param word2
 * @param msl Minimum stem length
 * @return root
 */
char *get_root(char *word1, char *word2, int msl) {
    if (!word1 || !word2 || msl < 1)
        return NULL;

    char *root = NULL;
    int i = 0, word1Len, word2Len, rootLen;
    word1Len = strlen(word1);
    word2Len = strlen(word2);

    // get maximum possible root length
    if (word2Len > word1Len)
        rootLen = word2Len - (word2Len - word1Len);
    else
        rootLen = word2Len;

    // while maximum possible root length is >= Minimum stem length
    while (rootLen >= msl) {
        for (i = 0; i <= word2Len - rootLen; i++) {
            // get substring of rootLen from word2
            root = strndup(&word2[i], rootLen);

//            if (!root) {                              //
//                printf("!root\n");                    //
//                continue;                             //
//            }

            // does word1 contain substring root
            if (strstr(word1, root))
                return root;

            free(root);
        }
        rootLen--;
    }
    return NULL;
}

/**
 * Save roots and it's frequency into file
 *
 * @param roots trie of roots and it's frequency
 * @return Error level
 */
int save_roots(node *roots) {
    FILE* file = fopen(ROOT_FILE, "w");
    char *temp = NULL;

    if (!file) {
        printf("File create error.\n");
        return OUT_OF_MEMORY_ERR;
    }
    char *w = get_word(roots, "");
    while (w) {
        fprintf(file, "%s %d\r\n", w, get_frequency(roots, w));
        temp = get_next_word(roots, w, "");
        free(w);
        w = temp;
    }
    free(temp);
    free(w);
    fclose(file);

    return NO_ERR;
}

/**
 * Find roots from words in trie words
 * Save roots into file
 *
 * @param words trie with words
 * @param msl Minimum stem length
 * @return Error level
 */
int find_roots(node *words, int msl) {
    if (!words || msl < 0)
        return NONEXISTING_TRIE_ERR;

    printf("Searching for roots...\n");
    node *roots = create_trie();
    if (!roots) {
        printf("Error");
        return OUT_OF_MEMORY_ERR;
    }

    char *word1 = NULL;
    char *word2 = NULL;
    char *temp_word = NULL;
    char *root = NULL;

    // Go thru all words and find roots
    word1 = get_word(words, "");
    while (word1) {
        temp_word = get_next_word(words, word1, "");
        free(word2);
        word2 = temp_word;
        while (word2) {
            root = get_root(word1, word2, msl);

            if (root) {
                insert_to_trie(roots, root);
                free(root);
            }

            temp_word = get_next_word(words, word2, "");
            free(word2);
            word2 = temp_word;
        }
        temp_word = get_next_word(words, word1, "");
        free(word1);
        word1 = temp_word;
    }


    // save into file
    printf("Saving roots\n");
    save_roots(roots);

    // free everything
    free_trie(roots);
    free(word1);
    free(word2);
    free(temp_word);

    return NO_ERR;
}

/**
 * Load words longer then msl param. into trie and return it
 *
 * @param file File to search roots in
 * @param msl Minimum stem length
 * @return Trie with all words longer than msl
 */
node* load_words_msl(FILE *file, int msl) {
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


    while (c != EOF/* && word_num < 500*/) {
        char w[WORD_LEN];

        // Load word
        n = 0;
        c = process_char(c);
        while (c && c != EOF) {
            w[n] = (char) c;

            c = fgetc(file);
            c = process_char(c);
            n++;
        }
        w[n] = '\0';

        // Get next valid char or EOF
        c = fgetc(file);
        while (!c) {
            c = fgetc(file);
            c = process_char(c);
        }

        // word length is lower then msl do not save
        if (n < msl)
            continue;

        // Error check
        switch (insert_to_trie(trie, w)) {
            case NO_ERR:
                // add number of currently loaded words
                word_num++;
                break;
            case NONEXISTING_TRIE_ERR:
                printf("Error: Non existing data structure.\n");
                return trie;
            case OUT_OF_MEMORY_ERR:
                printf("Error: Out of memory.\n");
                return trie;
            default:
                printf("Error: Unknown error.\n");
                return trie;
        }
    }
    printf("-> All words loaded\n");

    return trie;
}

/**
 * Run algorithms searching for roots in file
 * With specified MLS
 *
 * @param file File to search roots in
 * @param msl Minimum stem length
 * @return Error level
 */
int learn_msl(FILE *file, int msl) {
    node *words = NULL;
    words = load_words_msl(file, msl);

    // if loading words went wrong
    if (!words) {
        printf("Error: Loading words from file error.");
        return STRUCT_CREATING_ERR;
    }

    find_roots(words, msl);

    free_trie(words);
    printf("Trie is free\n");

    return NO_ERR;
}

/**
 * Run algorithms searching for roots in file
 * With default MLS
 *
 * @param file File to search roots in
 * @return Error level
 */
int learn(FILE *file) {
    return learn_msl(file, DEF_MSL);
}