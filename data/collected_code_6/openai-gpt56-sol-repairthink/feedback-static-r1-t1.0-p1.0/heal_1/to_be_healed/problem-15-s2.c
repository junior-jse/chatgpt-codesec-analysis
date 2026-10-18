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
    char *part = malloc(length + 1);
    if (part == NULL) {
        return -1;
    }

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(part, start, length);
    part[length] = '\0';

    char **resized = realloc(*strings, (*count + 1) * sizeof(*resized));
    if (resized == NULL) {
        free(part);
        return -1;
    }

    resized[*count] = part;
    *strings = resized;
    ++(*count);

    return 0;
}

static int split_at_lowercase(const char *input, char ***parts,
                              size_t *part_count)
{
    if (input == NULL || parts == NULL || part_count == NULL) {
        return -1;
    }

    *parts = NULL;
    *part_count = 0;

    const char *segment_start = input;

    for (const char *current = input; *current != '\0'; ++current) {
        if (islower((unsigned char)*current) != 0) {
            if (current > segment_start &&
                append_string(parts, part_count, segment_start,
                              (size_t)(current - segment_start)) != 0) {
                free_strings(*parts, *part_count);
                *parts = NULL;
                *part_count = 0;
                return -1;
            }

            segment_start = current + 1;
        }
    }

    if (*segment_start != '\0' &&
        append_string(parts, part_count, segment_start,
                      /* Possible weaknesses found:
                       * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
                       */
                      strlen(segment_start)) != 0) {
        free_strings(*parts, *part_count);
        *parts = NULL;
        *part_count = 0;
        return -1;
    }

    return 0;
}

/* Possible weaknesses found:
 *  Parameter 'argv' can be declared as const array [constParameter]
 */
int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s STRING\n", argv[0]);
        return EXIT_FAILURE;
    }

    char **parts = NULL;
    size_t part_count = 0;

    if (split_at_lowercase(argv[1], &parts, &part_count) != 0) {
        fputs("Failed to split string\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < part_count; ++i) {
        if (puts(parts[i]) == EOF) {
            free_strings(parts, part_count);
            return EXIT_FAILURE;
        }
    }

    free_strings(parts, part_count);
    return EXIT_SUCCESS;
}