#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
 /* Possible weaknesses found:
  *  'SIZE_MAX' is defined in header '<stdint.h>'; this is probably fixable by adding '#include <stdint.h>'
  */

#define INITIAL_WORD_CAP 16U
#define INITIAL_UNIQUE_CAP 16U
#define MAX_TEXT_LEN (1024U * 1024U)

typedef struct {
    char *word;
    size_t count;
} WordCount;

static void free_words(WordCount *words, size_t unique_count)
{
    if (words == NULL) {
        return;
    }
    for (size_t i = 0; i < unique_count; i++) {
        free(words[i].word);
    }
    free(words);
}

static int add_word(WordCount **words, size_t *unique_count, size_t *capacity,
                    const char *buffer)
{
    for (size_t j = 0; j < *unique_count; j++) {
        if (strcmp((*words)[j].word, buffer) == 0) {
            (*words)[j].count++;
            return 0;
        }
    }

    if (*unique_count == *capacity) {
        size_t new_capacity = (*capacity == 0) ? INITIAL_UNIQUE_CAP
                                               : *capacity * 2;
        /* Possible weaknesses found:
         *  each undeclared identifier is reported only once for each function it appears in
         *  use of undeclared identifier 'SIZE_MAX'
         *  'SIZE_MAX' undeclared (first use in this function)
         */
        if (new_capacity > SIZE_MAX / sizeof **words) {
            return -1;
        }
        WordCount *tmp = realloc(*words, new_capacity * sizeof *tmp);
        if (tmp == NULL) {
            return -1;
        }
        *words = tmp;
        *capacity = new_capacity;
    }

    char *copy = strdup(buffer);
    if (copy == NULL) {
        return -1;
    }

    (*words)[*unique_count].word = copy;
    (*words)[*unique_count].count = 1;
    (*unique_count)++;
    return 0;
}

void find_most_common_words(const char *text)
{
    if (text == NULL) {
        printf("Invalid input text.\n");
        return;
    }

    size_t text_len = strnlen(text, MAX_TEXT_LEN);
    if (text_len == 0 || text_len >= MAX_TEXT_LEN) {
        printf("Invalid input text.\n");
        return;
    }

    WordCount *words = NULL;
    size_t unique_count = 0;
    size_t words_capacity = 0;

    size_t buf_capacity = INITIAL_WORD_CAP;
    char *buffer = malloc(buf_capacity);
    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return;
    }
    size_t buf_index = 0;
    int error = 0;

    for (size_t i = 0; i < text_len && !error; i++) {
        unsigned char c = (unsigned char)text[i];
        if (isalnum(c)) {
            if (buf_index + 1 >= buf_capacity) {
                /* Possible weaknesses found:
                 *  use of undeclared identifier 'SIZE_MAX'
                 *  'SIZE_MAX' is defined in header '<stdint.h>'; this is probably fixable by adding '#include <stdint.h>'
                 *  'SIZE_MAX' undeclared (first use in this function)
                 */
                if (buf_capacity > SIZE_MAX / 2) {
                    fprintf(stderr, "Word too long.\n");
                    error = 1;
                    break;
                }
                size_t new_capacity = buf_capacity * 2;
                char *tmp = realloc(buffer, new_capacity);
                if (tmp == NULL) {
                    fprintf(stderr, "Memory allocation failed.\n");
                    error = 1;
                    break;
                }
                buffer = tmp;
                buf_capacity = new_capacity;
            }
            buffer[buf_index++] = (char)tolower(c);
        } else if (buf_index > 0) {
            buffer[buf_index] = '\0';
            if (add_word(&words, &unique_count, &words_capacity, buffer) != 0) {
                fprintf(stderr, "Memory allocation failed.\n");
                error = 1;
                break;
            }
            buf_index = 0;
        }
    }

    if (!error && buf_index > 0) {
        buffer[buf_index] = '\0';
        if (add_word(&words, &unique_count, &words_capacity, buffer) != 0) {
            fprintf(stderr, "Memory allocation failed.\n");
            error = 1;
        }
    }

    free(buffer);

    if (error) {
        free_words(words, unique_count);
        return;
    }

    if (unique_count == 0) {
        printf("No words found in text.\n");
        free_words(words, unique_count);
        return;
    }

    size_t max_count = 0;
    for (size_t i = 0; i < unique_count; i++) {
        if (words[i].count > max_count) {
            max_count = words[i].count;
        }
    }

    printf("Most common word(s) with count %zu:\n", max_count);
    for (size_t i = 0; i < unique_count; i++) {
        if (words[i].count == max_count) {
            printf("%s: %zu\n", words[i].word, words[i].count);
        }
    }

    free_words(words, unique_count);
}

int main(void)
{
    const char *text = "The quick brown fox jumps over the lazy dog. The dog was not amused by the fox.";
    find_most_common_words(text);
    return 0;
}