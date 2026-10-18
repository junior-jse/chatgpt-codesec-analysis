#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_ADVERBS 100
#define INITIAL_SENTENCE_CAPACITY 128

typedef struct {
    char *word;
    size_t position;
} Adverb;

bool ends_with_ly(const char *word, size_t len) {
    if (word == NULL || len < 3) {
        return false;
    }
    return (word[len - 2] == 'l' && word[len - 1] == 'y');
}

bool is_word_char(int c) {
    return isalnum(c) || c == '\'' || c == '-';
}

char *read_sentence(FILE *stream, size_t *out_length) {
    size_t capacity = INITIAL_SENTENCE_CAPACITY;
    size_t length = 0;
    char *buffer = malloc(capacity);
    int c;

    if (buffer == NULL) {
        return NULL;
    }

    while ((c = fgetc(stream)) != EOF && c != '\n') {
        if (length >= capacity - 1) {
            size_t new_capacity;
            char *new_buffer;

            if (capacity > SIZE_MAX / 2) {
                free(buffer);
                return NULL;
            }

            new_capacity = capacity * 2;

            if (new_capacity <= capacity) {
                free(buffer);
                return NULL;
            }

            new_buffer = realloc(buffer, new_capacity);

            if (new_buffer == NULL) {
                free(buffer);
                return NULL;
            }

            buffer = new_buffer;
            capacity = new_capacity;
        }

        buffer[length++] = (char)c;
    }

    if (ferror(stream) != 0 || (c == EOF && length == 0)) {
        free(buffer);
        return NULL;
    }

    buffer[length] = '\0';

    if (out_length != NULL) {
        *out_length = length;
    }

    return buffer;
}

size_t find_adverbs(const char *sentence, size_t sentence_len, Adverb *adverbs, size_t max_adverbs) {
    size_t adverb_count = 0;
    size_t i = 0;

    if (sentence == NULL || adverbs == NULL || max_adverbs == 0) {
        return 0;
    }

    while (i < sentence_len && adverb_count < max_adverbs) {
        if (is_word_char((unsigned char)sentence[i])) {
            size_t word_start = i;
            size_t word_len;

            while (i < sentence_len && is_word_char((unsigned char)sentence[i])) {
                i++;
            }

            word_len = i - word_start;

            if (word_len > 0 && ends_with_ly(sentence + word_start, word_len)) {
                size_t word_buffer_size;
                char *word_copy;

                if (word_len >= SIZE_MAX) {
                    break;
                }

                word_buffer_size = word_len + 1;

                if (word_buffer_size < word_len) {
                    break;
                }

                word_copy = malloc(word_buffer_size);

                if (word_copy == NULL) {
                    break;
                }

                memcpy(word_copy, sentence + word_start, word_len);
                word_copy[word_len] = '\0';

                adverbs[adverb_count].word = word_copy;
                adverbs[adverb_count].position = word_start;
                adverb_count++;
            }
        } else {
            i++;
        }
    }

    return adverb_count;
}

int main(void) {
    char *sentence = NULL;
    Adverb *adverbs = NULL;
    size_t adverb_count;
    size_t sentence_len = 0;

    printf("Enter a sentence: ");
    fflush(stdout);

    sentence = read_sentence(stdin, &sentence_len);

    if (sentence == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }

    if ((size_t)MAX_ADVERBS > SIZE_MAX / sizeof(Adverb)) {
        fprintf(stderr, "Allocation size overflow\n");
        free(sentence);
        return EXIT_FAILURE;
    }

    adverbs = calloc(MAX_ADVERBS, sizeof(Adverb));

    if (adverbs == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(sentence);
        return EXIT_FAILURE;
    }

    adverb_count = find_adverbs(sentence, sentence_len, adverbs, MAX_ADVERBS);

    if (adverb_count == 0) {
        printf("No adverbs found.\n");
    } else {
        printf("Found %zu adverb(s):\n", adverb_count);

        for (size_t i = 0; i < adverb_count; i++) {
            printf("  \"%s\" at position %zu\n", adverbs[i].word, adverbs[i].position);
        }
    }

    for (size_t i = 0; i < adverb_count; i++) {
        free(adverbs[i].word);
        adverbs[i].word = NULL;
    }

    free(adverbs);
    adverbs = NULL;

    free(sentence);
    sentence = NULL;

    return EXIT_SUCCESS;
}