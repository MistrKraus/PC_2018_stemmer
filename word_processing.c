//
// Created by kraus on 23.10.2018.
//
// "capek umeni vydlak aktery" -msf=1

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "word_processing.h"

/**
 * Find the longest root of the word in linked list
 *
 * @param word word to find roots in
 * @param roots linked list of roots
 * @return The longest root of the word; or ROOT_NOT_FOUND constant
 */
char *get_longest_root(char *word, root *roots) {
    if (!word || !roots)
        return ROOT_NOT_FOUND;

    char *root = NULL;
    int max_root_len = 0;
    int w_len = 0;

    // while there are roots search
    while (roots->word) {
        w_len = strlen(roots->word);
        // if length of root > length of the longest already found root && root actually is root of the word
        if ((w_len > max_root_len) && strstr(word, roots->word)) {
            root = roots->word;
            max_root_len = w_len;
        }

        if (roots->next_root)
            roots = roots->next_root;
        else
            break;
    }
    // if root not found return ROOT_NOT_FOUND constant
    if (!root)
        return ROOT_NOT_FOUND;
    return root;
}

/**
 * Find words roots in linked list of roots
 *
 * @param words Words where to find roots
 * @param init_root linked list of words
 * @return Error level
 */
int find_words_roots(const char *words, root *init_root) {
    if (!words || !init_root)
        return UNKNOWN_ARG_ERR;

    int words_len = strlen(words);
    int w_len = 0;
    char *w = NULL;
    char *temp = NULL;

    int i = 0, n;
    while (i < words_len && words[i]) {
        w = (char *) malloc(sizeof(char) * words_len + 1);
        if (!w)
            return OUT_OF_MEMORY_ERR;

        // Load one word
        n = 0;
        while (i < words_len && words[i] != SEPARATOR) {
            w[n] = words[i];
            i++;
            n++;
        }
        w[n] = '\0';

        // Find root of the word and print it
        w_len = strlen(w);
        if (w_len > 0) {
            temp = get_longest_root(w, init_root);
            printf("%s -> %s\n", w, temp);
        }
        i++;
        free(w);
    }
    return NO_ERR;
}

/**
 * Loads roots from file *f with frequency >= msf
 *
 * @param f File with roots and it's frequency
 * @param msf Minimum Stem length
 * @return Linked list first node with roots
 */
root *load_roots(FILE *f, int msf) {
    if (!f)
        return NULL;

    char *w = NULL;
    int root_size = sizeof(root);
    // Root linked list first node
    root *init_root = (root *) malloc(root_size);
    if (!init_root)
        return NULL;
    root *root1 = (root *) malloc(root_size);
    if (!root1) {
        free(init_root);
        return NULL;
    }
    root *root2 = NULL;

    init_root->word = NULL;
    init_root->next_root = NULL;

    root1->word = NULL;
    root1->next_root = NULL;

    int c = fgetc(f);
    int n = 0, freq = 0;
    while (c != EOF) {
        n = 0;
        freq = 0;
        w = (char *) malloc(sizeof(char) * WORD_LEN);
        if (!w) {
            free(init_root);
            free(root1);
            return NULL;
        }
        root2 = (root *) malloc(root_size);
        if (!root2) {
            free(init_root);
            free(root1);
            free(w);
            return NULL;
        }

        root2->word = NULL;
        root2->next_root = NULL;

        // load word
        c = process_char(c);
        while (c && c != EOF) {
            w[n] = (uchar) c;

            c = fgetc(f);
            c = process_char(c);
            n++;
        }
        w[n] = '\0';

        // load freq
        c = fgetc(f);
        while (c >= ASCII_0 && c <= ASCII_9 && c != -1) {
            freq = freq * 10 + c - ASCII_0;

            c = fgetc(f);
        }
        // Loading next valid char or EOF
        while (c && c != EOF) {
            c = fgetc(f);
            c = process_char(c);
        }

        if (freq < msf) {
            free(root2);
            free(w);
            continue;
        }

        // save root into linked list
        if (init_root->word) {
            root2->word = w;

            root1->next_root = root2;
            root1 = root2;
        } else {
            init_root->word = w;
            root1 = init_root;
        }
    }

//    printf("init_root: %s\n", init_root->word);
//    printf("root1: %s\n", root1->word);
//    printf("root2: %s\n", root2->word);
//    free(root1);

    return init_root;
}

/**
 * Run algorithms searching for roots in words
 * With specified MSF
 *
 * @param words words words to find roots in
 * @param msf Minimum stem frequency
 * @return Error level
 */
int process_words_msf(const char *words, int msf) {
    // if words are not defined
    if (!words)
        return UNKNOWN_ARG_ERR;

    FILE *f = fopen(ROOT_FILE, "r");
    if (!f)
        return ERR_NONEXISTING_FILE;

    root *init_root = load_roots(f, msf);
    fclose(f);
    if (!init_root)
        return OUT_OF_MEMORY_ERR;

    find_words_roots(words, init_root);

    free_linked_list(init_root);
    init_root = NULL;
//    free(init_root);

    return NO_ERR;
}

/**
 * Run algorithms searching for roots in words
 *
 * @param words words to find roots in
 * @return Error level
 */
int process_words(const char *words) {
    return process_words_msf(words, DEF_MSF);
}