//
// Created by kraus on 23.10.2018.
//

#include <stdio.h>
#include "word.h"
#include "learning.h"
#include "trie.h"

#define DEF_MLS 3

node* load_words_mls(FILE *file, int mls) {
    printf("File found! %d\n", mls);
    // ASCII of char in file
    int c;
    // index of chars in string
    int n;
    // number of words in array
    size_t word_num = 0;

    node *trie = createTrie();
    if (!trie)
        return trie;

    c = fgetc(file);

    while (c != EOF && word_num < 300) {// && arr_size < DEF_ARR_SIZE) {
        //full_words[arr_size][0] = (char *) malloc(sizeof(char) * WORD_LEN);
        char w[WORD_LEN];

        n = 0;
        while (c >= ASCII_A) {
            w[n++] = (char) c;
            c = fgetc(file);
        }
        // terminate with the null character
        w[n] = '\0';

        // Next valid char
        while (c < 65 && c != EOF) {
            c = fgetc(file);
        }

        // word length is lower then mls do not save
        if (n < mls)
            continue;

        switch (insertToTrie(trie, w)) {
            case EC:
                // add number of currently loaded words
                word_num++;
                break;
            case ERR_NONEXISTENT_TRIE:
                printf("Error: Non existing data structure.\n");
                return trie;
            case ERR_OUT_OF_MEMORY:
                printf("Error: Out of memory.\n");
                return trie;
            default:
                printf("Unknown error.\n");
                return trie;
        }
    }
    printf("-> All words loaded\n");

    fclose(file);

    return trie;
    //count_words(full_words, word_num);
}

node *load_words(FILE *file) {
    return load_words_mls(file, DEF_MLS);
}