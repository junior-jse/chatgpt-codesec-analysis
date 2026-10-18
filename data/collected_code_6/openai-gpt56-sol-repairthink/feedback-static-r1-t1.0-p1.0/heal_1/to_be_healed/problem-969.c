#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *initial;
    char **elements;
    size_t count;
} Tuple;

static char *duplicate_string(const char *source)
{
    size_t length;
    char *copy;

    if (source == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    length = strlen(source);
    if (length == SIZE_MAX) {
        return NULL;
    }

    copy = malloc(length + 1U);
    if (copy == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(copy, source, length + 1U);
    return copy;
}

static void free_tuple(Tuple *tuple)
{
    size_t i;

    if (tuple == NULL) {
        return;
    }

    free(tuple->initial);
    tuple->initial = NULL;

    for (i = 0U; i < tuple->count; ++i) {
        free(tuple->elements[i]);
    }

    free(tuple->elements);
    tuple->elements = NULL;
    tuple->count = 0U;
}

static void free_tuples(Tuple *tuples, size_t count)
{
    size_t i;

    if (tuples == NULL) {
        return;
    }

    for (i = 0U; i < count; ++i) {
        free_tuple(&tuples[i]);
    }

    free(tuples);
}

static int append_element(Tuple *tuple, const char *element)
{
    char **resized;
    char *copy;

    if (tuple == NULL || element == NULL ||
        tuple->count == SIZE_MAX ||
        tuple->count + 1U > SIZE_MAX / sizeof(*tuple->elements)) {
        return 0;
    }

    copy = duplicate_string(element);
    if (copy == NULL) {
        return 0;
    }

    resized = realloc(tuple->elements,
                      (tuple->count + 1U) * sizeof(*tuple->elements));
    if (resized == NULL) {
        free(copy);
        return 0;
    }

    tuple->elements = resized;
    tuple->elements[tuple->count] = copy;
    ++tuple->count;
    return 1;
}

static int add_tuple(Tuple **tuples, size_t *count, const char *initial)
{
    Tuple *resized;
    char *initial_copy;

    if (tuples == NULL || count == NULL || initial == NULL ||
        *count == SIZE_MAX ||
        *count + 1U > SIZE_MAX / sizeof(**tuples)) {
        return 0;
    }

    initial_copy = duplicate_string(initial);
    if (initial_copy == NULL) {
        return 0;
    }

    resized = realloc(*tuples, (*count + 1U) * sizeof(**tuples));
    if (resized == NULL) {
        free(initial_copy);
        return 0;
    }

    *tuples = resized;
    (*tuples)[*count].initial = initial_copy;
    (*tuples)[*count].elements = NULL;
    (*tuples)[*count].count = 0U;
    ++(*count);
    return 1;
}

static int join_tuples(const Tuple *input, size_t input_count,
                       Tuple **output, size_t *output_count)
{
    size_t i;
    size_t j;
    size_t destination;

    if (output == NULL || output_count == NULL ||
        (input == NULL && input_count != 0U)) {
        return 0;
    }

    *output = NULL;
    *output_count = 0U;

    for (i = 0U; i < input_count; ++i) {
        if (input[i].initial == NULL ||
            (input[i].elements == NULL && input[i].count != 0U)) {
            free_tuples(*output, *output_count);
            *output = NULL;
            *output_count = 0U;
            return 0;
        }

        for (destination = 0U; destination < *output_count; ++destination) {
            if (strcmp((*output)[destination].initial, input[i].initial) == 0) {
                break;
            }
        }

        if (destination == *output_count) {
            if (!add_tuple(output, output_count, input[i].initial)) {
                free_tuples(*output, *output_count);
                *output = NULL;
                *output_count = 0U;
                return 0;
            }
        }

        for (j = 0U; j < input[i].count; ++j) {
            if (input[i].elements[j] == NULL ||
                !append_element(&(*output)[destination],
                                input[i].elements[j])) {
                free_tuples(*output, *output_count);
                *output = NULL;
                *output_count = 0U;
                return 0;
            }
        }
    }

    return 1;
}

int main(void)
{
    char *first_elements[] = {"1", "2"};
    char *second_elements[] = {"3"};
    char *third_elements[] = {"4", "5"};
    char *fourth_elements[] = {"6"};
    Tuple input[] = {
        {"A", first_elements, 2U},
        {"B", second_elements, 1U},
        {"A", third_elements, 2U},
        {"C", fourth_elements, 1U}
    };
    Tuple *joined = NULL;
    size_t joined_count = 0U;
    size_t i;
    size_t j;

    if (!join_tuples(input, sizeof(input) / sizeof(input[0]),
                     &joined, &joined_count)) {
        fputs("Failed to join tuples.\n", stderr);
        return EXIT_FAILURE;
    }

    for (i = 0U; i < joined_count; ++i) {
        if (printf("(%s", joined[i].initial) < 0) {
            free_tuples(joined, joined_count);
            return EXIT_FAILURE;
        }

        for (j = 0U; j < joined[i].count; ++j) {
            if (printf(", %s", joined[i].elements[j]) < 0) {
                free_tuples(joined, joined_count);
                return EXIT_FAILURE;
            }
        }

        if (puts(")") == EOF) {
            free_tuples(joined, joined_count);
            return EXIT_FAILURE;
        }
    }

    free_tuples(joined, joined_count);
    return EXIT_SUCCESS;
}