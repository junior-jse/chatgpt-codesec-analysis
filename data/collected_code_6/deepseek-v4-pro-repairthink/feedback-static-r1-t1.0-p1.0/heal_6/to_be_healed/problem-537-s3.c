#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char *word;
    int count;
} WordEntry;

static char *my_strdup(const char *s) {
    size_t len;
    char *p;
    if (s == NULL) return NULL;
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    len = strlen(s) + 1;
    p = malloc(len);
    /* Possible weaknesses found:
     * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). (risk 4, buffer)
     */
    if (p) strcpy(p, s);
    return p;
}

char *first_repeated_word(const char *str) {
    WordEntry *words = NULL;
    int word_count = 0;
    int word_capacity = 0;
    char *current_word = NULL;
    int cur_len = 0;
    int cur_cap = 0;
    int i = 0;

    if (str == NULL) return NULL;

    while (str[i] != '\0') {
        if (isalpha((unsigned char)str[i])) {
            if (cur_len + 1 >= cur_cap) {
                cur_cap = cur_cap ? cur_cap * 2 : 16;
                char *tmp = realloc(current_word, cur_cap);
                if (tmp == NULL) {
                    free(current_word);
                    for (int j = 0; j < word_count; j++) free(words[j].word);
                    free(words);
                    return NULL;
                }
                current_word = tmp;
            }
            current_word[cur_len++] = tolower((unsigned char)str[i]);
        } else {
            if (cur_len > 0) {
                int found = 0;
                current_word[cur_len] = '\0';
                for (int j = 0; j < word_count; j++) {
                    if (strcmp(words[j].word, current_word) == 0) {
                        words[j].count++;
                        found = 1;
                        if (words[j].count == 2) {
                            char *result = my_strdup(words[j].word);
                            free(current_word);
                            for (int k = 0; k < word_count; k++) free(words[k].word);
                            free(words);
                            return result;
                        }
                        break;
                    }
                }
                if (!found) {
                    if (word_count == word_capacity) {
                        word_capacity = word_capacity ? word_capacity * 2 : 8;
                        WordEntry *tmp = realloc(words, word_capacity * sizeof(WordEntry));
                        if (tmp == NULL) {
                            free(current_word);
                            for (int j = 0; j < word_count; j++) free(words[j].word);
                            free(words);
                            return NULL;
                        }
                        words = tmp;
                    }
                    words[word_count].word = current_word;
                    words[word_count].count = 1;
                    word_count++;
                    current_word = NULL;
                    cur_len = 0;
                    cur_cap = 0;
                } else {
                    free(current_word);
                    current_word = NULL;
                    cur_len = 0;
                    cur_cap = 0;
                }
            }
        }
        i++;
    }

    if (cur_len > 0) {
        int found = 0;
        current_word[cur_len] = '\0';
        for (int j = 0; j < word_count; j++) {
            if (strcmp(words[j].word, current_word) == 0) {
                words[j].count++;
                found = 1;
                if (words[j].count == 2) {
                    char *result = my_strdup(words[j].word);
                    free(current_word);
                    for (int k = 0; k < word_count; k++) free(words[k].word);
                    free(words);
                    return result;
                }
                break;
            }
        }
        if (!found) {
            if (word_count == word_capacity) {
                word_capacity = word_capacity ? word_capacity * 2 : 8;
                WordEntry *tmp = realloc(words, word_capacity * sizeof(WordEntry));
                if (tmp == NULL) {
                    free(current_word);
                    for (int j = 0; j < word_count; j++) free(words[j].word);
                    free(words);
                    return NULL;
                }
                words = tmp;
            }
            words[word_count].word = current_word;
            words[word_count].count = 1;
            word_count++;
            current_word = NULL;
        } else {
            free(current_word);
        }
    }

    for (int j = 0; j < word_count; j++) free(words[j].word);
    free(words);
    free(current_word);
    return NULL;
}

int main(void) {
    const char str1[] = "This is a test. This is only a test.";
    const char str2[] = "Hello world, hello everyone.";
    const char str3[] = "No repeated words here.";
    const char str4[] = "The cat and the dog and the bird";
    char *result;

    result = first_repeated_word(str1);
    if (result) {
        printf("First repeated word: %s\n", result);
        free(result);
    } else {
        printf("No repeated words found.\n");
    }

    result = first_repeated_word(str2);
    if (result) {
        printf("First repeated word: %s\n", result);
        free(result);
    } else {
        printf("No repeated words found.\n");
    }

    result = first_repeated_word(str3);
    if (result) {
        printf("First repeated word: %s\n", result);
        free(result);
    } else {
        printf("No repeated words found.\n");
    }

    result = first_repeated_word(str4);
    if (result) {
        printf("First repeated word: %s\n", result);
        free(result);
    } else {
        printf("No repeated words found.\n");
    }

    result = first_repeated_word(NULL);
    if (result) {
        printf("First repeated word: %s\n", result);
        free(result);
    } else {
        printf("No repeated words found.\n");
    }

    return 0;
}