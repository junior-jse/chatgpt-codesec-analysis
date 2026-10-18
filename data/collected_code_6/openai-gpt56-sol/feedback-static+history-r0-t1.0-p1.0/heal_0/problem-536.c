#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

static int select_nth(const int *list, size_t length, size_t n, int *result)
{
    if (list == NULL || result == NULL || n >= length) {
        return 0;
    }

    *result = list[n];
    return 1;
}

int main(int argc, char *argv[])
{
    char *end = NULL;
    unsigned long long parsed_n;
    size_t n;
    int selected;
    const int list[] = {10, 20, 30, 40, 50};
    const size_t length = sizeof(list) / sizeof(list[0]);

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <zero-based-index>\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (argv[1][0] == '-') {
        fprintf(stderr, "Index must be a non-negative integer.\n");
        return EXIT_FAILURE;
    }

    errno = 0;
    parsed_n = strtoull(argv[1], &end, 10);

    if (errno == ERANGE || end == argv[1] || *end != '\0' ||
        parsed_n > (unsigned long long)SIZE_MAX) {
        fprintf(stderr, "Invalid index.\n");
        return EXIT_FAILURE;
    }

    n = (size_t)parsed_n;

    if (!select_nth(list, length, n, &selected)) {
        fprintf(stderr, "Index is out of range.\n");
        return EXIT_FAILURE;
    }

    printf("%d\n", selected);
    return EXIT_SUCCESS;
}