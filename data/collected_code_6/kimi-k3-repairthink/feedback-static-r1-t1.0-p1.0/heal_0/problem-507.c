#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORDS 100
#define MAX_WORD_LEN 50

int remove_words(char words[][MAX_WORD_LEN], int count, const char *to_remove[], int remove_count) {
    if (words == NULL || to_remove == NULL || count < 0 || remove_count < 0) {
        return -1;
    }

    int write_index = 0;
    int removed = 0;

    for (int i = 0; i < count; i++) {
        int should_remove = 0;
        for (int j = 0; j < remove_count; j++) {
            if (to_remove[j] != NULL && strcmp(words[i], to_remove[j]) == 0) {
                should_remove = 1;
                break;
            }
        }

        if (!should_remove) {
            if (write_index != i) {
                strncpy(words[write_index], words[i], MAX_WORD_LEN - 1);
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
    int count = 10;

    const char *to_remove[] = {
        "banana", "date", "grape"
    };
    int remove_count = 3;

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