#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reverse_strings(char **strings, size_t count) {
    size_t i;
    for (i = 0; i < count; i++) {
        if (strings[i] != NULL) {
            /* Possible weaknesses found:
             * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
             */
            size_t len = strlen(strings[i]);
            /* Possible weaknesses found:
             *  The scope of the variable 'left' can be reduced. [variableScope]
             */
            size_t left;
            /* Possible weaknesses found:
             *  The scope of the variable 'right' can be reduced. [variableScope]
             */
            size_t right;
            if (len > 0) {
                left = 0;
                right = len - 1;
                while (left < right) {
                    char temp = strings[i][left];
                    strings[i][left] = strings[i][right];
                    strings[i][right] = temp;
                    left++;
                    right--;
                }
            }
        }
    }
}

int main(void) {
    char **list = NULL;
    size_t count = 5;
    size_t i;

    list = (char **)malloc(count * sizeof(char *));
    if (list == NULL) {
        return EXIT_FAILURE;
    }

    list[0] = (char *)malloc(6 * sizeof(char));
    list[1] = (char *)malloc(6 * sizeof(char));
    list[2] = (char *)malloc(2 * sizeof(char));
    list[3] = (char *)malloc(5 * sizeof(char));
    list[4] = NULL;

    if (!list[0] || !list[1] || !list[2] || !list[3]) {
        for (i = 0; i < 4; i++) {
            free(list[i]);
        }
        free(list);
        return EXIT_FAILURE;
    }

    /* Possible weaknesses found:
     * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). Risk is low because the source is a constant string. (risk 2, buffer)
     */
    strcpy(list[0], "hello");
    /* Possible weaknesses found:
     * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). Risk is low because the source is a constant string. (risk 2, buffer)
     */
    strcpy(list[1], "world");
    /* Possible weaknesses found:
     * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). Risk is low because the source is a constant character. (risk 1, buffer)
     */
    strcpy(list[2], "");
    /* Possible weaknesses found:
     * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). Risk is low because the source is a constant string. (risk 2, buffer)
     */
    strcpy(list[3], "test");

    reverse_strings(list, count);

    for (i = 0; i < count; i++) {
        if (list[i] != NULL) {
            printf("%s\n", list[i]);
        }
    }

    for (i = 0; i < 4; i++) {
        free(list[i]);
    }
    free(list);

    return EXIT_SUCCESS;
}