#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char *word;
    int count;
} WordCount;

char *str_dup(const char *str) {
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len = strlen(str) + 1;
    char *copy = malloc(len);
    if (copy) {
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(copy, str, len);
    }
    return copy;
}

void find_most_common(const char *text) {
    if (text == NULL) {
        return;
    }

    WordCount *words = NULL;
    size_t unique_count = 0;
    size_t capacity = 0;
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char buffer[256];
    size_t buf_idx = 0;

    for (size_t i = 0; text[i] != '\0'; i++) {
        if (isalpha((unsigned char)text[i])) {
            if (buf_idx < sizeof(buffer) - 1) {
                buffer[buf_idx++] = tolower((unsigned char)text[i]);
            }
        } else {
            if (buf_idx > 0) {
                buffer[buf_idx] = '\0';
                buf_idx = 0;

                int found = 0;
                for (size_t j = 0; j < unique_count; j++) {
                    if (strcmp(words[j].word, buffer) == 0) {
                        words[j].count++;
                        found = 1;
                        break;
                    }
                }

                if (!found) {
                    if (unique_count >= capacity) {
                        size_t new_capacity = capacity == 0 ? 16 : capacity * 2;
                        WordCount *new_words = realloc(words, new_capacity * sizeof(WordCount));
                        if (new_words == NULL) {
                            for (size_t k = 0; k < unique_count; k++) {
                                free(words[k].word);
                            }
                            free(words);
                            return;
                        }
                        words = new_words;
                        capacity = new_capacity;
                    }

                    words[unique_count].word = str_dup(buffer);
                    if (words[unique_count].word == NULL) {
                        for (size_t k = 0; k < unique_count; k++) {
                            free(words[k].word);
                        }
                        free(words);
                        return;
                    }
                    words[unique_count].count = 1;
                    unique_count++;
                }
            }
        }
    }

    if (buf_idx > 0) {
        buffer[buf_idx] = '\0';

        int found = 0;
        for (size_t j = 0; j < unique_count; j++) {
            if (strcmp(words[j].word, buffer) == 0) {
                words[j].count++;
                found = 1;
                break;
            }
        }

        if (!found) {
            if (unique_count >= capacity) {
                size_t new_capacity = capacity == 0 ? 16 : capacity * 2;
                WordCount *new_words = realloc(words, new_capacity * sizeof(WordCount));
                if (new_words == NULL) {
                    for (size_t k = 0; k < unique_count; k++) {
                        free(words[k].word);
                    }
                    free(words);
                    return;
                }
                words = new_words;
                /* Possible weaknesses found:
                 *  Value stored to 'capacity' is never read [deadcode.DeadStores]
                 *  Variable 'capacity' is assigned a value that is never used. [unreadVariable]
                 */
                capacity = new_capacity;
            }

            words[unique_count].word = str_dup(buffer);
            if (words[unique_count].word == NULL) {
                for (size_t k = 0; k < unique_count; k++) {
                    free(words[k].word);
                }
                free(words);
                return;
            }
            words[unique_count].count = 1;
            unique_count++;
        }
    }

    int max_count = 0;
    for (size_t i = 0; i < unique_count; i++) {
        if (words[i].count > max_count) {
            max_count = words[i].count;
        }
    }

    if (max_count > 0) {
        printf("Most common elements (count: %d):\n", max_count);
        for (size_t i = 0; i < unique_count; i++) {
            if (words[i].count == max_count) {
                printf("%s\n", words[i].word);
            }
        }
    }

    for (size_t i = 0; i < unique_count; i++) {
        free(words[i].word);
    }
    free(words);
}

int main(void) {
    const char *sample_text = "Hello world! This is a test. Hello again, this test is only a test.";
    find_most_common(sample_text);
    return 0;
}