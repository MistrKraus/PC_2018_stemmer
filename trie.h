//
// Created by kraus on 19.11.2018.
//

#ifndef CV10_18_TRIE_H
#define CV10_18_TRIE_H

#define CHARSET_LEN 256

#define EC 0
#define ERR_NONEXISTING_TRIE 1
#define ERR_OUT_OF_MEMORY 2

typedef struct thenode {
    struct thenode *subtries[CHARSET_LEN];
    int frequency[CHARSET_LEN];
    int depth;
} node;

node *create_trie();
int insert_to_trie(node *root, char *word/* word[] */);
int free_trie(node *root);
void dump_trie(node *root, char *prefix);
char *get_word(node *root, char *prefix);
char *get_next_word(node *root, char *word, char *prefix);
int get_frequency(node *root, char *word);

#endif //CV10_18_TRIE_H
