#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *str;
    int count;
} StrCount;

char* find_second_most_repeated(char *arr[], int n) {
    if (n < 2) {
        return NULL;
    }

    StrCount *counts = (StrCount *)malloc(n * sizeof(StrCount));
    if (!counts) {
        return NULL;
    }

    int unique_size = 0;

    for (int i = 0; i < n; i++) {
        int found = 0;
        for (int j = 0; j < unique_size; j++) {
            if (strcmp(arr[i], counts[j].str) == 0) {
                counts[j].count++;
                found = 1;
                break;
            }
        }
        if (!found) {
            counts[unique_size].str = arr[i];
            counts[unique_size].count = 1;
            unique_size++;
        }
    }

    if (unique_size < 2) {
        free(counts);
        return NULL;
    }

    int first_max_idx = 0;
    int second_max_idx = -1;

    for (int i = 1; i < unique_size; i++) {
        if (counts[i].count > counts[first_max_idx].count) {
            second_max_idx = first_max_idx;
            first_max_idx = i;
        } else if (counts[i].count < counts[first_max_idx].count) {
            if (second_max_idx == -1 || counts[i].count > counts[second_max_idx].count) {
                second_max_idx = i;
            }
        }
    }

    char *result = NULL;
    if (second_max_idx != -1) {
        result = counts[second_max_idx].str;
    }

    free(counts);
    return result;
}

int main() {
    char *arr[] = {"apple", "banana", "apple", "orange", "banana", "apple", "banana"};
    int n = sizeof(arr) / sizeof(arr[0]);

    char *result = find_second_most_repeated(arr, n);

    if (result != NULL) {
        printf("%s\n", result);
    } else {
        printf("No second most repeated string found\n");
    }

    return 0;
}