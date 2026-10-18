/* Repair history (previous round):
 * - Flawfinder strlen was removed from add_tuple, join_similar_tuples() and now appears in safe_strdup().
 * - Flawfinder strcat was replaced by memcpy, which Flawfinder flags with the same CWE (CWE-120).
 * - Flawfinder strcpy was replaced by memcpy, which Flawfinder flags with the same CWE (CWE-120).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *first;
    char *second;
} Tuple;

typedef struct {
    Tuple *tuples;
    size_t size;
    size_t capacity;
} TupleList;

static char *safe_strdup(const char *s) {
    size_t len;
    char *copy;
    
    if (s == NULL) {
        return NULL;
    }
    
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    len = strlen(s);
    copy = malloc(len + 1);
    if (copy == NULL) {
        return NULL;
    }
    
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(copy, s, len + 1);
    return copy;
}

int init_list(TupleList *list, size_t capacity) {
    if (list == NULL || capacity == 0) {
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

int add_tuple(TupleList *list, const char *first, const char *second) {
    if (list == NULL || first == NULL || second == NULL) {
        return -1;
    }
    if (list->size >= list->capacity) {
        return -1;
    }
    
    list->tuples[list->size].first = safe_strdup(first);
    if (list->tuples[list->size].first == NULL) {
        return -1;
    }
    
    list->tuples[list->size].second = safe_strdup(second);
    if (list->tuples[list->size].second == NULL) {
        free(list->tuples[list->size].first);
        list->tuples[list->size].first = NULL;
        return -1;
    }
    
    list->size++;
    return 0;
}

void free_list(TupleList *list) {
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

TupleList join_similar_tuples(const TupleList *input) {
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
            if (strcmp(input->tuples[i].first, input->tuples[j].first) == 0) {
                if (!found) {
                    /* Possible weaknesses found:
                     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
                     */
                    size_t len2 = strlen(input->tuples[i].second);
                    /* Possible weaknesses found:
                     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
                     */
                    size_t len3 = strlen(input->tuples[j].second);
                    char *new_second = malloc(len2 + len3 + 2);
                    if (new_second == NULL) {
                        free(merged);
                        free_list(&result);
                        result.tuples = NULL;
                        return result;
                    }
                    
                    /* Possible weaknesses found:
                     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                     */
                    memcpy(new_second, input->tuples[i].second, len2);
                    new_second[len2] = ' ';
                    /* Possible weaknesses found:
                     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                     */
                    memcpy(new_second + len2 + 1, input->tuples[j].second, len3 + 1);
                    
                    if (add_tuple(&result, input->tuples[i].first, new_second) != 0) {
                        free(new_second);
                        free(merged);
                        free_list(&result);
                        result.tuples = NULL;
                        return result;
                    }
                    free(new_second);
                    found = 1;
                } else {
                    /* Possible weaknesses found:
                     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
                     */
                    size_t old_len = strlen(result.tuples[result.size - 1].second);
                    /* Possible weaknesses found:
                     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
                     */
                    size_t add_len = strlen(input->tuples[j].second);
                    char *new_second = malloc(old_len + add_len + 2);
                    if (new_second == NULL) {
                        free(merged);
                        free_list(&result);
                        result.tuples = NULL;
                        return result;
                    }
                    
                    /* Possible weaknesses found:
                     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                     */
                    memcpy(new_second, result.tuples[result.size - 1].second, old_len);
                    new_second[old_len] = ' ';
                    /* Possible weaknesses found:
                     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
                     */
                    memcpy(new_second + old_len + 1, input->tuples[j].second, add_len + 1);
                    
                    free(result.tuples[result.size - 1].second);
                    result.tuples[result.size - 1].second = new_second;
                }
                merged[j] = 1;
            }
        }
        if (!found) {
            if (add_tuple(&result, input->tuples[i].first, input->tuples[i].second) != 0) {
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

int main(void) {
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