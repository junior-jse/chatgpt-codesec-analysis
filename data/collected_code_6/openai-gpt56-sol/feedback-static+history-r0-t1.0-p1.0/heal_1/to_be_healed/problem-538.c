#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int append_text(char **buffer, size_t *length, size_t *capacity,
                       const char *text, size_t text_length)
{
    size_t required;
    /* Possible weaknesses found:
     *  The scope of the variable 'new_capacity' can be reduced. [variableScope]
     */
    size_t new_capacity;
    char *resized;

    if (text_length > SIZE_MAX - *length - 1) {
        return -1;
    }

    required = *length + text_length + 1;

    if (required > *capacity) {
        new_capacity = *capacity;

        while (new_capacity < required) {
            if (new_capacity > SIZE_MAX / 2) {
                new_capacity = required;
                break;
            }
            new_capacity *= 2;
        }

        resized = realloc(*buffer, new_capacity);
        if (resized == NULL) {
            return -1;
        }

        *buffer = resized;
        *capacity = new_capacity;
    }

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(*buffer + *length, text, text_length);
    *length += text_length;
    (*buffer)[*length] = '\0';

    return 0;
}

static char *string_list_to_tuple(const char *const strings[], size_t count)
{
    char *result;
    size_t length = 0;
    size_t capacity = 16;

    if (strings == NULL && count != 0) {
        return NULL;
    }

    result = malloc(capacity);
    if (result == NULL) {
        return NULL;
    }
    result[0] = '\0';

    if (append_text(&result, &length, &capacity, "(", 1) != 0) {
        free(result);
        return NULL;
    }

    for (size_t i = 0; i < count; ++i) {
        const char *current = strings[i];

        if (current == NULL) {
            free(result);
            return NULL;
        }

        if (i != 0 &&
            append_text(&result, &length, &capacity, ", ", 2) != 0) {
            free(result);
            return NULL;
        }

        if (append_text(&result, &length, &capacity, "\"", 1) != 0) {
            free(result);
            return NULL;
        }

        for (; *current != '\0'; ++current) {
            if (*current == '"' || *current == '\\') {
                if (append_text(&result, &length, &capacity, "\\", 1) != 0) {
                    free(result);
                    return NULL;
                }
            }

            if (append_text(&result, &length, &capacity, current, 1) != 0) {
                free(result);
                return NULL;
            }
        }

        if (append_text(&result, &length, &capacity, "\"", 1) != 0) {
            free(result);
            return NULL;
        }
    }

    if (count == 1 &&
        append_text(&result, &length, &capacity, ",", 1) != 0) {
        free(result);
        return NULL;
    }

    if (append_text(&result, &length, &capacity, ")", 1) != 0) {
        free(result);
        return NULL;
    }

    return result;
}

int main(void)
{
    const char *strings[] = {"apple", "banana", "cherry"};
    const size_t count = sizeof(strings) / sizeof(strings[0]);
    char *tuple = string_list_to_tuple(strings, count);

    if (tuple == NULL) {
        fputs("Failed to create tuple\n", stderr);
        return EXIT_FAILURE;
    }

    if (printf("%s\n", tuple) < 0) {
        free(tuple);
        return EXIT_FAILURE;
    }

    free(tuple);
    return EXIT_SUCCESS;
}