#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int64_t sum;
    uint64_t count;
} State;

static int compare_states(const void *lhs, const void *rhs)
{
    const State *a = lhs;
    const State *b = rhs;

    return (a->sum > b->sum) - (a->sum < b->sum);
}

static int add_checked_i64(int64_t a, int64_t b, int64_t *result)
{
    if ((b > 0 && a > INT64_MAX - b) ||
        (b < 0 && a < INT64_MIN - b)) {
        return 0;
    }

    *result = a + b;
    return 1;
}

static int add_checked_u64(uint64_t a, uint64_t b, uint64_t *result)
{
    if (a > UINT64_MAX - b) {
        return 0;
    }

    *result = a + b;
    return 1;
}

static int count_sequences(const int64_t *values, size_t value_count,
                           size_t length, uint64_t *result)
{
    State *states = malloc(sizeof(*states));
    size_t state_count = 1;

    if (result == NULL || (value_count != 0 && values == NULL)) {
        /* Possible weaknesses found:
         *  Memory leak: states [memleak]
         */
        return EINVAL;
    }

    if (states == NULL) {
        return ENOMEM;
    }

    states[0].sum = 0;
    states[0].count = 1;

    for (size_t position = 0; position < length; ++position) {
        size_t candidate_count = 0;
        State *candidates;

        if (value_count != 0 && state_count > SIZE_MAX / value_count) {
            free(states);
            return EOVERFLOW;
        }

        candidate_count = state_count * value_count;

        if (candidate_count > SIZE_MAX / sizeof(*candidates)) {
            free(states);
            return EOVERFLOW;
        }

        candidates = candidate_count == 0
                         ? NULL
                         : malloc(candidate_count * sizeof(*candidates));

        if (candidate_count != 0 && candidates == NULL) {
            free(states);
            return ENOMEM;
        }

        size_t used = 0;

        for (size_t i = 0; i < state_count; ++i) {
            for (size_t j = 0; j < value_count; ++j) {
                int64_t next_sum;

                if (!add_checked_i64(states[i].sum, values[j], &next_sum)) {
                    free(candidates);
                    free(states);
                    return EOVERFLOW;
                }

                if (next_sum >= 0) {
                    candidates[used].sum = next_sum;
                    candidates[used].count = states[i].count;
                    ++used;
                }
            }
        }

        free(states);
        states = NULL;

        if (used == 0) {
            free(candidates);
            *result = 0;
            return 0;
        }

        qsort(candidates, used, sizeof(*candidates), compare_states);

        size_t merged = 0;

        for (size_t i = 0; i < used; ++i) {
            if (merged != 0 &&
                candidates[merged - 1].sum == candidates[i].sum) {
                uint64_t total;

                if (!add_checked_u64(candidates[merged - 1].count,
                                     candidates[i].count, &total)) {
                    free(candidates);
                    return EOVERFLOW;
                }

                candidates[merged - 1].count = total;
            } else {
                candidates[merged++] = candidates[i];
            }
        }

        states = candidates;
        state_count = merged;
    }

    uint64_t total = 0;

    for (size_t i = 0; i < state_count; ++i) {
        if (!add_checked_u64(total, states[i].count, &total)) {
            free(states);
            return EOVERFLOW;
        }
    }

    free(states);
    *result = total;
    return 0;
}

int main(void)
{
    size_t value_count;
    size_t length;
    int64_t *values = NULL;
    uint64_t answer;
    int status;

    if (scanf("%zu %zu", &value_count, &length) != 2) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }

    if (value_count > SIZE_MAX / sizeof(*values)) {
        fprintf(stderr, "Input is too large\n");
        return EXIT_FAILURE;
    }

    if (value_count != 0) {
        values = malloc(value_count * sizeof(*values));
        if (values == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            return EXIT_FAILURE;
        }
    }

    for (size_t i = 0; i < value_count; ++i) {
        /* Possible weaknesses found:
         * Flawfinder scanf: The scanf() family's %s operation, without a limit specification, permits buffer overflows (CWE-120, CWE-20). Specify a limit to %s, or use a different input function. If the scanf format is influenceable by an attacker, it's exploitable. (risk 4, buffer)
         */
        if (scanf("%" SCNd64, &values[i]) != 1) {
            fprintf(stderr, "Invalid input\n");
            free(values);
            return EXIT_FAILURE;
        }
    }

    status = count_sequences(values, value_count, length, &answer);
    free(values);

    if (status != 0) {
        fprintf(stderr, "Unable to count sequences: %s\n",
                status == ENOMEM ? "out of memory" :
                status == EOVERFLOW ? "numeric overflow" :
                "invalid argument");
        return EXIT_FAILURE;
    }

    printf("%" PRIu64 "\n", answer);
    return EXIT_SUCCESS;
}