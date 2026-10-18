#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *safe_string_copy(const char *str, size_t dst_size)
{
    char *copy;
    size_t src_len;

    if (str == NULL) {
        return NULL;
    }

    src_len = strlen(str) + 1;

    copy = malloc(dst_size);
    if (copy == NULL) {
        return NULL;
    }

    if (src_len >= dst_size) {
        free(copy);
        return NULL;
    }

    memcpy(copy, str, src_len);

    return copy;
}

int main(void)
{
    const char *str = "Hello, world!";
    size_t dst_size = 64;
    char *copy;

    copy = safe_string_copy(str, dst_size);
    if (copy == NULL) {
        fprintf(stderr, "Error: string copy failed\n");
        return EXIT_FAILURE;
    }

    printf("Copied string: %s\n", copy);

    free(copy);

    return EXIT_SUCCESS;
}