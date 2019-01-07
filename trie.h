//
// Created by kraus on 19.11.2018.
//

#ifndef CV10_18_TRIE_H
#define CV10_18_TRIE_H

#include "word.h"
#include "error_levels.h"

// Supported charset length
#define CHARSET_LEN 256

// Trie data structure
typedef struct thenode {
    struct thenode *subtries[CHARSET_LEN];
    int frequency[CHARSET_LEN];
} node;

// Create trie
node *create_trie();
// Insert word into trie
int insert_to_trie(node *root, char *word);
// Get frequency of word in trie
int get_frequency(node *root, char *word);
// Free trie
void free_trie(node *root);
// Print all words in trie
void dump_trie(node *root, char *prefix);
// Get first word in trie with given prefix
char *get_word(node *root, char *prefix);
// Get word following given word in trie
char *get_next_word(node *root, char *word, char *prefix);

#endif //CV10_18_TRIE_H
