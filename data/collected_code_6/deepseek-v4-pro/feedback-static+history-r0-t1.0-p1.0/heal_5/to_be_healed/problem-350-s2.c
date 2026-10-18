#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* minimize_by_removing_one_char(const char* str, size_t str_len) {
    if (str == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     *  Assuming that condition 'str_len==0' is not redundant
     */
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
        /* Possible weaknesses found:
         *  Assuming condition is false
         */
        if (copy == NULL) {
            return NULL;
        }
        /* Possible weaknesses found:
         *  Condition 'str_len>0' is always true
         *  Condition 'str_len>0' is always true [knownConditionTrueFalse]
         */
        if (str_len > 0) {
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(copy, str, str_len);
        }
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

    const size_t test_lengths[] = {
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        strlen("aabbbcc"),
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        strlen("abcd"),
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        strlen("aaabbb"),
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        strlen("abcabcabc"),
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        strlen(""),
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        strlen("aaaa")
    };

    for (size_t i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        char* minimized = minimize_by_removing_one_char(test_cases[i], test_lengths[i]);
        if (minimized == NULL) {
            printf("Input: \"%s\", Result: (allocation failed or null input)\n",
                   test_cases[i] ? test_cases[i] : "NULL");
            continue;
        }
        printf("Input: \"%s\", Minimized: \"%s\", Length: %zu\n",
               /* Possible weaknesses found:
                * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
                */
               test_cases[i], minimized, strlen(minimized));
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