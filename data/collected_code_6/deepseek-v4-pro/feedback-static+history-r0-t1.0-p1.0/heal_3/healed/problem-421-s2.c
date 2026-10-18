#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static size_t safe_strlen(const char *str, size_t max_len) {
    size_t len = 0;
    if (str != NULL) {
        while (len < max_len && str[len] != '\0') {
            len++;
        }
    }
    return len;
}

char *concat_tuple(const char **tuple, size_t size, const char *delimiter) {
    if (tuple == NULL || delimiter == NULL || size == 0) {
        return NULL;
    }

    size_t total_length = 0;
    size_t delim_length = safe_strlen(delimiter, SIZE_MAX);
    size_t *lengths = (size_t *)malloc(size * sizeof(size_t));
    if (lengths == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < size; i++) {
        if (tuple[i] == NULL) {
            free(lengths);
            return NULL;
        }
        lengths[i] = safe_strlen(tuple[i], SIZE_MAX);
        if (total_length > SIZE_MAX - lengths[i]) {
            free(lengths);
            return NULL;
        }
        total_length += lengths[i];
    }

    if (size > 1) {
        if (delim_length > 0 && total_length > SIZE_MAX - (delim_length * (size - 1))) {
            free(lengths);
            return NULL;
        }
        total_length += delim_length * (size - 1);
    }

    if (total_length >= SIZE_MAX) {
        free(lengths);
        return NULL;
    }

    char *result = (char *)malloc(total_length + 1);
    if (result == NULL) {
        free(lengths);
        return NULL;
    }

    size_t offset = 0;
    for (size_t i = 0; i < size; i++) {
        if (offset + lengths[i] > total_length) {
            free(lengths);
            free(result);
            return NULL;
        }
        size_t remaining = total_length - offset;
        size_t copy_len = lengths[i] < remaining ? lengths[i] : remaining;
        memcpy(result + offset, tuple[i], copy_len);
        offset += copy_len;
        if (i < size - 1) {
            if (offset + delim_length > total_length) {
                free(lengths);
                free(result);
                return NULL;
            }
            remaining = total_length - offset;
            size_t delim_copy_len = delim_length < remaining ? delim_length : remaining;
            memcpy(result + offset, delimiter, delim_copy_len);
            offset += delim_copy_len;
        }
    }
    result[total_length] = '\0';

    free(lengths);
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