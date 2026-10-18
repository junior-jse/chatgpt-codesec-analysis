#include <stdio.h>
#include <stdlib.h>

#define MAX_STR_LEN 4096

static size_t bounded_strnlen(const char *str, size_t max_len)
{
    size_t len = 0;

    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return len;
}

char *minimize_string(const char *str)
{
    size_t len;
    size_t freq[256] = {0};
    size_t min_freq;
    char char_to_remove;
    char *result;
    size_t i;
    size_t j;

    if (str == NULL) {
        result = malloc(1);
        if (result != NULL) {
            result[0] = '\0';
        }
        return result;
    }

    len = bounded_strnlen(str, MAX_STR_LEN + 1);
    if (len == 0 || len > MAX_STR_LEN) {
        result = malloc(1);
        if (result != NULL) {
            result[0] = '\0';
        }
        return result;
    }

    for (i = 0; i < len; i++) {
        freq[(unsigned char)str[i]]++;
    }

    min_freq = len;
    char_to_remove = str[0];

    for (i = 0; i < len; i++) {
        unsigned char c = (unsigned char)str[i];
        if (freq[c] < min_freq) {
            min_freq = freq[c];
            char_to_remove = str[i];
        }
    }

    result = malloc(len - min_freq + 1);
    if (result == NULL) {
        return NULL;
    }

    j = 0;
    for (i = 0; i < len; i++) {
        if (str[i] != char_to_remove) {
            result[j++] = str[i];
        }
    }
    result[j] = '\0';

    return result;
}

int main(void)
{
    char *result = minimize_string("aabbcc");

    if (result != NULL) {
        printf("%s\n", result);
        free(result);
    }
    return 0;
}