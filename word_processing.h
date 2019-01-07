//
// Created by kraus on 27.10.2018.
//

#ifndef STEMMER_WORD_PROCESSING_H
#define STEMMER_WORD_PROCESSING_H

#include "trie.h"
#include "word.h"
#include "learning.h"

// Default MSF value
#define DEF_MSF 10
// Root not found constant
#define ROOT_NOT_FOUND "0"
// Word separator
#define SEPARATOR ' '

// Run algorithms searching for roots in words
int process_words(const char *words);
// Run algorithms searching for roots in words; with specified MSF
int process_words_msf(const char *words, int msf);
// Loads roots from file *f with frequency >= msf
root *load_roots(FILE *f, int mls);
// Find words roots in linked list of roots
int find_words_roots(const char *words, root *init_root);
// Find the longest root of the word in linked list
char *get_longest_root(char *word, root *roots);

#endif //STEMMER_WORD_PROCESSING_H
