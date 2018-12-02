//
// Created by kraus on 19.11.2018.
//

#ifndef CV10_18_TRIE_H
#define CV10_18_TRIE_H

#define CHARSET_LEN 256

#define EC 0
#define ERR_NONEXISTENT_TRIE 1
#define ERR_OUT_OF_MEMORY 2

typedef struct thenode {
    struct thenode *subtries[CHARSET_LEN];
    int frequency[CHARSET_LEN];
    int depth;
} node;

node *createTrie();
int insertToTrie(node *root, char *word/* word[] */);
void freeTrie(node *root);
void dumpTrie(node *root, char prefix[]);
char *getWord(node *root, char prefix[]);
char *getNextWord(node *root, char* word, char prefix[]);

#endif //CV10_18_TRIE_H
