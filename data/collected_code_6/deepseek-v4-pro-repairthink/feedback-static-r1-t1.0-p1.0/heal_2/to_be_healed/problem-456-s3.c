#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRINGS 100
#define MAX_LENGTH 256
#define BUF_SIZE (MAX_LENGTH + 2)

void reverse_string(char *str, size_t len) {
    if (str == NULL || len == 0) {
        return;
    }
    for (size_t i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}

/* Possible weaknesses found:
 *  Parameter 'lengths' can be declared as pointer to const [constParameterPointer]
 */
void reverse_strings(char **strings, size_t *lengths, int count) {
    if (strings == NULL || lengths == NULL || count <= 0) {
        return;
    }
    for (int i = 0; i < count; i++) {
        if (strings[i] != NULL && lengths[i] > 0) {
            reverse_string(strings[i], lengths[i]);
        }
    }
}

int main(void) {
    char **strings = NULL;
    size_t *lengths = NULL;
    int count = 0;
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char buffer[BUF_SIZE];

    printf("Enter number of strings (max %d): ", MAX_STRINGS);
    if (scanf("%d", &count) != 1 || count <= 0 || count > MAX_STRINGS) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }

    int c;
    /* Possible weaknesses found:
     * Flawfinder getchar: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
     */
    while ((c = getchar()) != '\n' && c != EOF);

    strings = malloc(count * sizeof(char *));
    lengths = malloc(count * sizeof(size_t));
    if (strings == NULL || lengths == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(strings);
        free(lengths);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < count; i++) {
        printf("Enter string %d: ", i + 1);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            for (int j = 0; j < i; j++) {
                free(strings[j]);
            }
            free(strings);
            free(lengths);
            return EXIT_FAILURE;
        }

        size_t len = strnlen(buffer, sizeof(buffer));
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        }

        strings[i] = malloc(len + 1);
        if (strings[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            for (int j = 0; j < i; j++) {
                free(strings[j]);
            }
            free(strings);
            free(lengths);
            return EXIT_FAILURE;
        }
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(strings[i], buffer, len + 1);
        lengths[i] = len;
    }

    reverse_strings(strings, lengths, count);

    printf("\nReversed strings:\n");
    for (int i = 0; i < count; i++) {
        printf("%s\n", strings[i]);
    }

    for (int i = 0; i < count; i++) {
        free(strings[i]);
    }
    free(strings);
    free(lengths);

    return EXIT_SUCCESS;
}