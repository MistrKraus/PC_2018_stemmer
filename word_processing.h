//
// Created by kraus on 27.10.2018.
//

#ifndef STEMMER_WORD_PROCESSING_H
#define STEMMER_WORD_PROCESSING_H

#include "trie.h"
#include "word.h"

#define ERR_NONEXISTING_FILE 1
#define SEPARATOR ' '

int process_words(const char *words);
int process_words_msf(const char *words, int msf);
root *load_roots(FILE *f, int mls);
int find_words_roots(const char *words, root *init_root, int msf);
char *find_next_word(char *words, int start_id);
char *get_longest_root(char *word, root *roots, int msf);
int process_char2(int c);

#endif //STEMMER_WORD_PROCESSING_H
