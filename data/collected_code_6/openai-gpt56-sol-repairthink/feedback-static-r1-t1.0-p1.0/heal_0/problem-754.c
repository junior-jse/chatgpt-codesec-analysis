#include <stdio.h>
#include <stdlib.h>

static int find_common_index_elements(const int *first,
                                      size_t first_length,
                                      const int *second,
                                      size_t second_length,
                                      const int *third,
                                      size_t third_length,
                                      int **result,
                                      size_t *result_length)
{
    size_t limit;
    size_t count = 0;
    int *common = NULL;

    if (first == NULL || second == NULL || third == NULL ||
        result == NULL || result_length == NULL) {
        return -1;
    }

    *result = NULL;
    *result_length = 0;

    limit = first_length;
    if (second_length < limit) {
        limit = second_length;
    }
    if (third_length < limit) {
        limit = third_length;
    }

    for (size_t i = 0; i < limit; ++i) {
        if (first[i] == second[i] && second[i] == third[i]) {
            ++count;
        }
    }

    if (count == 0) {
        return 0;
    }

    if (count > SIZE_MAX / sizeof(*common)) {
        return -1;
    }

    common = malloc(count * sizeof(*common));
    if (common == NULL) {
        return -1;
    }

    count = 0;
    for (size_t i = 0; i < limit; ++i) {
        if (first[i] == second[i] && second[i] == third[i]) {
            common[count++] = first[i];
        }
    }

    *result = common;
    *result_length = count;

    return 0;
}

int main(void)
{
    const int first[] = {1, 2, 3, 4, 5, 6};
    const int second[] = {1, 7, 3, 8, 5, 9};
    const int third[] = {1, 0, 3, 4, 5, 10};
    int *common = NULL;
    size_t common_length = 0;

    if (find_common_index_elements(first,
                                   sizeof(first) / sizeof(first[0]),
                                   second,
                                   sizeof(second) / sizeof(second[0]),
                                   third,
                                   sizeof(third) / sizeof(third[0]),
                                   &common,
                                   &common_length) != 0) {
        fputs("Failed to find common index elements.\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < common_length; ++i) {
        if (printf("%d%s", common[i], i + 1 < common_length ? " " : "\n") < 0) {
            free(common);
            return EXIT_FAILURE;
        }
    }

    if (common_length == 0 && putchar('\n') == EOF) {
        free(common);
        return EXIT_FAILURE;
    }

    free(common);
    return EXIT_SUCCESS;
}