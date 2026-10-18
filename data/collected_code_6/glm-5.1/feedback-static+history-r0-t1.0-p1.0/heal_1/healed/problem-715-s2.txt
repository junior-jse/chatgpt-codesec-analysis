#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

typedef struct {
    int *elements;
    size_t size;
} IntTuple;

IntTuple *str_to_int_tuple(const char *str) {
    if (str == NULL) {
        return NULL;
    }

    size_t capacity = 16;
    size_t count = 0;
    int *nums = (int *)malloc(capacity * sizeof(int));
    if (nums == NULL) {
        return NULL;
    }

    const char *ptr = str;
    while (*ptr != '\0') {
        char *endptr;
        errno = 0;
        long val = strtol(ptr, &endptr, 10);

        if (ptr == endptr) {
            if (*endptr != '\0') {
                ptr++;
            }
            continue;
        }

        if (errno == ERANGE || val > INT_MAX || val < INT_MIN) {
            free(nums);
            return NULL;
        }

        if (count >= capacity) {
            capacity *= 2;
            int *temp = (int *)realloc(nums, capacity * sizeof(int));
            if (temp == NULL) {
                free(nums);
                return NULL;
            }
            nums = temp;
        }

        nums[count++] = (int)val;
        ptr = endptr;
    }

    IntTuple *tuple = (IntTuple *)malloc(sizeof(IntTuple));
    if (tuple == NULL) {
        free(nums);
        return NULL;
    }

    if (count == 0) {
        free(nums);
        tuple->elements = NULL;
        tuple->size = 0;
    } else {
        int *final_nums = (int *)realloc(nums, count * sizeof(int));
        if (final_nums == NULL) {
            tuple->elements = nums;
        } else {
            tuple->elements = final_nums;
        }
        tuple->size = count;
    }

    return tuple;
}

void free_int_tuple(IntTuple *tuple) {
    if (tuple != NULL) {
        free(tuple->elements);
        free(tuple);
    }
}

int main(void) {
    const char *input = "10 20 30 40 50";
    IntTuple *tuple = str_to_int_tuple(input);

    if (tuple == NULL) {
        return EXIT_FAILURE;
    }

    printf("(");
    for (size_t i = 0; i < tuple->size; i++) {
        printf("%d", tuple->elements[i]);
        if (i < tuple->size - 1) {
            printf(", ");
        }
    }
    printf(")\n");

    free_int_tuple(tuple);

    return EXIT_SUCCESS;
}