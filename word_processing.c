//
// Created by kraus on 23.10.2018.
//
// "capek umeni vydlak aktery" -msf=1

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "word.h"
#include "learning.h"
#include "word_processing.h"

#define DEF_MSF 10

char *get_longest_root(char *word, root *roots, int msf) {
    if (!word || !roots)
        return "0";

    char *root = NULL;
    int max_root_len = 0;

    while (roots) {
        if ((strlen(roots->word) > max_root_len) && strstr(word, roots->word)) {
            root = roots->word;
            max_root_len = strlen(root);
        }

        roots = roots->next_root;
    }
    if (!root)
        return "0";
    return root;
}

int find_words_roots(const char *words, root *init_root, int msf) {
    if (!words || !init_root)
        return -1;

    int words_len = strlen(words);

    int i = 0, n;
    while (i < words_len && words[i]) {
        char *w = (char *) malloc(sizeof(char) * words_len);
        if (!w)
            return ERR_OUT_OF_MEMORY;

        n = 0;
        while (i < words_len && words[i] != SEPARATOR) {
            w[n] = words[i];

            i++;
            n++;
        }
        if (strlen(w) > 0) {
            printf("%s -> %s\n", w,get_longest_root(w, init_root, msf));
        }

        i++;
        free(w);
    }

    return EC;
}

root *load_roots(FILE *f, int msf) {
    int root_size = sizeof(root);
    root *init_root = (root *) malloc(root_size);
    root *root1 = (root *) malloc(root_size);
    root *root2 = NULL;

    if (!init_root || !root1)
        return NULL;

    init_root->prev_root = NULL;
    init_root->next_root = NULL;

    int c = fgetc(f);
    int n = 0, freq = 0;
    while (c != EOF) {
        n = 0;
        freq = 0;
        char *w = (char *) malloc(sizeof(char) * WORD_LEN);
        root2 = (root *) malloc(root_size);
        root2->next_root = NULL;
        root2->prev_root = NULL;

        if (!w || !root2)
            return init_root;

        // load word
        c = process_char(c);
        while (c != -1) {
            w[n] = (uchar) c;

            c = fgetc(f);
            c = process_char(c);
            n++;
        }
        w[n] = '\0';

        // load freq
        c = fgetc(f);
        while (c >= ASCII_0 && c <= ASCII_9) {
            freq = freq * 10 + c - ASCII_0;

            c = fgetc(f);
        }
        while (c < ASCII_a && c != EOF)
            c = fgetc(f);

        if (freq < msf) {
            free(root2);
            free(w);
            continue;
        }

        // save
        if (init_root->word) {
            root2->word = w;
            root2->freq = freq;
            root2->prev_root = root1;

            root1->next_root = root2;
            root1 = root2;
        } else {
            init_root->word = w;
            init_root->freq = freq;
            root1 = init_root;
        }
    }
    return init_root;
}

int process_words_msf(const char *words, int msf) {
    if (!words)
        return -1;

    FILE *f = fopen(ROOT_FILE, "r");

    if (!f)
        return ERR_NONEXISTING_FILE;

    root *init_root = load_roots(f, msf);
    if (!init_root)
        return ERR_OUT_OF_MEMORY;

//    root *root = init_root;
//    while (root) {
//        printf("%s %d\n", root->word, root->freq);
//        root = root->next_root;
//    }

    find_words_roots(words, init_root, msf);

    free_likend_list(init_root);

    return EC;
}

int process_words(const char *words) {
    return process_words_msf(words, DEF_MSF);
}