//
// Created by kraus on 23.10.2018.
//

#ifndef __WORD__
#define __WORD__

// Word length in trie
#define WORD_LEN 128
// ASCII code for A
#define ASCII_A 65
// ASCII code for Z
#define ASCII_Z 90
// ASCII code for a
#define ASCII_a 97
// ASCII code for z
#define ASCII_z 122
// First ASCII code for uppercase letters with diacritic 1
#define ASCII_DIAC_MIN1 138
// Last ASCII code for uppercase letters with diacritic 1
#define ASCII_DIAC_MAX1 143
// First ASCII code for uppercase letters with diacritic 2
#define ASCII_DIAC_MIN2 192
// Last ASCII code for uppercase letters with diacritic 2
#define ASCII_DIAC_MAX2 222
// First ASCII code for uppercase letters with diacritic 3
#define ASCII_DIAC_MIN3 161
// Last ASCII code for uppercase letters with diacritic 3
#define ASCII_DIAC_MAX3 175
// First ASCII code for lowercase letters with diacritic 1
#define ASCII_LOW_DIAC_MIN1 154
// Last ASCII code for lowercase letters with diacritic 1
#define ASCII_LOW_DIAC_MAX1 159
// First ASCII code for lowercase letters with diacritic 2
#define ASCII_LOW_DIAC_MIN2 224
// Last ASCII code for lowercase letters with diacritic 2
#define ASCII_LOW_DIAC_MAX2 254
// First ASCII code for lowercase letters with diacritic 3
#define ASCII_LOW_DIAC_MIN3 177
// Last ASCII code for lowercase letters with diacritic 3
#define ASCII_LOW_DIAC_MAX3 191
// Uppercase to lowercase shift diacritic 1 & 3
#define ASCII_DIAC_SHIFHT1_3 16
// Uppercase to lowercase shift diacritic 2
#define ASCII_DIAC_SHIFHT2 32
// Uppercase to lowercase
#define ASCII_SHIFT 32
// ASCII 0
#define ASCII_0 48
// ASCII 9
#define ASCII_9 57
// Invalid char code
#define INVALID_CHAR 0

// define unsigned char shortcut
typedef unsigned char uchar;

// Linked list data structure
typedef struct theroot {
    char *word;
    struct theroot *next_root;
} root;

// Free linked list node
void free_linked_list_node(root *node);
// Free linked list
void free_linked_list(root *init_root);

#endif  //__WORD__
