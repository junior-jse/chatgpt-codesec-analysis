#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRINGS 1000
#define MAX_LENGTH 100

typedef struct {
    char str[MAX_LENGTH];
    int count;
} StringCount;

char *second_most_frequent(const char strings[][MAX_LENGTH], int n) {
    if (n <= 0) return NULL;
    
    StringCount *counts = calloc(n, sizeof(StringCount));
    if (!counts) return NULL;
    
    int unique_count = 0;
    
    for (int i = 0; i < n; i++) {
        int found = 0;
        for (int j = 0; j < unique_count; j++) {
            if (strncmp(counts[j].str, strings[i], MAX_LENGTH) == 0) {
                counts[j].count++;
                found = 1;
                break;
            }
        }
        if (!found && unique_count < n) {
            strncpy(counts[unique_count].str, strings[i], MAX_LENGTH - 1);
            counts[unique_count].str[MAX_LENGTH - 1] = '\0';
            counts[unique_count].count = 1;
            unique_count++;
        }
    }
    
    if (unique_count < 2) {
        free(counts);
        return NULL;
    }
    
    int first_max = 0;
    int second_max = -1;
    
    for (int i = 1; i < unique_count; i++) {
        if (counts[i].count > counts[first_max].count) {
            second_max = first_max;
            first_max = i;
        } else if (counts[i].count < counts[first_max].count) {
            if (second_max == -1 || counts[i].count > counts[second_max].count) {
                second_max = i;
            }
        }
    }
    
    char *result = NULL;
    if (second_max != -1) {
        size_t len = strnlen(counts[second_max].str, MAX_LENGTH);
        result = malloc(len + 1);
        if (result) {
            if (len > 0) {
                memcpy(result, counts[second_max].str, len);
            }
            result[len] = '\0';
        }
    }
    
    free(counts);
    return result;
}

int main(void) {
    char strings[MAX_STRINGS][MAX_LENGTH];
    int n = 0;
    char input_buffer[32];
    
    printf("Enter number of strings: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Invalid input\n");
        return 1;
    }
    
    char *endptr = NULL;
    long val = strtol(input_buffer, &endptr, 10);
    if (endptr == input_buffer || val <= 0 || val > MAX_STRINGS) {
        printf("Invalid input\n");
        return 1;
    }
    n = (int)val;
    
    printf("Enter %d strings:\n", n);
    for (int i = 0; i < n; i++) {
        if (fgets(strings[i], MAX_LENGTH, stdin) != NULL) {
            size_t len = strnlen(strings[i], MAX_LENGTH);
            if (len > 0 && strings[i][len - 1] == '\n') {
                strings[i][len - 1] = '\0';
            }
        }
    }
    
    char *result = second_most_frequent(strings, n);
    
    if (result) {
        printf("Second most frequent string: %s\n", result);
        free(result);
    } else {
        printf("No second most frequent string found\n");
    }
    
    return 0;
}