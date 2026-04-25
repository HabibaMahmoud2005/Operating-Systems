// Check if two input strings form a Joey-Kangaroo Pair (JKP)
// A kangaroo word contains all letters of the joey word in the same order (subsequence)

// The program takes exactly two strings from command line arguments (argv)

// Comparison must be case-insensitive (convert both strings to same case before processing)

// A valid JKP requires:
// 1) The two words are NOT identical
// 2) One word is longer (kangaroo), the other is shorter (joey)
// 3) All characters of the shorter word appear in order inside the longer word

// Output:
// Print 1 if the pair is a valid JKP
// Print 0 otherwise

// Do NOT use scanf/gets → input is taken only from argv

// No need to validate input (all inputs are guaranteed valid)

// String size is small (~20 chars), so simple solutions are acceptable
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

    if (argc != 3) {
        fprintf(stderr, "INVALID INPUT\n");
        exit(-1);
    }

    char *str1 = argv[1];
    char *str2 = argv[2];

    if (strcmp(str1, str2) == 0) {
        printf("0\n");
        return 0;
    }

    int prev = -1;
    int j;

    int len1 = strlen(str1);
    int len2 = strlen(str2);

    if (len1 >= len2) {
        for (int i = 0; i < len2; i++) {
            char *pos = strchr(str1, str2[i]);

            if (pos == NULL) {
                printf("0\n");
                return 0;
            }

            j = pos - str1;

            if (j <= prev) {
                printf("0\n");
                return 0;
            }

            prev = j;
        }
        printf("1\n");
    } else {
        for (int i = 0; i < len1; i++) {
            char *pos = strchr(str2, str1[i]);

            if (pos == NULL) {
                printf("0\n");
                return 0;
            }

            j = pos - str2;

            if (j <= prev) {
                printf("0\n");
                return 0;
            }

            prev = j;
        }
        printf("1\n");
    }

    return 0;
}