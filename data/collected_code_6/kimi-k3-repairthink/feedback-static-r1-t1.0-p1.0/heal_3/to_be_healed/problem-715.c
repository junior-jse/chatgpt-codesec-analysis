#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>

typedef struct {
    int *data;
    size_t size;
} IntTuple;

void free_tuple(IntTuple *t) {
    if (t != NULL) {
        free(t->data);
        t->data = NULL;
        t->size = 0;
    }
}

int parse_int(const char *str, int *out) {
    char *endptr;
    long val;

    if (str == NULL || out == NULL) {
        return -1;
    }

    errno = 0;
    val = strtol(str, &endptr, 10);

    if (errno == ERANGE || val > INT_MAX || val < INT_MIN) {
        return -1;
    }

    if (endptr == str || *endptr != '\0') {
        return -1;
    }

    *out = (int)val;
    return 0;
}

IntTuple string_to_tuple(const char *input, size_t input_size) {
    IntTuple result = {NULL, 0};
    char *copy = NULL;
    char *token;
    char *saveptr = NULL;
    size_t capacity = 0;
    size_t count = 0;
    size_t length;
    size_t copy_size;

    if (input == NULL || input_size == 0) {
        return result;
    }

    length = strnlen(input, input_size);

    if (length == SIZE_MAX) {
        return result;
    }

    /* Possible weaknesses found:
     *  copy_size is assigned 'length+1' here.
     */
    copy_size = length + 1;

    copy = malloc(copy_size);
    /* Possible weaknesses found:
     *  Assuming condition is false
     */
    if (copy == NULL) {
        return result;
    }

    /* Possible weaknesses found:
     *  Condition 'length<copy_size' is always true
     *  Condition 'length<copy_size' is always true [knownConditionTrueFalse]
     */
    if (length < copy_size) {
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(copy, input, length);
    } else {
        free(copy);
        return result;
    }
    copy[length] = '\0';

    token = strtok_r(copy, " \t\n,", &saveptr);
    while (token != NULL) {
        int value;

        if (parse_int(token, &value) != 0) {
            free(copy);
            free_tuple(&result);
            return result;
        }

        if (count >= capacity) {
            size_t new_capacity;
            int *new_data;

            if (capacity > SIZE_MAX / 2) {
                free(copy);
                free_tuple(&result);
                return result;
            }

            new_capacity = (capacity == 0) ? 8 : capacity * 2;

            if (new_capacity > SIZE_MAX / sizeof(int)) {
                free(copy);
                free_tuple(&result);
                return result;
            }

            new_data = realloc(result.data, new_capacity * sizeof(int));
            if (new_data == NULL) {
                free(copy);
                free_tuple(&result);
                return result;
            }

            result.data = new_data;
            capacity = new_capacity;
        }

        result.data[count] = value;
        count++;
        token = strtok_r(NULL, " \t\n,", &saveptr);
    }

    result.size = count;
    free(copy);
    return result;
}

int main(void) {
    static const char input[] = "1 2 3 4 5";
    IntTuple tuple = string_to_tuple(input, strnlen(input, sizeof(input)));
    size_t i;

    if (tuple.data == NULL && tuple.size == 0) {
        fprintf(stderr, "Error: Failed to parse input string\n");
        return EXIT_FAILURE;
    }

    printf("Tuple: (");
    for (i = 0; i < tuple.size; i++) {
        printf("%d", tuple.data[i]);
        if (i < tuple.size - 1) {
            printf(", ");
        }
    }
    printf(")\n");

    free_tuple(&tuple);
    return EXIT_SUCCESS;
}