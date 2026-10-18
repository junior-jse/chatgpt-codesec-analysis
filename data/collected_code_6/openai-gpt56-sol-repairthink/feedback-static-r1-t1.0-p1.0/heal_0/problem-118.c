#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct StringList {
    char **items;
    size_t count;
} StringList;

static void string_list_free(StringList *list)
{
    if (list == NULL) {
        return;
    }

    for (size_t i = 0; i < list->count; ++i) {
        free(list->items[i]);
    }

    free(list->items);
    list->items = NULL;
    list->count = 0;
}

static int string_list_append(StringList *list, const char *start, size_t length)
{
    if (list == NULL || start == NULL || length == SIZE_MAX) {
        return -1;
    }

    if (list->count == SIZE_MAX / sizeof(*list->items)) {
        return -1;
    }

    char *item = malloc(length + 1);
    if (item == NULL) {
        return -1;
    }

    memcpy(item, start, length);
    item[length] = '\0';

    char **new_items = realloc(
        list->items,
        (list->count + 1) * sizeof(*list->items)
    );

    if (new_items == NULL) {
        free(item);
        return -1;
    }

    list->items = new_items;
    list->items[list->count] = item;
    ++list->count;

    return 0;
}

static int string_to_list(const char *text, char delimiter, StringList *result)
{
    if (text == NULL || result == NULL || delimiter == '\0') {
        return -1;
    }

    result->items = NULL;
    result->count = 0;

    const char *start = text;
    const char *current = text;

    for (;;) {
        if (*current == delimiter || *current == '\0') {
            if (string_list_append(result, start, (size_t)(current - start)) != 0) {
                string_list_free(result);
                return -1;
            }

            if (*current == '\0') {
                break;
            }

            start = current + 1;
        }

        ++current;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 3 || argv[2][0] == '\0' || argv[2][1] != '\0') {
        fprintf(stderr, "Usage: %s <string> <delimiter>\n", argv[0]);
        return EXIT_FAILURE;
    }

    StringList list;

    if (string_to_list(argv[1], argv[2][0], &list) != 0) {
        fprintf(stderr, "Failed to convert string to list: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < list.count; ++i) {
        if (printf("%s\n", list.items[i]) < 0) {
            string_list_free(&list);
            return EXIT_FAILURE;
        }
    }

    string_list_free(&list);

    if (fflush(stdout) == EOF) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}