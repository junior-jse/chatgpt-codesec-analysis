#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 100

typedef struct {
    char *str;
    int count;
} StringCount;

char* find_second_most_frequent(const char strings[][MAX_LENGTH], int n) {
    if (n <= 0 || strings == NULL) {
        return NULL;
    }

    StringCount* counts = (StringCount*)malloc(n * sizeof(StringCount));
    if (counts == NULL) {
        return NULL;
    }
    for (int i = 0; i < n; i++) {
        counts[i].str = NULL;
        counts[i].count = 0;
    }

    int unique_count = 0;

    for (int i = 0; i < n; i++) {
        int found = 0;
        for (int j = 0; j < unique_count; j++) {
            if (strcmp(counts[j].str, strings[i]) == 0) {
                counts[j].count++;
                found = 1;
                break;
            }
        }
        if (!found) {
            size_t len = 0;
            while (len < MAX_LENGTH && strings[i][len] != '\0') {
                len++;
            }
            counts[unique_count].str = (char*)malloc(len + 1);
            if (counts[unique_count].str == NULL) {
                for (int k = 0; k < unique_count; k++) {
                    free(counts[k].str);
                }
                free(counts);
                return NULL;
            }
            /* Possible weaknesses found:
             * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). (risk 1, buffer)
             */
            strncpy(counts[unique_count].str, strings[i], len);
            counts[unique_count].str[len] = '\0';
            counts[unique_count].count = 1;
            unique_count++;
        }
    }

    if (unique_count < 2) {
        for (int i = 0; i < unique_count; i++) {
            free(counts[i].str);
        }
        free(counts);
        return NULL;
    }

    int max_count = -1;
    int second_max_count = -1;

    for (int i = 0; i < unique_count; i++) {
        if (counts[i].count > max_count) {
            second_max_count = max_count;
            max_count = counts[i].count;
        } else if (counts[i].count > second_max_count && counts[i].count < max_count) {
            second_max_count = counts[i].count;
        }
    }

    char* result = NULL;
    for (int i = 0; i < unique_count; i++) {
        if (counts[i].count == second_max_count) {
            result = counts[i].str;
            break;
        }
    }

    char* return_str = NULL;
    if (result != NULL) {
        size_t reslen = 0;
        while (reslen < MAX_LENGTH && result[reslen] != '\0') {
            reslen++;
        }
        return_str = (char*)malloc(reslen + 1);
        if (return_str != NULL) {
            /* Possible weaknesses found:
             * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). (risk 1, buffer)
             */
            strncpy(return_str, result, reslen);
            return_str[reslen] = '\0';
        }
    }

    for (int i = 0; i < unique_count; i++) {
        free(counts[i].str);
    }
    free(counts);

    return return_str;
}

int main() {
    const char strings[][MAX_LENGTH] = {
        "apple",
        "banana",
        "apple",
        "cherry",
        "banana",
        "apple",
        "cherry",
        "cherry",
        "date"
    };

    int n = sizeof(strings) / sizeof(strings[0]);

    char* second_most = find_second_most_frequent(strings, n);

    if (second_most != NULL) {
        printf("Second most frequent string: %s\n", second_most);
        free(second_most);
    } else {
        printf("Not enough unique strings\n");
    }

    return 0;
}