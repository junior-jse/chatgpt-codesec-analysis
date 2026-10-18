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

    char *input_copy = strdup(str);
    if (input_copy == NULL) {
        return;
    }

    WordList seen;
    init_wordlist(&seen);

    size_t res_cap = 16;
    char *result = malloc(res_cap);
    if (result == NULL) {
        free(input_copy);
        free_wordlist(&seen);
        return;
    }
    result[0] = '\0';
    size_t res_len = 0;
    int first = 1;

    const char *delim = " \t\n\r";
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t len_input = strlen(input_copy);
    size_t i = 0;

    while (i < len_input) {
        while (i < len_input && strchr(delim, input_copy[i]) != NULL) {
            i++;
        }
        if (i >= len_input) {
            break;
        }
        size_t start = i;
        while (i < len_input && strchr(delim, input_copy[i]) == NULL) {
            i++;
        }
        size_t end = i;
        size_t toklen = end - start;

        char saved = '\0';
        if (end < len_input) {
            saved = input_copy[end];
            input_copy[end] = '\0';
        }
        /* Possible weaknesses found:
         *  Variable 'token' can be declared as pointer to const [constVariablePointer]
         */
        char *token = input_copy + start;

        if (!is_duplicate(&seen, token)) {
            if (add_word(&seen, token) != 0) {
                if (end < len_input) {
                    input_copy[end] = saved;
                }
                free(result);
                free(input_copy);
                free_wordlist(&seen);
                return;
            }

            size_t needed = res_len + (first ? 0 : 1) + toklen + 1;
            if (needed > res_cap) {
                size_t newcap = res_cap;
                while (newcap < needed) {
                    newcap *= 2;
                }
                char *tmp = realloc(result, newcap);
                if (tmp == NULL) {
                    if (end < len_input) {
                        input_copy[end] = saved;
                    }
                    free(result);
                    free(input_copy);
                    free_wordlist(&seen);
                    return;
                }
                result = tmp;
                res_cap = newcap;
            }

            if (!first) {
                result[res_len++] = ' ';
            }
            for (size_t j = 0; j < toklen; j++) {
                result[res_len + j] = token[j];
            }
            res_len += toklen;
            result[res_len] = '\0';
            first = 0;
        }

        if (end < len_input) {
            input_copy[end] = saved;
        }
    }

    memmove(str, result, res_len + 1);
    free(result);
    free(input_copy);
    free_wordlist(&seen);
}

int main(void) {
    char str[] = "hello world hello again world test hello";
    printf("Original: %s\n", str);
    remove_duplicate_words(str);
    printf("After removing duplicates: %s\n", str);
    return 0;
}