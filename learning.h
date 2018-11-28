//
// Created by kraus on 27.10.2018.
//

#ifndef STEMMER_LEARNING_H
#define STEMMER_LEARNING_H

#include "trie.h"

node *load_words(FILE *file);
node *load_words_mls(FILE *file, int mls);
int count_words(char **full_words, size_t word_num);

#endif //STEMMER_LEARNING_H
