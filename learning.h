//
// Created by kraus on 27.10.2018.
//

#ifndef STEMMER_LEARNING_H
#define STEMMER_LEARNING_H

#include "trie.h"

#define ROOT_FILE "stems.dat"

node *load_words_msl(FILE *file, int msl);
int learn_msl(FILE *file, int msl);
int learn(FILE *file);
int save_roots(node *roots);
int find_roots(node *words, int msl);
char *get_root(char *word1, char *word2, int msl);
int process_char(int c);

#endif //STEMMER_LEARNING_H
