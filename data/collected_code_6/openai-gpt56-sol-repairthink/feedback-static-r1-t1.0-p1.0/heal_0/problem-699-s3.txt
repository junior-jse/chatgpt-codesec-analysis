#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

static int minimum_swaps(const char *source, const char *target, size_t *result)
{
    size_t length;
    size_t source_ones = 0;
    size_t target_ones = 0;
    size_t mismatches_01 = 0;
    size_t mismatches_10 = 0;

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

        if (source[i] == '1') {
            ++source_ones;
        }

        if (target[i] == '1') {
            ++target_ones;
        }

        if (source[i] == '0' && target[i] == '1') {
            ++mismatches_01;
        } else if (source[i] == '1' && target[i] == '0') {
            ++mismatches_10;
        }
    }

    if (source_ones != target_ones || mismatches_01 != mismatches_10) {
        return 0;
    }

    *result = mismatches_01;
    return 1;
}

int main(void)
{
    char *source = NULL;
    char *target = NULL;
    size_t source_capacity = 0;
    size_t target_capacity = 0;
    size_t swaps = 0;
    ssize_t source_length;
    ssize_t target_length;
    int status;

    source_length = getline(&source, &source_capacity, stdin);
    if (source_length < 0) {
        free(source);
        free(target);
        return EXIT_FAILURE;
    }

    target_length = getline(&target, &target_capacity, stdin);
    if (target_length < 0) {
        free(source);
        free(target);
        return EXIT_FAILURE;
    }

    if (source_length > 0 && source[source_length - 1] == '\n') {
        source[source_length - 1] = '\0';
    }

    if (target_length > 0 && target[target_length - 1] == '\n') {
        target[target_length - 1] = '\0';
    }

    status = minimum_swaps(source, target, &swaps);

    if (status == 1) {
        printf("%zu\n", swaps);
    } else if (status == 0) {
        printf("-1\n");
    } else {
        free(source);
        free(target);
        return EXIT_FAILURE;
    }

    free(source);
    free(target);
    return EXIT_SUCCESS;
}