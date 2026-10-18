#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRINGS 1000
#define MAX_LENGTH 100

typedef struct {
    /* Possible weaknesses found:
     * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
     */
    char str[MAX_LENGTH];
    int count;
} StringCount;

/* Possible weaknesses found:
 *  Parameter 'strings' can be declared as const array [constParameter]
 */
char* find_second_most_frequent(char strings[][MAX_LENGTH], int n) {
    if (n <= 0 || strings == NULL) {
        return NULL;
    }
    
    StringCount* counts = (StringCount*)malloc(n * sizeof(StringCount));
    if (counts == NULL) {
        return NULL;
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
            /* Possible weaknesses found:
             * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). (risk 4, buffer)
             */
            strcpy(counts[unique_count].str, strings[i]);
            counts[unique_count].count = 1;
            unique_count++;
        }
    }
    
    if (unique_count < 2) {
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
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        return_str = (char*)malloc(strlen(result) + 1);
        if (return_str != NULL) {
            /* Possible weaknesses found:
             * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). (risk 4, buffer)
             */
            strcpy(return_str, result);
        }
    }
    
    free(counts);
    return return_str;
}

int main() {
    char strings[][MAX_LENGTH] = {
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