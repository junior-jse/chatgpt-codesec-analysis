#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

static int select_nth(const int *list, size_t length, size_t n, int *result)
{
    if (list == NULL || result == NULL || n >= length) {
        return EINVAL;
    }

    *result = list[n];
    return 0;
}

int main(void)
{
    const int list[] = {10, 20, 30, 40, 50};
    const size_t length = sizeof list / sizeof list[0];
    const size_t n = 2U;
    int value = 0;
    int status = select_nth(list, length, n, &value);

    if (status != 0) {
        fprintf(stderr, "Unable to select item at index %zu\n", n);
        return EXIT_FAILURE;
    }

    printf("%d\n", value);
    return EXIT_SUCCESS;
}