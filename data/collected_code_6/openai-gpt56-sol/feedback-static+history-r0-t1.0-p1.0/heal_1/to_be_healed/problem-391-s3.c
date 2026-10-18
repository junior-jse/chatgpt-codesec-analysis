#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Dictionary {
    char *key;
    char *value;
    struct Dictionary *children;
    size_t child_count;
} Dictionary;

static char *duplicate_string(const char *source)
{
    size_t length;
    char *copy;

    if (source == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    length = strlen(source);
    if (length == SIZE_MAX) {
        return NULL;
    }

    copy = malloc(length + 1U);
    if (copy == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(copy, source, length + 1U);
    return copy;
}

static void free_dictionary(Dictionary *entries, size_t count)
{
    size_t i;

    if (entries == NULL) {
        return;
    }

    for (i = 0U; i < count; ++i) {
        free(entries[i].key);
        free(entries[i].value);
        free_dictionary(entries[i].children, entries[i].child_count);
    }

    free(entries);
}

static Dictionary *lists_to_nested_dictionary(
    const char *const *keys,
    const char *const *values,
    size_t list_count,
    size_t list_length)
{
    Dictionary *level;
    Dictionary *node;
    size_t depth;

    if (keys == NULL || list_count == 0U || list_length == 0U) {
        return NULL;
    }

    if (list_count > SIZE_MAX / list_length) {
        return NULL;
    }

    level = calloc(1U, sizeof(*level));
    if (level == NULL) {
        return NULL;
    }

    node = level;

    for (depth = 0U; depth < list_length; ++depth) {
        size_t column;
        /* Possible weaknesses found:
         *  The scope of the variable 'offset' can be reduced. [variableScope]
         */
        size_t offset;

        node->children = calloc(list_count, sizeof(*node->children));
        if (node->children == NULL) {
            free_dictionary(level, 1U);
            return NULL;
        }

        node->child_count = list_count;

        for (column = 0U; column < list_count; ++column) {
            offset = column * list_length + depth;

            if (keys[offset] == NULL) {
                free_dictionary(level, 1U);
                return NULL;
            }

            node->children[column].key = duplicate_string(keys[offset]);
            if (node->children[column].key == NULL) {
                free_dictionary(level, 1U);
                return NULL;
            }

            if (depth + 1U == list_length && values != NULL &&
                values[column] != NULL) {
                node->children[column].value =
                    duplicate_string(values[column]);
                if (node->children[column].value == NULL) {
                    free_dictionary(level, 1U);
                    return NULL;
                }
            }
        }

        node = &node->children[0];
    }

    return level;
}

static int print_json_string(const char *text)
{
    const unsigned char *current;

    if (text == NULL || putchar('"') == EOF) {
        return -1;
    }

    for (current = (const unsigned char *)text; *current != '\0'; ++current) {
        switch (*current) {
            case '"':
                if (fputs("\\\"", stdout) == EOF) {
                    return -1;
                }
                break;
            case '\\':
                if (fputs("\\\\", stdout) == EOF) {
                    return -1;
                }
                break;
            case '\n':
                if (fputs("\\n", stdout) == EOF) {
                    return -1;
                }
                break;
            case '\r':
                if (fputs("\\r", stdout) == EOF) {
                    return -1;
                }
                break;
            case '\t':
                if (fputs("\\t", stdout) == EOF) {
                    return -1;
                }
                break;
            default:
                if (*current < 0x20U) {
                    if (printf("\\u%04x", (unsigned int)*current) < 0) {
                        return -1;
                    }
                } else if (putchar((int)*current) == EOF) {
                    return -1;
                }
                break;
        }
    }

    return putchar('"') == EOF ? -1 : 0;
}

static int print_dictionary(const Dictionary *dictionary)
{
    size_t i;

    if (dictionary == NULL || putchar('{') == EOF) {
        return -1;
    }

    for (i = 0U; i < dictionary->child_count; ++i) {
        const Dictionary *entry = &dictionary->children[i];

        if (i != 0U && putchar(',') == EOF) {
            return -1;
        }

        if (print_json_string(entry->key) != 0 || putchar(':') == EOF) {
            return -1;
        }

        if (entry->child_count != 0U) {
            if (print_dictionary(entry) != 0) {
                return -1;
            }
        } else if (entry->value != NULL) {
            if (print_json_string(entry->value) != 0) {
                return -1;
            }
        } else if (fputs("null", stdout) == EOF) {
            return -1;
        }
    }

    return putchar('}') == EOF ? -1 : 0;
}

int main(void)
{
    const char *keys[] = {
        "users", "alice",
        "users", "bob",
        "services", "database"
    };
    const char *values[] = {
        "administrator",
        "developer",
        "online"
    };
    Dictionary *dictionary;
    int status = EXIT_SUCCESS;

    dictionary = lists_to_nested_dictionary(keys, values, 3U, 2U);
    if (dictionary == NULL) {
        fputs("Failed to create dictionary\n", stderr);
        return EXIT_FAILURE;
    }

    if (print_dictionary(dictionary) != 0 || putchar('\n') == EOF) {
        fputs("Failed to print dictionary\n", stderr);
        status = EXIT_FAILURE;
    }

    free_dictionary(dictionary, 1U);

    if (fflush(stdout) == EOF) {
        status = EXIT_FAILURE;
    }

    return status;
}