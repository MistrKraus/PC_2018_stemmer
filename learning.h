//
// Created by kraus on 27.10.2018.
//

#ifndef STEMMER_LEARNING_H
#define STEMMER_LEARNING_H

#include "trie.h"
#include "word.h"
#include "error_levels.h"

// Name of file for roots
#define ROOT_FILE "stems.dat"
// Default MSL value
#define DEF_MSL 3

// Load words from file
node *load_words_msl(FILE *file, int msl);
// Run learning algorithms with specified minimum stem lenght
int learn_msl(FILE *file, int msl);
// Run learning algorithms
int learn(FILE *file);
// Save roots into file
int save_roots(node *roots);
// Find words roots
int find_roots(node *words, int msl);
// Search "ll_node" by comparing word1 and word2
char *get_root(char *word1, char *word2, int msl);

#endif //STEMMER_LEARNING_H
