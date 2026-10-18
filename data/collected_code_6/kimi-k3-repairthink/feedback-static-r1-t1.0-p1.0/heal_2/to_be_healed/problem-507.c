#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORDS 100
#define MAX_WORD_LEN 50

static size_t bounded_strlen(const char *s, size_t max_len) {
    size_t len = 0;
    if (s == NULL) {
        return 0;
    }
    while (len < max_len && s[len] != '\0') {
        len++;
    }
    return len;
}

int remove_words(char words[][MAX_WORD_LEN], int count, const char *to_remove[], int remove_count) {
    if (words == NULL || to_remove == NULL || count < 0 || remove_count < 0 || count > MAX_WORDS) {
        return -1;
    }

    int write_index = 0;
    int removed = 0;

    for (int i = 0; i < count; i++) {
        size_t word_len = bounded_strlen(words[i], MAX_WORD_LEN);
        if (word_len >= MAX_WORD_LEN) {
            return -1;
        }

        int should_remove = 0;
        for (int j = 0; j < remove_count; j++) {
            if (to_remove[j] != NULL && strcmp(words[i], to_remove[j]) == 0) {
                should_remove = 1;
                break;
            }
        }

        if (!should_remove) {
            if (write_index != i) {
                /* Possible weaknesses found:
                 * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                 */
                memcpy(words[write_index], words[i], word_len + 1);
            }
            write_index++;
        } else {
            removed++;
        }
    }

    for (int i = write_index; i < count; i++) {
        words[i][0] = '\0';
    }

    return removed;
}

int main(void) {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char words[MAX_WORDS][MAX_WORD_LEN] = {
        "apple", "banana", "cherry", "date", "elderberry",
        "fig", "grape", "banana", "kiwi", "lemon"
    };
    /* Possible weaknesses found:
     *  Assignment 'count=10', assigned value is 10
     */
    int count = 10;

    const char *to_remove[] = {
        "banana", "date", "grape"
    };
    /* Possible weaknesses found:
     *  Assignment 'remove_count=3', assigned value is 3
     */
    int remove_count = 3;

    /* Possible weaknesses found:
     *  Condition 'count<=0' is always false [knownConditionTrueFalse]
     *  Condition 'count<=0' is always false
     */
    if (count <= 0 || count > MAX_WORDS) {
        fprintf(stderr, "Error: word count out of range\n");
        return EXIT_FAILURE;
    }

    /* Possible weaknesses found:
     *  Condition 'remove_count<=0' is always false [knownConditionTrueFalse]
     *  Condition 'remove_count>(int)(sizeof(to_remove)/sizeof(to_remove[0]))' is always false [knownConditionTrueFalse]
     *  Condition 'remove_count>(int)(sizeof(to_remove)/sizeof(to_remove[0]))' is always false
     *  Condition 'remove_count<=0' is always false
     */
    if (remove_count <= 0 || remove_count > (int)(sizeof(to_remove) / sizeof(to_remove[0]))) {
        fprintf(stderr, "Error: remove count out of range\n");
        return EXIT_FAILURE;
    }

    printf("Original list (%d words):\n", count);
    for (int i = 0; i < count; i++) {
        printf("%s ", words[i]);
    }
    printf("\n\n");

    int removed = remove_words(words, count, to_remove, remove_count);

    if (removed < 0) {
        fprintf(stderr, "Error: Invalid parameters\n");
        return EXIT_FAILURE;
    }

    int new_count = count - removed;

    printf("Removed %d word(s)\n", removed);
    printf("Updated list (%d words):\n", new_count);
    for (int i = 0; i < new_count; i++) {
        printf("%s ", words[i]);
    }
    printf("\n");

    return EXIT_SUCCESS;
}