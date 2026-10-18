#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *value;
    size_t count;
} Run;

typedef struct {
    Run *runs;
    size_t size;
} EncodedList;

static char *duplicate_string(const char *source)
{
    size_t length;
    char *copy;

    if (source == NULL) {
        return NULL;
    }

    length = strlen(source);
    if (length == SIZE_MAX) {
        return NULL;
    }

    copy = malloc(length + 1U);
    if (copy == NULL) {
        return NULL;
    }

    memcpy(copy, source, length + 1U);
    return copy;
}

static void free_encoded_list(EncodedList *encoded)
{
    size_t i;

    if (encoded == NULL) {
        return;
    }

    for (i = 0U; i < encoded->size; ++i) {
        free(encoded->runs[i].value);
    }

    free(encoded->runs);
    encoded->runs = NULL;
    encoded->size = 0U;
}

static int modified_run_length_encode(
    const char *const input[],
    size_t input_size,
    EncodedList *result)
{
    Run *runs;
    size_t run_count;
    size_t i;

    if (result == NULL || (input == NULL && input_size != 0U)) {
        return -1;
    }

    result->runs = NULL;
    result->size = 0U;

    if (input_size == 0U) {
        return 0;
    }

    if (input_size > SIZE_MAX / sizeof(*runs)) {
        return -1;
    }

    runs = calloc(input_size, sizeof(*runs));
    if (runs == NULL) {
        return -1;
    }

    run_count = 0U;
    i = 0U;

    while (i < input_size) {
        size_t count = 1U;
        char *value;

        if (input[i] == NULL) {
            result->runs = runs;
            result->size = run_count;
            free_encoded_list(result);
            return -1;
        }

        while (i + count < input_size) {
            if (input[i + count] == NULL) {
                result->runs = runs;
                result->size = run_count;
                free_encoded_list(result);
                return -1;
            }

            if (strcmp(input[i], input[i + count]) != 0) {
                break;
            }

            ++count;
        }

        value = duplicate_string(input[i]);
        if (value == NULL) {
            result->runs = runs;
            result->size = run_count;
            free_encoded_list(result);
            return -1;
        }

        runs[run_count].value = value;
        runs[run_count].count = count;
        ++run_count;
        i += count;
    }

    result->runs = runs;
    result->size = run_count;
    return 0;
}

static int print_encoded_list(const EncodedList *encoded)
{
    size_t i;

    if (encoded == NULL || (encoded->runs == NULL && encoded->size != 0U)) {
        return -1;
    }

    if (putchar('[') == EOF) {
        return -1;
    }

    for (i = 0U; i < encoded->size; ++i) {
        int status;

        if (i != 0U && fputs(", ", stdout) == EOF) {
            return -1;
        }

        if (encoded->runs[i].count == 1U) {
            status = printf("\"%s\"", encoded->runs[i].value);
        } else {
            status = printf(
                "[%zu, \"%s\"]",
                encoded->runs[i].count,
                encoded->runs[i].value);
        }

        if (status < 0) {
            return -1;
        }
    }

    if (puts("]") == EOF) {
        return -1;
    }

    return 0;
}

int main(void)
{
    const char *input[] = {
        "a", "a", "a", "b", "c", "c", "a", "a", "d", "e", "e", "e", "e"
    };
    EncodedList encoded = {NULL, 0U};
    int status;

    status = modified_run_length_encode(
        input,
        sizeof(input) / sizeof(input[0]),
        &encoded);

    if (status != 0) {
        fputs("Encoding failed.\n", stderr);
        return EXIT_FAILURE;
    }

    status = print_encoded_list(&encoded);
    free_encoded_list(&encoded);

    if (status != 0) {
        fputs("Output failed.\n", stderr);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}