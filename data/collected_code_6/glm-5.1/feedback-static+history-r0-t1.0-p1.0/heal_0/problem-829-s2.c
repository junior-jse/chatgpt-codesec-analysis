#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* find_second_most_repeated(char** strings, int size) {
    if (size < 2) {
        return NULL;
    }

    char** unique_strings = malloc(size * sizeof(char*));
    int* counts = malloc(size * sizeof(int));
    if (!unique_strings || !counts) {
        free(unique_strings);
        free(counts);
        return NULL;
    }

    int unique_size = 0;

    for (int i = 0; i < size; i++) {
        int found = 0;
        for (int j = 0; j < unique_size; j++) {
            if (strcmp(strings[i], unique_strings[j]) == 0) {
                counts[j]++;
                found = 1;
                break;
            }
        }
        if (!found) {
            unique_strings[unique_size] = strings[i];
            counts[unique_size] = 1;
            unique_size++;
        }
    }

    if (unique_size < 2) {
        free(unique_strings);
        free(counts);
        return NULL;
    }

    int first = 0, second = -1;

    for (int i = 1; i < unique_size; i++) {
        if (counts[i] > counts[first]) {
            second = first;
            first = i;
        } else if (counts[i] < counts[first]) {
            if (second == -1 || counts[i] > counts[second]) {
                second = i;
            }
        }
    }

    char* result = NULL;
    if (second != -1) {
        result = unique_strings[second];
    }

    free(unique_strings);
    free(counts);

    return result;
}

int main() {
    char* arr[] = {"apple", "banana", "apple", "orange", "banana", "apple", "banana"};
    int size = sizeof(arr) / sizeof(arr[0]);

    char* result = find_second_most_repeated(arr, size);

    if (result != NULL) {
        printf("%s\n", result);
    } else {
        printf("No second most repeated string found\n");
    }

    return 0;
}