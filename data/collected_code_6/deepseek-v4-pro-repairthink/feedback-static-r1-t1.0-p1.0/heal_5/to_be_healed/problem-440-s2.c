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

static int is_delimiter(char c) {
    return strchr(" \t\n\r.,;:!?\"'()[]{}", c) != NULL;
}

static int find_adverbs(const char *sentence, AdverbInfo *adverbs, int max_adverbs) {
    size_t len;
    size_t i;
    int count;

    if (sentence == NULL || adverbs == NULL || max_adverbs <= 0) {
        return 0;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    len = strlen(sentence);
    i = 0;
    count = 0;

    while (i < len && count < max_adverbs) {
        size_t start;
        /* Possible weaknesses found:
         * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
         */
        char clean_word[MAX_WORD_LEN];
        size_t wlen;
        int valid;
        /* Possible weaknesses found:
         *  Unused variable: j [unusedVariable]
         */
        size_t j;

        while (i < len && is_delimiter(sentence[i])) {
            i++;
        }
        if (i >= len) {
            break;
        }

        start = i;
        wlen = 0;
        valid = 1;

        while (i < len && (isalpha((unsigned char)sentence[i]) || sentence[i] == '-')) {
            if (wlen < MAX_WORD_LEN - 1) {
                /* Possible weaknesses found:
                 *  Variable 'clean_word[wlen]' is assigned a value that is never used. [unreadVariable]
                 */
                clean_word[wlen] = sentence[i];
                wlen++;
            } else {
                valid = 0;
            }
            i++;
        }

        while (i < len && !is_delimiter(sentence[i])) {
            i++;
        }

        if (valid && wlen > 0 && wlen < MAX_WORD_LEN) {
            clean_word[wlen] = '\0';
            {
                /* Possible weaknesses found:
                 * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
                 */
                char lower_word[MAX_WORD_LEN];
                size_t k;
                for (k = 0; k < wlen && k < MAX_WORD_LEN - 1; k++) {
                    lower_word[k] = (char)tolower((unsigned char)clean_word[k]);
                }
                lower_word[k] = '\0';

                if (is_adverb(lower_word)) {
                    snprintf(adverbs[count].word, MAX_WORD_LEN, "%s", clean_word);
                    adverbs[count].position = (int)start;
                    count++;
                }
            }
        }
    }

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