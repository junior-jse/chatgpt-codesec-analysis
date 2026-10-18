#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* replace_with_colon(const char* str, size_t len) {
    if (str == NULL) {
        return NULL;
    }

    char* result = malloc(len + 1);
    if (result == NULL) {
        return NULL;
    }

    const char* targets = " ,.";
    for (size_t i = 0; i < len; ++i) {
        if (strchr(targets, str[i])) {
            result[i] = ':';
        } else {
            result[i] = str[i];
        }
    }
    result[len] = '\0';

    return result;
}

int main(int argc, const char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        return 1;
    }

    size_t len = strlen(argv[1]);
    char* modified = replace_with_colon(argv[1], len);
    if (modified == NULL) {
        fprintf(stderr, "Failed to process string\n");
        return 1;
    }

    printf("%s\n", modified);
    free(modified);

    return 0;
}