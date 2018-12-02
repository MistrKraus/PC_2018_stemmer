//
// Created by kraus on 23.10.2018.
//

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include "word.h"
#include "learning.h"
#include "trie.h"

#define DEF_MLS 3

// return lowercase char
int processChar(int c) {
//    setlocale(LC_CTYPE, "");
    if (isupper(c)) {
        c = tolower(c);
    } else {
        if (c >= 192 && c <= 221)
            c += 32;
    }

    return c;
}

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
        while ((c >= ASCII_A && c <= ASCII_Z) || c >= ASCII_a) {
            w[n++] = (char) processChar(c);

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

int learn_mls(FILE *file, int mls) {
    node *words = NULL;
    words = load_words_mls(file, mls);

    if (!words) {
        printf("Error");
        return 1;
    }

//    dumpTrie(words, "");

    // TODO Národní znaky
//    char *x = "èapka";
//    printf("%s (%d)\n", x, (int)x[0]);
//
//    printf("%d", (int) 'è');

//    char *word = getWord(words, "");
//    char *word2 = getNextWord(words, "karel", "");
//    char *word3 = getNextWord(words, word2, "");
//    char *word4 = getNextWord(words, word3, "");
//    char *word5 = getNextWord(words, word4, "");
//
//    printf("Word1: %s\n", word);
//    printf("Word2: %s\n", word2);
//    printf("Word3: %s\n", word3);
//    printf("Word4: %s\n", word4);
//    printf("Word5: %s\n", word5);
}

int learn(FILE *file) {
    learn_mls(file, DEF_MLS);
}