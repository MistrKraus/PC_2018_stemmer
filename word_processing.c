//
// Created by kraus on 23.10.2018.
//
// "šel pes do lesa a potkal dlažební kostku" -msf=15

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
char *get_longest_root(char *word, ll_node *roots) {
    if (!word || !roots)
        return ROOT_NOT_FOUND;

    char *root = NULL;
    int max_root_len = 0;
//    int w_len = strlen(roots->word);

    // while there are roots search
    while (roots->word) {
        // if roots->word is root of the word
        if (strstr(word, roots->word)) {
            root = roots->word;
            max_root_len = strlen(root);
        }

        if (!roots->next)
            break;

        while (roots->next) {
            roots = roots->next;
            if (strlen(roots->word) > max_root_len)
                break;
        }
    }
    // if ll_node not found return ROOT_NOT_FOUND constant
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
int find_words_roots(const char *words, ll_node *init_root) {
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

        if (words[i] == ASCII_QUOT_MARK)
            i++;

        // Load one word
        n = 0;
        while (i < words_len && words[i] != SEPARATOR && words[i] != ASCII_QUOT_MARK) {
            w[n] = words[i];
            i++;
            n++;
        }
        w[n] = '\0';

        // Find ll_node of the word and print it
        w_len = strlen(w);
        if (w_len > 0) {
            temp = get_longest_root(w, init_root);
            printf("%s -> %s\n", w, temp);
        }
        i++;
        free(w);
    }
    w = NULL;
    temp = NULL;

    return NO_ERR;
}

/**
 * Loads roots from file *f with frequency >= msf
 *
 * @param f File with roots and it's frequency
 * @param msf Minimum Stem length
 * @return Linked list first node with roots
 */
ll_node *load_roots(FILE *f, int msf) {
    if (!f)
        return NULL;

    char *w = NULL;
    int root_size = sizeof(ll_node);
    // Root linked list first node
    ll_node *init_root = (ll_node *) malloc(root_size);
    if (!init_root)
        return NULL;

    init_root->word = NULL;
    init_root->next = NULL;

    int c = fgetc(f);
    int n = 0;
    int freq = 0;
    while (c != EOF) {
        n = 0;
        freq = 0;
        w = (char *) malloc(sizeof(char) * WORD_LEN);
        if (!w)
            return NULL;

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
        c = process_char(c);
        while (!c) {
            c = fgetc(f);
            c = process_char(c);
        }

        // if freq < msf don't add into linked list
        if (freq < msf) {
            free(w);
            continue;
        }
        add_node(&init_root, w);
        w = NULL;
    }

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

    ll_node *init_root = load_roots(f, msf);
    fclose(f);
    f = NULL;
    if (!init_root)
        return OUT_OF_MEMORY_ERR;
//    printf("-> All roots loaded\n");


//    printf("Searching for words roots...\n");
    int output = find_words_roots(words, init_root);

//    printf("Freeing allocated memory\n");
    free_linked_list(&init_root);
    free(init_root);
    init_root = NULL;

    return output;
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