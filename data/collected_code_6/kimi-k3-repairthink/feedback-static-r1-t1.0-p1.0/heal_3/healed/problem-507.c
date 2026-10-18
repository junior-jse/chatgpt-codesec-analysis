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
            if (to_remove[j] != NULL) {
                size_t remove_len = bounded_strlen(to_remove[j], MAX_WORD_LEN);
                if (remove_len < MAX_WORD_LEN && remove_len == word_len &&
                    strncmp(words[i], to_remove[j], MAX_WORD_LEN) == 0) {
                    should_remove = 1;
                    break;
                }
            }
        }

        if (!should_remove) {
            if (write_index != i) {
                if (word_len + 1 > sizeof(words[write_index])) {
                    return -1;
                }
                int written = snprintf(words[write_index], sizeof(words[write_index]), "%s", words[i]);
                if (written < 0 || (size_t)written >= sizeof(words[write_index])) {
                    return -1;
                }
                words[write_index][MAX_WORD_LEN - 1] = '\0';
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
    char words[MAX_WORDS][MAX_WORD_LEN] = {
        "apple", "banana", "cherry", "date", "elderberry",
        "fig", "grape", "banana", "kiwi", "lemon"
    };

    for (int i = 0; i < MAX_WORDS; i++) {
        words[i][MAX_WORD_LEN - 1] = '\0';
    }

    int count = 0;
    while (count < MAX_WORDS && words[count][0] != '\0') {
        count++;
    }

    const char *to_remove[] = {
        "banana", "date", "grape"
    };

    int remove_count = (int)(sizeof(to_remove) / sizeof(to_remove[0]));

    for (int i = 0; i < remove_count; i++) {
        if (to_remove[i] == NULL || bounded_strlen(to_remove[i], MAX_WORD_LEN) >= MAX_WORD_LEN) {
            fprintf(stderr, "Error: invalid removal word\n");
            return EXIT_FAILURE;
        }
    }

    if (count == 0) {
        fprintf(stderr, "Error: word list is empty\n");
        return EXIT_FAILURE;
    }

    printf("Original list (%d words):\n", count);
    for (int i = 0; i < count; i++) {
        printf("%.*s ", MAX_WORD_LEN - 1, words[i]);
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
        printf("%.*s ", MAX_WORD_LEN - 1, words[i]);
    }
    printf("\n");

    return EXIT_SUCCESS;
}