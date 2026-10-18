#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

int minimum_swaps(const char *source, const char *target, size_t *result)
{
    size_t length;
    size_t source_ones = 0;
    size_t target_ones = 0;
    size_t mismatches = 0;

    if (source == NULL || target == NULL || result == NULL) {
        return -1;
    }

    length = strlen(source);

    if (length != strlen(target)) {
        return -1;
    }

    for (size_t i = 0; i < length; ++i) {
        if ((source[i] != '0' && source[i] != '1') ||
            (target[i] != '0' && target[i] != '1')) {
            return -1;
        }

        source_ones += source[i] == '1';
        target_ones += target[i] == '1';
        mismatches += source[i] != target[i];
    }

    if (source_ones != target_ones) {
        return -1;
    }

    *result = mismatches / 2;
    return 0;
}

int main(void)
{
    char *source = NULL;
    char *target = NULL;
    size_t source_capacity = 0;
    size_t target_capacity = 0;
    size_t swaps;
    ssize_t source_length;
    ssize_t target_length;
    int status = EXIT_FAILURE;

    source_length = getline(&source, &source_capacity, stdin);
    if (source_length < 0) {
        goto cleanup;
    }

    target_length = getline(&target, &target_capacity, stdin);
    if (target_length < 0) {
        goto cleanup;
    }

    if (source_length > 0 && source[source_length - 1] == '\n') {
        source[--source_length] = '\0';
    }
    if (target_length > 0 && target[target_length - 1] == '\n') {
        target[--target_length] = '\0';
    }

    if (source_length > 0 && source[source_length - 1] == '\r') {
        source[--source_length] = '\0';
    }
    if (target_length > 0 && target[target_length - 1] == '\r') {
        target[--target_length] = '\0';
    }

    if (minimum_swaps(source, target, &swaps) != 0) {
        if (puts("-1") == EOF) {
            goto cleanup;
        }
    } else {
        if (printf("%zu\n", swaps) < 0) {
            goto cleanup;
        }
    }

    status = EXIT_SUCCESS;

cleanup:
    free(source);
    free(target);
    return status;
}