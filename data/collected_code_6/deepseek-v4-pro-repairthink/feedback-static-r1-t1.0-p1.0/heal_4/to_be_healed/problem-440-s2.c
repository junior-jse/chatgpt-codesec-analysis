#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define MAX_ADVERB_LEN 32

typedef struct {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char word[MAX_WORD_LEN];
    int position;
} AdverbInfo;

static const char *adverb_suffixes[] = {
    "ly",
    "wise",
    "wards",
    "where",
    "when",
    "how",
    "why",
    NULL
};

static int ends_with(const char *str, const char *suffix) {
    size_t str_len;
    size_t suffix_len;

    if (str == NULL || suffix == NULL) {
        return 0;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    str_len = strlen(str);
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    suffix_len = strlen(suffix);

    if (str_len < suffix_len) {
        return 0;
    }

    return strcmp(str + str_len - suffix_len, suffix) == 0;
}

static int is_adverb(const char *word) {
    size_t len;
    int i;

    if (word == NULL) {
        return 0;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    len = strlen(word);
    if (len < 2 || len >= MAX_ADVERB_LEN) {
        return 0;
    }

    if (isupper((unsigned char)word[0])) {
        return 0;
    }

    for (i = 0; adverb_suffixes[i] != NULL; i++) {
        if (ends_with(word, adverb_suffixes[i])) {
            if (strcmp(word, "family") == 0 || strcmp(word, "rely") == 0 ||
                strcmp(word, "apply") == 0 || strcmp(word, "supply") == 0) {
                continue;
            }
            return 1;
        }
    }

    return 0;
}

static void to_lowercase(char *str) {
    if (str == NULL) {
        return;
    }
    for (; *str != '\0'; str++) {
        *str = (char)tolower((unsigned char)*str);
    }
}

static int find_adverbs(const char *sentence, AdverbInfo *adverbs, int max_adverbs) {
    char *buffer;
    size_t sentence_len;
    char *token;
    const char *delimiters = " \t\n\r.,;:!?\"'()[]{}";
    int count;
    int position;
    char *saveptr;

    if (sentence == NULL || adverbs == NULL || max_adverbs <= 0) {
        return 0;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    sentence_len = strlen(sentence);
    buffer = malloc(sentence_len + 1);
    if (buffer == NULL) {
        return 0;
    }
    /* Possible weaknesses found:
     * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). (risk 1, buffer)
     */
    strncpy(buffer, sentence, sentence_len);
    buffer[sentence_len] = '\0';

    count = 0;
    position = 0;
    token = strtok_r(buffer, delimiters, &saveptr);

    while (token != NULL && count < max_adverbs) {
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        size_t token_len = strlen(token);

        if (token_len > 0 && token_len < MAX_WORD_LEN) {
            /* Possible weaknesses found:
             * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
             */
            char clean_word[MAX_WORD_LEN];
            size_t clean_len = 0;
            size_t i;

            for (i = 0; i < token_len; i++) {
                if (isalpha((unsigned char)token[i]) || token[i] == '-') {
                    if (clean_len < MAX_WORD_LEN - 1) {
                        clean_word[clean_len] = token[i];
                        clean_len++;
                    }
                }
            }
            clean_word[clean_len] = '\0';

            if (clean_len > 0) {
                /* Possible weaknesses found:
                 * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
                 */
                char lower_word[MAX_WORD_LEN];
                size_t j;

                for (j = 0; j < clean_len && j < MAX_WORD_LEN - 1; j++) {
                    lower_word[j] = (char)tolower((unsigned char)clean_word[j]);
                }
                lower_word[j] = '\0';

                if (is_adverb(lower_word)) {
                    snprintf(adverbs[count].word, MAX_WORD_LEN, "%s", clean_word);
                    adverbs[count].position = position;
                    count++;
                }
            }
        }

        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        position += (int)strlen(token);
        token = strtok_r(NULL, delimiters, &saveptr);
        if (token != NULL) {
            position++;
        }
    }

    free(buffer);
    return count;
}

int main(void) {
    const char *sentence = "The quick brown fox jumped extremely quickly over the lazy dog gracefully";
    AdverbInfo adverbs[MAX_WORDS];
    int count;
    int i;

    count = find_adverbs(sentence, adverbs, MAX_WORDS);

    printf("Adverbs found: %d\n", count);
    for (i = 0; i < count; i++) {
        printf("Adverb: '%s' at position %d\n", adverbs[i].word, adverbs[i].position);
    }

    return 0;
}