#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* minimize_by_removing_one_char(const char* str, size_t str_len) {
    if (str == NULL) {
        return NULL;
    }

    if (str_len == 0) {
        char* empty = (char*)malloc(1);
        if (empty == NULL) {
            return NULL;
        }
        empty[0] = '\0';
        return empty;
    }

    int freq[256] = {0};
    for (size_t i = 0; i < str_len; i++) {
        freq[(unsigned char)str[i]]++;
    }

    int max_freq = -1;
    char char_to_remove = '\0';
    for (int i = 0; i < 256; i++) {
        if (freq[i] > max_freq) {
            max_freq = freq[i];
            char_to_remove = (char)i;
        }
    }

    if (max_freq <= 0) {
        char* copy = (char*)malloc(str_len + 1);
        if (copy == NULL) {
            return NULL;
        }
        memcpy(copy, str, str_len);
        copy[str_len] = '\0';
        return copy;
    }

    size_t remaining = str_len - (size_t)max_freq;
    char* result = (char*)malloc(remaining + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t j = 0;
    for (size_t i = 0; i < str_len; i++) {
        if (str[i] != char_to_remove) {
            result[j++] = str[i];
        }
    }
    result[j] = '\0';

    return result;
}

int main(void) {
    const char* test_cases[] = {
        "aabbbcc",
        "abcd",
        "aaabbb",
        "abcabcabc",
        "",
        "aaaa"
    };

    size_t test_lengths[6];
    test_lengths[0] = strlen("aabbbcc");
    test_lengths[1] = strlen("abcd");
    test_lengths[2] = strlen("aaabbb");
    test_lengths[3] = strlen("abcabcabc");
    test_lengths[4] = strlen("");
    test_lengths[5] = strlen("aaaa");

    size_t num_tests = sizeof(test_cases) / sizeof(test_cases[0]);
    for (size_t i = 0; i < num_tests; i++) {
        char* minimized = minimize_by_removing_one_char(test_cases[i], test_lengths[i]);
        if (minimized == NULL) {
            printf("Input: \"%s\", Result: (allocation failed or null input)\n",
                   test_cases[i] ? test_cases[i] : "NULL");
            continue;
        }
        size_t minimized_len = strlen(minimized);
        printf("Input: \"%s\", Minimized: \"%s\", Length: %zu\n",
               test_cases[i], minimized, minimized_len);
        free(minimized);
    }

    char* null_result = minimize_by_removing_one_char(NULL, 0);
    if (null_result == NULL) {
        printf("Input: NULL, Result: NULL (handled)\n");
    } else {
        printf("Input: NULL, Result: \"%s\"\n", null_result);
        free(null_result);
    }

    return 0;
}