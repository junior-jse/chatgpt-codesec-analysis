#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **string_to_list(const char *str, size_t *out_len) {
    size_t len;
    /* Possible weaknesses found:
     *  Unused variable: i [unusedVariable]
     */
    size_t i;
    char *buffer;
    char **list;
    char *token;

    if (str == NULL || out_len == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    len = strlen(str);
    if (len == 0) {
        *out_len = 0;
        return NULL;
    }

    buffer = (char *)malloc(len + 1);
    if (buffer == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). (risk 4, buffer)
     */
    strcpy(buffer, str);

    size_t capacity = 16;
    list = (char **)malloc(capacity * sizeof(char *));
    if (list == NULL) {
        free(buffer);
        return NULL;
    }

    *out_len = 0;
    token = strtok(buffer, " ");
    while (token != NULL) {
        if (*out_len >= capacity) {
            capacity *= 2;
            char **new_list = (char **)realloc(list, capacity * sizeof(char *));
            if (new_list == NULL) {
                for (size_t j = 0; j < *out_len; j++) {
                    free(list[j]);
                }
                free(list);
                free(buffer);
                return NULL;
            }
            list = new_list;
        }

        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        list[*out_len] = (char *)malloc(strlen(token) + 1);
        if (list[*out_len] == NULL) {
            for (size_t j = 0; j < *out_len; j++) {
                free(list[j]);
            }
            free(list);
            free(buffer);
            return NULL;
        }
        /* Possible weaknesses found:
         * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). (risk 4, buffer)
         */
        strcpy(list[*out_len], token);
        (*out_len)++;
        token = strtok(NULL, " ");
    }

    free(buffer);
    return list;
}

void free_list(char **list, size_t len) {
    if (list == NULL) {
        return;
    }
    for (size_t i = 0; i < len; i++) {
        free(list[i]);
    }
    free(list);
}

int main(void) {
    const char *input = "This is a sample string to convert";
    size_t len = 0;
    char **list = string_to_list(input, &len);

    if (list != NULL) {
        for (size_t i = 0; i < len; i++) {
            printf("%s\n", list[i]);
        }
        free_list(list, len);
    }

    return 0;
}