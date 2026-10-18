#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_STRING_LENGTH 4096

typedef struct {
    char *first;
    char *second;
} Tuple;

typedef struct {
    Tuple *tuples;
    size_t size;
    size_t capacity;
} TupleList;

static size_t bounded_strlen(const char *s)
{
    size_t len = 0;
    if (s == NULL) {
        return 0;
    }
    while (len < MAX_STRING_LENGTH && s[len] != '\0') {
        len++;
    }
    return len;
}

static int is_valid_string(const char *s)
{
    return s != NULL && bounded_strlen(s) < MAX_STRING_LENGTH;
}

static char *join_strings(const char *a, const char *b)
{
    size_t a_len;
    size_t b_len;
    size_t total;
    char *joined;
    int written;

    if (!is_valid_string(a) || !is_valid_string(b)) {
        return NULL;
    }
    a_len = bounded_strlen(a);
    b_len = bounded_strlen(b);
    if (a_len > SIZE_MAX - b_len - 2) {
        return NULL;
    }
    total = a_len + b_len + 2;
    joined = malloc(total);
    if (joined == NULL) {
        return NULL;
    }
    written = snprintf(joined, total, "%s %s", a, b);
    if (written < 0 || (size_t)written >= total) {
        free(joined);
        return NULL;
    }
    return joined;
}

int init_list(TupleList *list, size_t capacity)
{
    if (list == NULL || capacity == 0) {
        return -1;
    }
    if (capacity > SIZE_MAX / sizeof(Tuple)) {
        return -1;
    }
    list->tuples = malloc(capacity * sizeof(Tuple));
    if (list->tuples == NULL) {
        return -1;
    }
    list->size = 0;
    list->capacity = capacity;
    return 0;
}

int add_tuple(TupleList *list, const char *first, const char *second)
{
    size_t first_len;
    size_t second_len;

    if (list == NULL || !is_valid_string(first) || !is_valid_string(second)) {
        return -1;
    }
    if (list->size >= list->capacity) {
        return -1;
    }

    first_len = bounded_strlen(first);
    second_len = bounded_strlen(second);

    list->tuples[list->size].first = malloc(first_len + 1);
    if (list->tuples[list->size].first == NULL) {
        return -1;
    }
    memcpy(list->tuples[list->size].first, first, first_len + 1);

    list->tuples[list->size].second = malloc(second_len + 1);
    if (list->tuples[list->size].second == NULL) {
        free(list->tuples[list->size].first);
        list->tuples[list->size].first = NULL;
        return -1;
    }
    memcpy(list->tuples[list->size].second, second, second_len + 1);

    list->size++;
    return 0;
}

void free_list(TupleList *list)
{
    size_t i;
    if (list == NULL) {
        return;
    }
    for (i = 0; i < list->size; i++) {
        free(list->tuples[i].first);
        free(list->tuples[i].second);
    }
    free(list->tuples);
    list->tuples = NULL;
    list->size = 0;
    list->capacity = 0;
}

TupleList join_similar_tuples(const TupleList *input)
{
    TupleList result;
    size_t i, j;
    int *merged;
    int found;

    result.tuples = NULL;
    result.size = 0;
    result.capacity = 0;

    if (input == NULL || input->size == 0) {
        return result;
    }

    if (init_list(&result, input->size) != 0) {
        return result;
    }

    merged = calloc(input->size, sizeof(int));
    if (merged == NULL) {
        free_list(&result);
        return result;
    }

    for (i = 0; i < input->size; i++) {
        if (merged[i]) {
            continue;
        }
        found = 0;
        for (j = i + 1; j < input->size; j++) {
            if (merged[j]) {
                continue;
            }
            if (strncmp(input->tuples[i].first, input->tuples[j].first,
                        MAX_STRING_LENGTH) == 0) {
                char *new_second;
                if (!found) {
                    new_second = join_strings(input->tuples[i].second,
                                              input->tuples[j].second);
                    if (new_second == NULL) {
                        free(merged);
                        free_list(&result);
                        result.tuples = NULL;
                        return result;
                    }
                    if (add_tuple(&result, input->tuples[i].first,
                                  new_second) != 0) {
                        free(new_second);
                        free(merged);
                        free_list(&result);
                        result.tuples = NULL;
                        return result;
                    }
                    free(new_second);
                    found = 1;
                } else {
                    new_second = join_strings(result.tuples[result.size - 1].second,
                                              input->tuples[j].second);
                    if (new_second == NULL) {
                        free(merged);
                        free_list(&result);
                        result.tuples = NULL;
                        return result;
                    }
                    free(result.tuples[result.size - 1].second);
                    result.tuples[result.size - 1].second = new_second;
                }
                merged[j] = 1;
            }
        }
        if (!found) {
            if (add_tuple(&result, input->tuples[i].first,
                          input->tuples[i].second) != 0) {
                free(merged);
                free_list(&result);
                result.tuples = NULL;
                return result;
            }
        }
        merged[i] = 1;
    }

    free(merged);
    return result;
}

int main(void)
{
    TupleList input;
    TupleList output;
    size_t i;

    if (init_list(&input, 10) != 0) {
        fprintf(stderr, "Failed to initialize list\n");
        return 1;
    }

    if (add_tuple(&input, "apple", "red") != 0 ||
        add_tuple(&input, "banana", "yellow") != 0 ||
        add_tuple(&input, "apple", "green") != 0 ||
        add_tuple(&input, "cherry", "red") != 0 ||
        add_tuple(&input, "banana", "ripe") != 0 ||
        add_tuple(&input, "apple", "sweet") != 0) {
        fprintf(stderr, "Failed to add tuples\n");
        free_list(&input);
        return 1;
    }

    output = join_similar_tuples(&input);
    if (output.tuples == NULL && input.size > 0) {
        fprintf(stderr, "Failed to join tuples\n");
        free_list(&input);
        return 1;
    }

    printf("Joined tuples:\n");
    for (i = 0; i < output.size; i++) {
        printf("(%s, %s)\n", output.tuples[i].first, output.tuples[i].second);
    }

    free_list(&input);
    free_list(&output);

    return 0;
}