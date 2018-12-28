#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "learning.h"
#include "word_processing.h"
#include "trie.h"

#define MIN_ARG_NUM 2
#define MAX_ARG_NUM 3
#define MIN_ROOT_LEN_ARG "-msl="
#define MIN_COUNT_ARG "-msf="

int input_check(int argc, char const *argv[]) {
    // test number of arguments
    if (argc < MIN_ARG_NUM || argc > MAX_ARG_NUM) return 1;

    // test postfix .exe
//    long file_strlen = strlen(argv[1]);
//    if (!('.' == argv[1][file_strlen-4] && 'e' == argv[1][file_strlen-3] == argv[1][file_strlen-1] &&
//            'x' == argv[1][file_strlen-2])) return 2;

//    printf("%s\n", &argv[1][file_strlen-4]);
//    printf("%d\n", strcmp(&argv[1][file_strlen-4], ".exe"));
//    if (strcmp(&argv[1][file_strlen-4], POSTFIX)) return 2;

    // test 2nd argument
    if (argc == MAX_ARG_NUM) {
        // test args validity
        if (!(strstr(argv[MAX_ARG_NUM - 1], MIN_ROOT_LEN_ARG) || strstr(argv[MAX_ARG_NUM -1], MIN_COUNT_ARG))
                || '-' != argv[MAX_ARG_NUM - 1][0])
            return 2;

        int arg = 0;
        arg = atoi(&argv[MAX_ARG_NUM - 1][5]);

        if (arg == 0) {
            return 3;
        }
    }

    return 0;
}

int main(int argc, char const *argv[]) {
    //chcp(28592);  zmena kodovani (windows)
    //ISO latin 2
    //    8859-2

//    get_root("poiuz", "oijkl", 3);
//
//    return 0;

    //printf("%d %s\n", argc, argv[1]);
    switch (input_check(argc, argv)) {
        case 0:
            break;
        case 1:
            printf("Error: Wrong argument count!\nUsage: sistem.exe <corpus-file | [\"]word-sequence[\"]> [-msl=<integer>] | [-msf=<integer>]\n");
            return EXIT_FAILURE;
        case 2:
            printf("Error: Unknown argument %s!\nUsage: sistem.exe <corpus-file | [\"]word-sequence[\"]> [-msl=<integer>] | [-msf=<integer>]\n",
                   argv[MAX_ARG_NUM - 1]);
            return EXIT_FAILURE;
        case 3:
            printf("Error: '%s' and '%s' arguments must be followed by integer > 0!\n", MIN_ROOT_LEN_ARG, MIN_COUNT_ARG);
            return EXIT_FAILURE;
        default:
            printf("Unknown error\n");
            return EXIT_FAILURE;
    }
//    printf("Arguments accepted!\n");

    FILE *file = fopen(argv[1], "r");

    // if file not found add .txt to the path
    if (!file) {
        char arg_1[256];
        snprintf(arg_1, sizeof arg_1, "%s%s", argv[1], ".txt");
        file = fopen(arg_1, "r");
    }

    if (argc == MAX_ARG_NUM) {
        int arg = 0;
        arg = atoi(&argv[MAX_ARG_NUM - 1][5]);

        // test file + msl
        if (file && strstr(argv[MAX_ARG_NUM - 1], MIN_COUNT_ARG)) {
            printf("Error: Argument mismatch! Can not use %s argument for learning mod.\n", MIN_COUNT_ARG);
            return EXIT_FAILURE;
        }

        // test words + msf
        if (!file && strstr(argv[MAX_ARG_NUM - 1], MIN_ROOT_LEN_ARG)) {
            printf("Error: Argument mismatch! Can not use %s argument for learning mod.\n", MIN_ROOT_LEN_ARG);
            return EXIT_FAILURE;
        }

        if (file) {
            learn_msl(file, arg);
        }
        else {
            process_words_msf(argv[1], arg);
        }
    } else {
        if (file) learn(file);
        else {
            //printf("File not found");
            process_words(argv[1]);
        }
    }

    return EXIT_SUCCESS;
}