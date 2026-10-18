#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char **split_at_lowercase(const char *str, int *count) {
    if (!str || !count) {
        if (count) *count = 0;
        return NULL;
    }
    
    size_t len = strlen(str);
    if (len == 0) {
        *count = 0;
        return NULL;
    }
    
    int max_parts = (int)len + 1;
    char **parts = (char **)malloc(max_parts * sizeof(char *));
    if (!parts) {
        *count = 0;
        return NULL;
    }
    
    int part_count = 0;
    size_t start = 0;
    
    for (size_t i = 0; i <= len; i++) {
        if (i == len || islower((unsigned char)str[i])) {
            size_t part_len = i - start;
            if (part_len > 0) {
                char *part = (char *)malloc(part_len + 1);
                if (!part) {
                    for (int j = 0; j < part_count; j++) {
                        free(parts[j]);
                    }
                    free(parts);
                    *count = 0;
                    return NULL;
                }
                memcpy(part, str + start, part_len);
                part[part_len] = '\0';
                parts[part_count] = part;
                part_count++;
            }
            start = i + 1;
        }
    }
    
    *count = part_count;
    if (part_count == 0) {
        free(parts);
        return NULL;
    }
    
    return parts;
}

void free_split_result(char **parts, int count) {
    if (!parts) return;
    for (int i = 0; i < count; i++) {
        free(parts[i]);
    }
    free(parts);
}

int main(void) {
    const char *test_string = "HelloWorldThisIsATest";
    int count = 0;
    
    char **result = split_at_lowercase(test_string, &count);
    if (result) {
        printf("Split into %d parts:\n", count);
        for (int i = 0; i < count; i++) {
            printf("[%d]: '%s'\n", i, result[i]);
        }
        free_split_result(result, count);
    } else {
        printf("No parts or error occurred (count=%d)\n", count);
    }
    
    return 0;
}