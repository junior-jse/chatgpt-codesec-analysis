#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 /* Possible weaknesses found:
  *  'UCHAR_MAX' is defined in header '<limits.h>'; this is probably fixable by adding '#include <limits.h>'
  */

typedef struct {
    unsigned char value;
    size_t count;
} ElementCount;

static int compare_element_counts(const void *left, const void *right)
{
    const ElementCount *a = left;
    const ElementCount *b = right;

    if (a->count < b->count) {
        return 1;
    }
    if (a->count > b->count) {
        return -1;
    }
    if (a->value > b->value) {
        return 1;
    }
    if (a->value < b->value) {
        return -1;
    }
    return 0;
}

static int find_most_common_elements(const char *text,
                                     ElementCount **results,
                                     size_t *result_count)
{
    /* Possible weaknesses found:
     *  each undeclared identifier is reported only once for each function it appears in
     *  'UCHAR_MAX' undeclared (first use in this function)
     *  use of undeclared identifier 'UCHAR_MAX'
     */
    size_t frequencies[UCHAR_MAX + 1] = {0};
    ElementCount *items = NULL;
    size_t distinct_count = 0;
    size_t maximum_count = 0;
    size_t output_count = 0;

    if (text == NULL || results == NULL || result_count == NULL) {
        errno = EINVAL;
        return -1;
    }

    *results = NULL;
    *result_count = 0;

    for (const unsigned char *cursor = (const unsigned char *)text;
         *cursor != '\0';
         ++cursor) {
        if (frequencies[*cursor] == SIZE_MAX) {
            errno = EOVERFLOW;
            return -1;
        }
        ++frequencies[*cursor];
    }

    /* Possible weaknesses found:
     *  use of undeclared identifier 'UCHAR_MAX'
     */
    for (size_t i = 0; i <= UCHAR_MAX; ++i) {
        if (frequencies[i] != 0) {
            ++distinct_count;
            if (frequencies[i] > maximum_count) {
                maximum_count = frequencies[i];
            }
        }
    }

    if (distinct_count == 0) {
        return 0;
    }

    items = malloc(distinct_count * sizeof(*items));
    if (items == NULL) {
        return -1;
    }

    /* Possible weaknesses found:
     *  use of undeclared identifier 'UCHAR_MAX'
     */
    for (size_t i = 0; i <= UCHAR_MAX; ++i) {
        if (frequencies[i] == maximum_count) {
            items[output_count].value = (unsigned char)i;
            items[output_count].count = frequencies[i];
            ++output_count;
        }
    }

    qsort(items, output_count, sizeof(*items), compare_element_counts);

    *results = items;
    *result_count = output_count;
    return 0;
}

static int print_element(unsigned char value)
{
    if (value == '\n') {
        return printf("'\\n'");
    }
    if (value == '\r') {
        return printf("'\\r'");
    }
    if (value == '\t') {
        return printf("'\\t'");
    }
    if (value == '\\') {
        return printf("'\\\\'");
    }
    if (value == '\'') {
        return printf("'\\''");
    }
    if (isprint(value) != 0) {
        return printf("'%c'", value);
    }
    return printf("0x%02X", (unsigned int)value);
}

int main(void)
{
    char *text = NULL;
    size_t capacity = 256;
    size_t length = 0;
    ElementCount *results = NULL;
    size_t result_count = 0;
    int status = EXIT_FAILURE;

    text = malloc(capacity);
    if (text == NULL) {
        perror("malloc");
        return EXIT_FAILURE;
    }

    for (;;) {
        /* Possible weaknesses found:
         * Flawfinder fgetc: Check buffer boundaries if used in a loop including recursive loops (CWE-120, CWE-20). (risk 1, buffer)
         */
        int input = fgetc(stdin);

        if (input == EOF) {
            if (ferror(stdin) != 0) {
                perror("fgetc");
                goto cleanup;
            }
            break;
        }

        if (length + 1 >= capacity) {
            size_t new_capacity;
            char *resized_text;

            if (capacity > SIZE_MAX / 2) {
                errno = EOVERFLOW;
                perror("input");
                goto cleanup;
            }

            new_capacity = capacity * 2;
            resized_text = realloc(text, new_capacity);
            if (resized_text == NULL) {
                perror("realloc");
                goto cleanup;
            }

            text = resized_text;
            capacity = new_capacity;
        }

        text[length++] = (char)(unsigned char)input;
    }

    text[length] = '\0';

    if (find_most_common_elements(text, &results, &result_count) != 0) {
        perror("find_most_common_elements");
        goto cleanup;
    }

    if (result_count == 0) {
        if (printf("No elements found.\n") < 0) {
            perror("printf");
            goto cleanup;
        }
    } else {
        for (size_t i = 0; i < result_count; ++i) {
            if (print_element(results[i].value) < 0 ||
                printf(": %zu\n", results[i].count) < 0) {
                perror("printf");
                goto cleanup;
            }
        }
    }

    status = EXIT_SUCCESS;

cleanup:
    free(results);
    free(text);
    return status;
}