#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int grow_buffer(char **buffer, size_t *capacity, size_t required)
{
    size_t new_capacity;
    char *temporary;

    if (required <= *capacity) {
        return 0;
    }

    new_capacity = *capacity;
    while (new_capacity < required) {
        if (new_capacity > SIZE_MAX / 2U) {
            return -1;
        }
        new_capacity *= 2U;
    }

    temporary = realloc(*buffer, new_capacity);
    if (temporary == NULL) {
        return -1;
    }

    *buffer = temporary;
    *capacity = new_capacity;
    return 0;
}

static char *camel_to_snake(const char *input)
{
    static const char pattern[] = "([[:lower:][:digit:]])([[:upper:]])";
    regex_t regex;
    regmatch_t matches[3];
    char *current;
    char *next;
    size_t capacity;
    int regex_status;

    if (input == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    capacity = strlen(input) + 1U;
    current = malloc(capacity);
    if (current == NULL) {
        return NULL;
    }
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(current, input, capacity);

    regex_status = regcomp(&regex, pattern, REG_EXTENDED);
    if (regex_status != 0) {
        free(current);
        return NULL;
    }

    for (;;) {
        size_t length;
        size_t required;
        size_t first_end;
        size_t second_start;

        regex_status = regexec(&regex, current, 3U, matches, 0);
        if (regex_status == REG_NOMATCH) {
            break;
        }
        if (regex_status != 0) {
            regfree(&regex);
            free(current);
            return NULL;
        }

        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        length = strlen(current);
        first_end = (size_t)matches[1].rm_eo;
        second_start = (size_t)matches[2].rm_so;

        if (length == SIZE_MAX || first_end > length || second_start > length) {
            regfree(&regex);
            free(current);
            return NULL;
        }

        /* Possible weaknesses found:
         *  required is assigned 'length+2U' here.
         */
        required = length + 2U;
        /* Possible weaknesses found:
         *  Condition 'required<length' is always false [knownConditionTrueFalse]
         *  Condition 'required<length' is always false
         */
        if (required < length) {
            regfree(&regex);
            free(current);
            return NULL;
        }

        next = malloc(required);
        if (next == NULL) {
            regfree(&regex);
            free(current);
            return NULL;
        }

        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(next, current, first_end);
        next[first_end] = '_';
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(next + first_end + 1U, current + second_start, length - second_start + 1U);

        free(current);
        current = next;
        capacity = required;
    }

    regfree(&regex);
    (void)capacity;
    return current;
}

/* Possible weaknesses found:
 *  Parameter 'argv' can be declared as const array [constParameter]
 */
int main(int argc, char *argv[])
{
    char *result;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <camelCaseString>\n", argv[0]);
        return EXIT_FAILURE;
    }

    result = camel_to_snake(argv[1]);
    if (result == NULL) {
        fputs("Conversion failed\n", stderr);
        return EXIT_FAILURE;
    }

    if (printf("%s\n", result) < 0) {
        free(result);
        return EXIT_FAILURE;
    }

    free(result);
    return EXIT_SUCCESS;
}