//
// Created by kraus on 23.10.2018.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "word.h"
#include "learning.h"
#include "word_processing.h"

#define DEF_MSF 10

char *get_longest_root(char *word, node *roots) {
    char *root;

    return NULL;
}

char *find_next_word(char *words, int start_id) {

}

int process_char2(int c) {
    if (c >= ASCII_0 && c <= ASCII_9 || c == SEPARATOR)
        return c;

    return process_char(c);
}

node *load_roots(FILE *f, int msf) {
    node *roots = create_trie();
    if (!roots)
        return NULL;

    int c = fgetc(f);

    while (c != EOF) {
        char w[WORD_LEN];

        c = process_char2(c);
        while (c != -1) {

            c = fgetc(f);
            c = process_char(c);
        }
    }
}

int process_words_msf(const char *words, int msf) {
    FILE *f = fopen(ROOT_FILE, "r");

    if (!f)
        return ERR_NONEXISTING_FILE;

    node *roots = load_roots(f, msf);
    if (!roots)
        return ERR_OUT_OF_MEMORY;



    return EC;
}

int process_words(const char *words) {
    return process_words_msf(words, DEF_MSF);
}