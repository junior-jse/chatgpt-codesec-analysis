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
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
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
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    test_lengths[0] = strlen("aabbbcc");
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    test_lengths[1] = strlen("abcd");
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    test_lengths[2] = strlen("aaabbb");
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    test_lengths[3] = strlen("abcabcabc");
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    test_lengths[4] = strlen("");
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    test_lengths[5] = strlen("aaaa");

    size_t num_tests = sizeof(test_cases) / sizeof(test_cases[0]);
    for (size_t i = 0; i < num_tests; i++) {
        char* minimized = minimize_by_removing_one_char(test_cases[i], test_lengths[i]);
        if (minimized == NULL) {
            printf("Input: \"%s\", Result: (allocation failed or null input)\n",
                   test_cases[i] ? test_cases[i] : "NULL");
            continue;
        }
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        size_t minimized_len = strlen(minimized);
        printf("Input: \"%s\", Minimized: \"%s\", Length: %zu\n",
               test_cases[i], minimized, minimized_len);
        free(minimized);
    }

    /* Possible weaknesses found:
     *  Assignment 'null_result=minimize_by_removing_one_char(NULL,0)', assigned value is 0
     *  Calling function 'minimize_by_removing_one_char' returns 0
     */
    char* null_result = minimize_by_removing_one_char(NULL, 0);
    /* Possible weaknesses found:
     *  Condition 'null_result==NULL' is always true
     *  Condition 'null_result==NULL' is always true [knownConditionTrueFalse]
     */
    if (null_result == NULL) {
        printf("Input: NULL, Result: NULL (handled)\n");
    } else {
        printf("Input: NULL, Result: \"%s\"\n", null_result);
        free(null_result);
    }

    return 0;
}