#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "learning.h"
#include "word_processing.h"
#include "error_levels.h"

#define MIN_ARG_NUM 2
#define MAX_ARG_NUM 3
#define MIN_ROOT_LEN_ARG "-msl="
#define MIN_COUNT_ARG "-msf="
#define MLS_MSF_ARG_STR_LEN 4

/**
 * Checks args
 *
 * @param argc Number of args
 * @param argv Args
 * @return Error level
 */
int input_check(int argc, char const *argv[]) {
    // test number of arguments
    if (argc < MIN_ARG_NUM || argc > MAX_ARG_NUM)
        return ARG_COUNT_ERR;

    // test 2nd argument
    if (argc == MAX_ARG_NUM) {
        // test 2nd arg validity
        if (!(strstr(argv[MAX_ARG_NUM - 1], MIN_ROOT_LEN_ARG) || strstr(argv[MAX_ARG_NUM -1], MIN_COUNT_ARG))
                || '-' != argv[MAX_ARG_NUM - 1][0])
            return UNKNOWN_ARG_ERR;

        // checks if MLS or MSF arg is followed by integer value
        int arg = 0;
        arg = atoi(&argv[MAX_ARG_NUM - 1][MLS_MSF_ARG_STR_LEN + 1]);
        if (arg == 0) {
            return INT_REQ_ERR;
        }
    }

    return NO_ERR;
}

/**
 * Application entrance point
 * Choose a mode and run it
 *
 * @param argc Number of args
 * @param argv Args
 * @return Err level
 */
int main(int argc, char const *argv[]) {
    int output = NO_ERR;
    switch (input_check(argc, argv)) {
        case NO_ERR:
            break;
        case ARG_COUNT_ERR:
            printf("Error: Wrong argument count!\nUsage: sistem.exe <corpus-file | [\"]word-sequence[\"]> [-msl=<integer>] | [-msf=<integer>]\n");
            return EXIT_FAILURE;
        case UNKNOWN_ARG_ERR:
            printf("Error: Unknown argument %s!\nUsage: sistem.exe <corpus-file | [\"]word-sequence[\"]> [-msl=<integer>] | [-msf=<integer>]\n",
                   argv[MAX_ARG_NUM - 1]);
            return EXIT_FAILURE;
        case INT_REQ_ERR:
            printf("Error: '%s' and '%s' arguments must be followed by integer > 0!\n", MIN_ROOT_LEN_ARG, MIN_COUNT_ARG);
            return EXIT_FAILURE;
        default:
            printf("Unknown error\n");
            return EXIT_FAILURE;
    }

    FILE *file = fopen(argv[1], "r");

    // if file not found add .txt to the path
    if (!file) {
        char arg_1[256];
        snprintf(arg_1, sizeof arg_1, "%s%s", argv[1], ".txt");
        file = fopen(arg_1, "r");
    }

    // if has specified MLS or MSF
    if (argc == MAX_ARG_NUM) {
        int arg = 0;
        arg = atoi(&argv[MAX_ARG_NUM - 1][5]);

        // test combination file + msl
        if (file && strstr(argv[MAX_ARG_NUM - 1], MIN_COUNT_ARG)) {
            printf("Error: Argument mismatch! Can not use %s argument for learning mod.\n", MIN_COUNT_ARG);
            return EXIT_FAILURE;
        }

        // test combination words + msf
        if (!file && strstr(argv[MAX_ARG_NUM - 1], MIN_ROOT_LEN_ARG)) {
            printf("Error: Argument mismatch! Can not use %s argument for learning mod.\n", MIN_ROOT_LEN_ARG);
            return EXIT_FAILURE;
        }

        // if 1st arg is file run learning with mls arg
        if (file)
            output = learn_msl(file, arg);
        // else search for roots with msf arg
        else
            output = process_words_msf(argv[1], arg);
    } else {
        // if 1st arg is file run learning
        if (file)
            output = learn(file);
        // else search for roots
        else
            output = process_words(argv[1]);
    }

    if (output == NO_ERR)
        printf("Algorithm successfully finished.");
    else
        printf("Something went wrong. Algorithm didn't finish.");

    return EXIT_SUCCESS;
}