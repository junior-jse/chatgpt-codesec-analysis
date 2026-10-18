#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

static int select_nth(const int *list, size_t length, size_t n, int *result)
{
    if (list == NULL || result == NULL || n >= length) {
        return -1;
    }

    *result = list[n];
    return 0;
}

int main(void)
{
    int list[] = {10, 20, 30, 40, 50};
    const size_t length = sizeof list / sizeof list[0];
    size_t n;
    int result;

    printf("Enter a zero-based index: ");
    if (scanf("%zu", &n) != 1) {
        fputs("Invalid input.\n", stderr);
        return EXIT_FAILURE;
    }

    if (select_nth(list, length, n, &result) != 0) {
        errno = ERANGE;
        perror("Unable to select item");
        return EXIT_FAILURE;
    }

    printf("%d\n", result);
    return EXIT_SUCCESS;
}