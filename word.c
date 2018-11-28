//
// Created by kraus on 12.11.2018.
//

#include <string.h>

int compare(const void *a, const void *b) {
//    const char **w1, **w2;

    const char *w1 = *(const char **)a;
    const char *w2 = *(const char **)b;

    return strcmp(w1, w2);
}