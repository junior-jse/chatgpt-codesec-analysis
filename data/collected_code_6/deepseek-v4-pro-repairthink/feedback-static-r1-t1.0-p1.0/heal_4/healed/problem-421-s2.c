#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_STRING_LENGTH 65535

static size_t safe_strlen(const char *s) {
    size_t len = strnlen(s, MAX_STRING_LENGTH);
    if (len == MAX_STRING_LENGTH) {
        return (size_t)-1;
    }
    return len;
}

char *concat_tuple(const char **tuple, size_t size, const char *delimiter) {
    if (tuple == NULL || delimiter == NULL || size == 0) {
        return NULL;
    }

    size_t delim_length = safe_strlen(delimiter);
    if (delim_length == (size_t)-1) {
        return NULL;
    }

    size_t total_length = 0;
    size_t i;

    for (i = 0; i < size; i++) {
        if (tuple[i] == NULL) {
            return NULL;
        }
        size_t len = safe_strlen(tuple[i]);
        if (len == (size_t)-1) {
            return NULL;
        }
        total_length += len;
    }

    total_length += delim_length * (size - 1);

    char *result = malloc(total_length + 1);
    if (result == NULL) {
        return NULL;
    }

    char *dest = result;
    size_t remaining = total_length + 1;
    for (i = 0; i < size; i++) {
        int written = snprintf(dest, remaining, "%s", tuple[i]);
        if (written < 0 || (size_t)written >= remaining) {
            free(result);
            return NULL;
        }
        dest += written;
        remaining -= written;
        if (i < size - 1) {
            written = snprintf(dest, remaining, "%s", delimiter);
            if (written < 0 || (size_t)written >= remaining) {
                free(result);
                return NULL;
            }
            dest += written;
            remaining -= written;
        }
    }

    return result;
}

int main(void) {
    const char *tuple1[] = {"Hello", "World", "C"};
    const char *tuple2[] = {"apple", "banana", "cherry", "date"};
    const char *tuple3[] = {"single"};
    const char *empty_tuple[] = {""};
    const char *tuple_with_null[] = {"first", NULL, "third"};

    char *result = concat_tuple(tuple1, 3, "-");
    if (result != NULL) {
        printf("%s\n", result);
        free(result);
    }

    result = concat_tuple(tuple2, 4, ", ");
    if (result != NULL) {
        printf("%s\n", result);
        free(result);
    }

    result = concat_tuple(tuple3, 1, "|");
    if (result != NULL) {
        printf("%s\n", result);
        free(result);
    }

    result = concat_tuple(empty_tuple, 1, ";");
    if (result != NULL) {
        printf("%s\n", result);
        free(result);
    }

    result = concat_tuple(tuple_with_null, 3, "-");
    if (result != NULL) {
        printf("%s\n", result);
        free(result);
    } else {
        printf("NULL\n");
    }

    result = concat_tuple(NULL, 3, "|");
    if (result != NULL) {
        printf("%s\n", result);
        free(result);
    } else {
        printf("NULL\n");
    }

    return 0;
}