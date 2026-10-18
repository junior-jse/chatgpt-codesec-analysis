#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void free_strings(char **strings, size_t count)
{
    if (strings == NULL) {
        return;
    }

    for (size_t i = 0; i < count; ++i) {
        free(strings[i]);
    }

    free(strings);
}

static int append_string(char ***strings, size_t *count, const char *start,
                         size_t length)
{
    if (*count == SIZE_MAX / sizeof(**strings)) {
        return 0;
    }

    char *part = malloc(length + 1);
    if (part == NULL) {
        return 0;
    }

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(part, start, length);
    part[length] = '\0';

    char **resized = realloc(*strings, (*count + 1) * sizeof(**strings));
    if (resized == NULL) {
        free(part);
        return 0;
    }

    resized[*count] = part;
    *strings = resized;
    ++*count;

    return 1;
}

static char **split_at_lowercase(const char *input, size_t *count)
{
    if (input == NULL || count == NULL) {
        return NULL;
    }

    char **parts = NULL;
    size_t part_count = 0;
    const char *start = input;

    for (const char *current = input; *current != '\0'; ++current) {
        if (islower((unsigned char)*current) != 0) {
            if (current != start &&
                !append_string(&parts, &part_count, start,
                               (size_t)(current - start))) {
                free_strings(parts, part_count);
                return NULL;
            }

            start = current + 1;
        }
    }

    if (*start != '\0') {
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        size_t length = strlen(start);

        if (!append_string(&parts, &part_count, start, length)) {
            free_strings(parts, part_count);
            return NULL;
        }
    }

    *count = part_count;
    return parts;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        return EXIT_FAILURE;
    }

    size_t count = 0;
    char **parts = split_at_lowercase(argv[1], &count);

    if (parts == NULL && count == 0 && argv[1][0] != '\0') {
        for (const char *p = argv[1]; *p != '\0'; ++p) {
            if (islower((unsigned char)*p) == 0) {
                fprintf(stderr, "Unable to split string\n");
                return EXIT_FAILURE;
            }
        }
    }

    for (size_t i = 0; i < count; ++i) {
        puts(parts[i]);
    }

    free_strings(parts, count);
    return EXIT_SUCCESS;
}