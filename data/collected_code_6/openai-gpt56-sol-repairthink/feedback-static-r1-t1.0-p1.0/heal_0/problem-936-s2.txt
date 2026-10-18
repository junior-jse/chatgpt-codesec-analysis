#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *key;
    char *value;
} Tuple;

static void free_tuples(Tuple *tuples, size_t count)
{
    if (tuples == NULL) {
        return;
    }

    for (size_t i = 0; i < count; ++i) {
        free(tuples[i].key);
        free(tuples[i].value);
    }

    free(tuples);
}

static int rearrange_tuples(const Tuple *tuples,
                            size_t tuple_count,
                            const char *const *ordered_keys,
                            size_t key_count,
                            Tuple **result)
{
    Tuple *arranged = NULL;
    unsigned char *used = NULL;
    size_t output_index = 0;

    if (result == NULL ||
        (tuple_count > 0 && tuples == NULL) ||
        (key_count > 0 && ordered_keys == NULL)) {
        return -1;
    }

    *result = NULL;

    if (tuple_count == 0) {
        return 0;
    }

    if (tuple_count > SIZE_MAX / sizeof(*arranged)) {
        return -1;
    }

    arranged = calloc(tuple_count, sizeof(*arranged));
    used = calloc(tuple_count, sizeof(*used));
    if (arranged == NULL || used == NULL) {
        free(arranged);
        free(used);
        return -1;
    }

    for (size_t i = 0; i < key_count; ++i) {
        if (ordered_keys[i] == NULL) {
            free(used);
            free_tuples(arranged, output_index);
            return -1;
        }

        for (size_t j = 0; j < tuple_count; ++j) {
            if (!used[j] && tuples[j].key != NULL &&
                strcmp(tuples[j].key, ordered_keys[i]) == 0) {
                arranged[output_index].key = strdup(tuples[j].key);
                arranged[output_index].value =
                    tuples[j].value != NULL ? strdup(tuples[j].value) : strdup("");

                if (arranged[output_index].key == NULL ||
                    arranged[output_index].value == NULL) {
                    free(used);
                    free_tuples(arranged, output_index + 1);
                    return -1;
                }

                used[j] = 1;
                ++output_index;
            }
        }
    }

    for (size_t i = 0; i < tuple_count; ++i) {
        if (!used[i]) {
            if (tuples[i].key == NULL) {
                free(used);
                free_tuples(arranged, output_index);
                return -1;
            }

            arranged[output_index].key = strdup(tuples[i].key);
            arranged[output_index].value =
                tuples[i].value != NULL ? strdup(tuples[i].value) : strdup("");

            if (arranged[output_index].key == NULL ||
                arranged[output_index].value == NULL) {
                free(used);
                free_tuples(arranged, output_index + 1);
                return -1;
            }

            ++output_index;
        }
    }

    free(used);
    *result = arranged;
    return 0;
}

int main(void)
{
    const Tuple tuples[] = {
        {"gamma", "3"},
        {"alpha", "1"},
        {"delta", "4"},
        {"beta", "2"}
    };
    const char *ordered_keys[] = {
        "alpha",
        "beta",
        "gamma"
    };
    const size_t tuple_count = sizeof(tuples) / sizeof(tuples[0]);
    const size_t key_count = sizeof(ordered_keys) / sizeof(ordered_keys[0]);
    Tuple *arranged = NULL;

    if (rearrange_tuples(tuples, tuple_count, ordered_keys, key_count,
                         &arranged) != 0) {
        fputs("Failed to rearrange tuples.\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < tuple_count; ++i) {
        if (printf("(%s, %s)\n", arranged[i].key, arranged[i].value) < 0) {
            free_tuples(arranged, tuple_count);
            return EXIT_FAILURE;
        }
    }

    free_tuples(arranged, tuple_count);

    if (fclose(stdout) == EOF) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}