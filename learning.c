//
// Created by kraus on 23.10.2018.
//

// param: C:\Users\kraus\Documents\PetrKraus\Skola\FAV\PC\stemmer\dasenka_cili_zivot_stenete -msl=3

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "learning.h"

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
        fprintf(file, "%s %d\n", w, get_frequency(roots, w));
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
 * Search "root" by comparing word1 and word2
 *
 * Source code assembled from C++ code: https://www.geeksforgeeks.org/print-longest-common-substring/
 *
 * @param word1
 * @param word2
 * @param msl Minimum stem length
 * @return root
 */
char *get_root(char *word1, char *word2, int msl) {
    if (!word1 || !word2 || msl < 1)
        return NULL;

    int i = 0;
    int j = 0;
    int root_len = 0;
    int w1_end = 0;
    int word1_len = strlen(word1);
    int word2_len = strlen(word2);
    int curr_row = 0;

    int lcs_table[2][word2_len];

    // fill lcs table
    for (i = 0; i <= word1_len; i++) {
        for (j = 0; j <= word2_len; j++) {
            // if first row / column fill with zero
            if (i == 0 || j == 0)
                lcs_table[curr_row][j] = 0;
            else {
                // if letters on the index i - 1;j - 1 are same
                if (word1[i - 1] == word2[j - 1]) {
                    lcs_table[curr_row][j] = lcs_table[1 - curr_row][j - 1] + 1;

                    // if it's new longest substring
                    if (lcs_table[curr_row][j] > root_len) {
                        root_len = lcs_table[curr_row][j];
                        w1_end = i - 1;
                    }
                }
                else
                    lcs_table[curr_row][j] = 0;
            }
        }
        curr_row = 1 - curr_row;
    }

    // if root isn't long enough
    if (root_len < msl)
        return NULL;

    return strndup(&word1[w1_end - root_len + 1], root_len);
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

//    printf("Searching for roots...\n");
    node *roots = create_trie();
    if (!roots) {
        return OUT_OF_MEMORY_ERR;
    }

    char *word1 = NULL;
    char *word2 = NULL;
    char *temp_word = NULL;
    char *root = NULL;
    int count = 0;

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
                count++;
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
//    printf("Saving roots\n");
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

    // while there is char to read
    while (c != EOF/* && word_num < 100*/) {
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
        c = process_char(c);
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
//    printf("-> All words loaded\n");

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
    if (!words)
        return NONEXISTING_TRIE_ERR;

    int output = find_roots(words, msl);

//    printf("Freeing allocated memory\n");
    free_trie(words);

    return output;
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