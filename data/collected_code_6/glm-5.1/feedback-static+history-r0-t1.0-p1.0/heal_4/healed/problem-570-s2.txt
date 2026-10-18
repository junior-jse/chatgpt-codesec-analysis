#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char **remove_strings_containing(char **list, size_t count, const char *substr, size_t *out_count) {
    if (list == NULL || substr == NULL || out_count == NULL) {
        return NULL;
    }

    size_t valid_count = 0;
    for (size_t i = 0; i < count; i++) {
        if (list[i] != NULL && strstr(list[i], substr) == NULL) {
            valid_count++;
        }
    }

    char **result = (char **)malloc((valid_count + 1) * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }

    size_t j = 0;
    for (size_t i = 0; i < count; i++) {
        if (list[i] != NULL && strstr(list[i], substr) == NULL) {
            size_t len = strlen(list[i]) + 1;
            result[j] = (char *)malloc(len * sizeof(char));
            if (result[j] == NULL) {
                for (size_t k = 0; k < j; k++) {
                    free(result[k]);
                }
                free(result);
                return NULL;
            }
            memcpy(result[j], list[i], len);
            j++;
        }
    }
    result[j] = NULL;
    
    *out_count = valid_count;
    return result;
}

int main(void) {
    char *input_strings[] = {
        "apple",
        "banana",
        "cherry",
        "date",
        "elderberry",
        "fig",
        "grape"
    };
    size_t input_count = sizeof(input_strings) / sizeof(input_strings[0]);
    
    size_t out_count = 0;
    char **filtered = remove_strings_containing(input_strings, input_count, "a", &out_count);
    
    if (filtered == NULL) {
        fprintf(stderr, "Allocation failed\n");
        return 1;
    }

    printf("Filtered strings:\n");
    for (size_t i = 0; i < out_count; i++) {
        printf("%s\n", filtered[i]);
    }
    
    for (size_t i = 0; i < out_count; i++) {
        free(filtered[i]);
    }
    free(filtered);
    
    return 0;
}