#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char **split_at_lowercase(const char *str, int *count) {
    if (str == NULL || count == NULL) {
        return NULL;
    }

    size_t len = strlen(str);
    size_t max_parts = len + 1;
    char **parts = malloc(max_parts * sizeof(char *));
    if (parts == NULL) {
        return NULL;
    }

    int part_count = 0;
    const char *start = str;
    const char *p = str;

    while (*p != '\0') {
        if (islower((unsigned char)*p)) {
            size_t part_len = p - start;
            parts[part_count] = malloc(part_len + 1);
            if (parts[part_count] == NULL) {
                for (int i = 0; i < part_count; i++) {
                    free(parts[i]);
                }
                free(parts);
                return NULL;
            }
            strncpy(parts[part_count], start, part_len);
            parts[part_count][part_len] = '\0';
            part_count++;
            start = p + 1;
        }
        p++;
    }

    size_t remaining_len = p - start;
    parts[part_count] = malloc(remaining_len + 1);
    if (parts[part_count] == NULL) {
        for (int i = 0; i < part_count; i++) {
            free(parts[i]);
        }
        free(parts);
        return NULL;
    }
    strcpy(parts[part_count], start);
    part_count++;

    *count = part_count;
    return parts;
}

int main(void) {
    const char *test_string = "HelloWorld thisIsATest";
    int count = 0;
    char **result = split_at_lowercase(test_string, &count);

    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    for (int i = 0; i < count; i++) {
        printf("[%s]\n", result[i]);
        free(result[i]);
    }
    free(result);

    return 0;
}