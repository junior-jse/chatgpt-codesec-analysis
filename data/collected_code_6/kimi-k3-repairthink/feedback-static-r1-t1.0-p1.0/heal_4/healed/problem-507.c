#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORDS 100
#define MAX_WORD_LEN 50
#define WORD_BUF_SIZE (MAX_WORD_LEN + 1)

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

static int copy_word(char *dst, size_t dst_size, const char *src, size_t src_len) {
    if (dst == NULL || src == NULL || dst_size == 0) {
        return -1;
    }
    if (src_len + 1 > dst_size) {
        return -1;
    }
    memcpy(dst, src, src_len);
    dst[src_len] = '\0';
    return 0;
}

int remove_words(char words[][WORD_BUF_SIZE], int count, const char *to_remove[], int remove_count) {
    if (words == NULL || to_remove == NULL || count < 0 || remove_count < 0 || count > MAX_WORDS) {
        return -1;
    }

    int write_index = 0;
    int removed = 0;

    for (int i = 0; i < count; i++) {
        size_t word_len = bounded_strlen(words[i], WORD_BUF_SIZE);
        if (word_len > MAX_WORD_LEN) {
            return -1;
        }

        int should_remove = 0;
        for (int j = 0; j < remove_count; j++) {
            if (to_remove[j] != NULL) {
                size_t remove_len = bounded_strlen(to_remove[j], WORD_BUF_SIZE);
                if (remove_len == word_len && memcmp(words[i], to_remove[j], word_len) == 0) {
                    should_remove = 1;
                    break;
                }
            }
        }

        if (!should_remove) {
            if (write_index != i) {
                if (copy_word(words[write_index], sizeof(words[write_index]), words[i], word_len) != 0) {
                    return -1;
                }
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
    char words[MAX_WORDS][WORD_BUF_SIZE] = {
        "apple", "banana", "cherry", "date", "elderberry",
        "fig", "grape", "banana", "kiwi", "lemon"
    };

    for (int i = 0; i < MAX_WORDS; i++) {
        words[i][WORD_BUF_SIZE - 1] = '\0';
    }

    int count = 0;
    while (count < MAX_WORDS && words[count][0] != '\0') {
        count++;
    }

    for (int i = 0; i < count; i++) {
        if (bounded_strlen(words[i], WORD_BUF_SIZE) > MAX_WORD_LEN) {
            fprintf(stderr, "Error: word too long\n");
            return EXIT_FAILURE;
        }
    }

    const char *to_remove[] = {
        "banana", "date", "grape"
    };

    int remove_count = (int)(sizeof(to_remove) / sizeof(to_remove[0]));

    for (int i = 0; i < remove_count; i++) {
        if (to_remove[i] == NULL || bounded_strlen(to_remove[i], WORD_BUF_SIZE) > MAX_WORD_LEN) {
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
        printf("%.*s ", MAX_WORD_LEN, words[i]);
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
        printf("%.*s ", MAX_WORD_LEN, words[i]);
    }
    printf("\n");

    return EXIT_SUCCESS;
}