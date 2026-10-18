#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** remove_words(char** list, int count, const char* substr, int* out_count) {
    if (list == NULL || substr == NULL || out_count == NULL || count <= 0) {
        if (out_count) *out_count = 0;
        return NULL;
    }

    char** result = (char**)malloc(count * sizeof(char*));
    if (result == NULL) {
        *out_count = 0;
        return NULL;
    }

    int res_idx = 0;
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     *  Value stored to 'sub_len' during its initialization is never read [deadcode.DeadStores]
     *  Variable 'sub_len' is assigned a value that is never used. [unreadVariable]
     */
    size_t sub_len = strlen(substr);

    for (int i = 0; i < count; i++) {
        if (list[i] == NULL) {
            continue;
        }

        if (strstr(list[i], substr) == NULL) {
            /* Possible weaknesses found:
             * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
             */
            size_t word_len = strlen(list[i]);
            result[res_idx] = (char*)malloc(word_len + 1);
            if (result[res_idx] == NULL) {
                for (int j = 0; j < res_idx; j++) {
                    free(result[j]);
                }
                free(result);
                *out_count = 0;
                return NULL;
            }
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result[res_idx], list[i], word_len + 1);
            res_idx++;
        }
    }

    if (res_idx == 0) {
        free(result);
        *out_count = 0;
        return NULL;
    }

    char** shrunk_result = (char**)realloc(result, res_idx * sizeof(char*));
    if (shrunk_result == NULL) {
        *out_count = res_idx;
        return result;
    }

    *out_count = res_idx;
    return shrunk_result;
}

int main(void) {
    char* words[] = {"apple", "banana", "cherry", "date", "elderberry"};
    int count = sizeof(words) / sizeof(words[0]);
    int out_count = 0;
    
    char** filtered = remove_words(words, count, "a", &out_count);
    
    if (filtered != NULL) {
        for (int i = 0; i < out_count; i++) {
            printf("%s\n", filtered[i]);
            free(filtered[i]);
        }
        free(filtered);
    }

    return 0;
}