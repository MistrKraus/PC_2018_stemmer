//
// Created by kraus on 23.10.2018.
//

#ifndef __WORD__
#define __WORD__

#define WORD_LEN 64
#define ASCII_A 65

typedef struct {
    char *word;
    int num;
} word;

int compare(const void *a, const void *b);

#endif
