//
// Created by kraus on 23.10.2018.
//

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "word.h"
#include "learning.h"

#define DEF_MLS 3
#define DEF_ARR_SIZE 100

/**
 * Resizes char array of array
 *
 * @param array array to be resized
 * @param arr_len length of array
 * @param offset length to be added to array
 * @return size_t new array length
 */
size_t resize(char **array, size_t arr_len, size_t offset) {
    size_t new_length = arr_len + offset;

    char **temp = (char **) malloc(sizeof(char *)*arr_len);
    if (!temp) {
        printf("Malloc error!\n");
        return 0;
    }

    size_t i = 0;
    for (i; i < arr_len; i++)
        temp[i] = array[i];

    array = (char **) realloc(array, new_length);

    if (!array) {
        printf("Realloc error!\n");
        return 0;
    }

    for (i = 0; i < arr_len; i++) {
        array[i] = temp[i];
    }

    for (int j = (unsigned int) arr_len; j < new_length; ++j) {
        array[j] = temp[arr_len - 1];
    }

    free(temp);
    temp = NULL;
    //free(temp);

// ? Alternative ? //

//    char **temp = (char **) malloc(sizeof(char *)*new_length);
//
//    size_t i = 0;
//    for (i; i < arr_len; i++)
//        temp[i] = array[i];
//
//    free(*array);
//
//    array = temp;

    return new_length;
}

//FILE *load_word(char *word, FILE *file, int c) {
//    int n = 0;
//    while (c >= 65) {
//        word[n++] = (char) c;
//
//        c = fgetc(file);
//    }
//    // terminate with the null character
//    word[n] = '\0';
//
//    printf("%s\n", word);
//
//    return file;
//}

void load_words_mls(FILE *file, int mls) {
    printf("File found! %d\n", mls);

    // ASCII of char in file
    int c;
    // index of  char in string
    int n;
    // size of array
    size_t arr_size = DEF_ARR_SIZE;
    // array of words
    char *full_words = (char *) malloc(sizeof(char *)*arr_size);
    // number of words in array
    size_t word_num = 0;
    //printf("%d\n", (int)(*(&full_words + 1) - full_words));
    c = fgetc(file);

    while (c != EOF && word_num < DEF_ARR_SIZE) {// && word_num < 300) {// && arr_size < DEF_ARR_SIZE) {
        //full_words[arr_size][0] = (char *) malloc(sizeof(char) * WORD_LEN);
        char *word = (char *) malloc(sizeof(char) * WORD_LEN);
        if (!word) {
            printf("Malloc error\n");
            break;
        }

        n = 0;
        while (c >= 65) {
            word[n++] = (char) c;
            c = fgetc(file);
        }
        // terminate with the null character
        word[n] = '\0';
        full_words[word_num] = word;
        // add number of currently loaded
        word_num++;

//        printf("[%d/%d]%s\n", (unsigned int) word_num, (unsigned int) arr_size, full_words[word_num - 1]);
//        if (word_num == arr_size) {
//            printf("-> Reallocking memory!\n");
//            arr_size = resize(full_words, arr_size, DEF_ARR_SIZE);
//            printf("New array size is %d\n", (unsigned int) arr_size);
//
//            if (arr_size == 0) {
//                printf("-> Realloc error!\n");
//            }
//        }

        while (c < 65) {
            c = fgetc(file);
        }
        //printf("+\n");
    }
    printf("Words loaded\n");

    return;


    fclose(file);

    printf("-> All words loaded.\n");

    for (int i = 0; i < word_num; ++i) {
        printf("%d%s\n", i, full_words[i]);
    }

//    size_t neco = resize(full_words, DEF_ARR_SIZE, 10);
//
//    printf("-> Realloc\n");
//
//    for (int j = 0; j < DEF_ARR_SIZE; ++j) {
//        printf("%d%s\n", j, full_words[j]);
//    }
}

void load_words(FILE *file) {
    load_words_mls(file, DEF_MLS);
}