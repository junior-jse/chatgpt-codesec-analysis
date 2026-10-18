#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **words;
    int count;
    int capacity;
} WordList;

static void init_wordlist(WordList *wl) {
    wl->words = NULL;
    wl->count = 0;
    wl->capacity = 0;
}

static void free_wordlist(WordList *wl) {
    for (int i = 0; i < wl->count; i++) {
        free(wl->words[i]);
    }
    free(wl->words);
    wl->words = NULL;
    wl->count = 0;
    wl->capacity = 0;
}

static int is_duplicate(const WordList *wl, const char *word) {
    for (int i = 0; i < wl->count; i++) {
        if (strcmp(wl->words[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

static int add_word(WordList *wl, const char *word) {
    if (wl->count >= wl->capacity) {
        int newcap = (wl->capacity == 0) ? 16 : wl->capacity * 2;
        char **tmp = realloc(wl->words, newcap * sizeof(char *));
        if (tmp == NULL) {
            return -1;
        }
        wl->words = tmp;
        wl->capacity = newcap;
    }
    wl->words[wl->count] = strdup(word);
    if (wl->words[wl->count] == NULL) {
        return -1;
    }
    wl->count++;
    return 0;
}

void remove_duplicate_words(char *str) {
    if (str == NULL || *str == '\0') {
        return;
    }

    char *copy = strdup(str);
    if (copy == NULL) {
        return;
    }

    WordList seen;
    init_wordlist(&seen);

    size_t res_cap = 16;
    char *result = malloc(res_cap);
    if (result == NULL) {
        free(copy);
        free_wordlist(&seen);
        return;
    }
    result[0] = '\0';
    size_t res_len = 0;
    int first = 1;

    char *token = strtok(copy, " \t\n\r");
    while (token != NULL) {
        if (!is_duplicate(&seen, token)) {
            if (add_word(&seen, token) != 0) {
                free(result);
                free(copy);
                free_wordlist(&seen);
                return;
            }

            /* Possible weaknesses found:
             * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
             */
            size_t toklen = strlen(token);
            size_t needed = res_len + (first ? 0 : 1) + toklen + 1;

            if (needed > res_cap) {
                size_t newcap = res_cap;
                while (newcap < needed) {
                    newcap *= 2;
                }
                char *tmp = realloc(result, newcap);
                if (tmp == NULL) {
                    free(result);
                    free(copy);
                    free_wordlist(&seen);
                    return;
                }
                result = tmp;
                res_cap = newcap;
            }

            if (!first) {
                result[res_len++] = ' ';
            }
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result + res_len, token, toklen);
            res_len += toklen;
            result[res_len] = '\0';
            first = 0;
        }
        token = strtok(NULL, " \t\n\r");
    }

    memmove(str, result, res_len + 1);
    free(result);
    free(copy);
    free_wordlist(&seen);
}

int main(void) {
    char str[] = "hello world hello again world test hello";
    printf("Original: %s\n", str);
    remove_duplicate_words(str);
    printf("After removing duplicates: %s\n", str);
    return 0;
}