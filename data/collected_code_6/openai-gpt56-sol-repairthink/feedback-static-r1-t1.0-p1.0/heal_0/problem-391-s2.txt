#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Dictionary {
    char *key;
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

    length = strlen(source);
    copy = malloc(length + 1);
    if (copy == NULL) {
        return NULL;
    }

    memcpy(copy, source, length + 1);
    return copy;
}

static void free_dictionary(Dictionary *dictionary)
{
    size_t i;

    if (dictionary == NULL) {
        return;
    }

    for (i = 0; i < dictionary->child_count; ++i) {
        free_dictionary(&dictionary->children[i]);
    }

    free(dictionary->children);
    free(dictionary->key);
    dictionary->children = NULL;
    dictionary->key = NULL;
    dictionary->child_count = 0;
}

static int initialize_dictionary(Dictionary *dictionary, const char *key)
{
    if (dictionary == NULL || key == NULL) {
        return -1;
    }

    dictionary->key = duplicate_string(key);
    if (dictionary->key == NULL) {
        return -1;
    }

    dictionary->children = NULL;
    dictionary->child_count = 0;
    return 0;
}

static Dictionary *find_child(Dictionary *parent, const char *key)
{
    size_t i;

    if (parent == NULL || key == NULL) {
        return NULL;
    }

    for (i = 0; i < parent->child_count; ++i) {
        if (strcmp(parent->children[i].key, key) == 0) {
            return &parent->children[i];
        }
    }

    return NULL;
}

static Dictionary *add_child(Dictionary *parent, const char *key)
{
    Dictionary *resized;
    Dictionary *child;
    size_t new_count;

    if (parent == NULL || key == NULL || parent->child_count == SIZE_MAX) {
        return NULL;
    }

    new_count = parent->child_count + 1;
    if (new_count > SIZE_MAX / sizeof(*parent->children)) {
        return NULL;
    }

    resized = realloc(parent->children, new_count * sizeof(*resized));
    if (resized == NULL) {
        return NULL;
    }

    parent->children = resized;
    child = &parent->children[parent->child_count];
    child->key = NULL;
    child->children = NULL;
    child->child_count = 0;

    if (initialize_dictionary(child, key) != 0) {
        return NULL;
    }

    parent->child_count = new_count;
    return child;
}

static int lists_to_nested_dictionary(
    const char *const *const *lists,
    const size_t *lengths,
    size_t list_count,
    Dictionary *root)
{
    size_t i;
    size_t j;

    if (lists == NULL || lengths == NULL || root == NULL) {
        return -1;
    }

    if (initialize_dictionary(root, "root") != 0) {
        return -1;
    }

    for (i = 0; i < list_count; ++i) {
        Dictionary *current = root;

        if (lists[i] == NULL && lengths[i] != 0) {
            free_dictionary(root);
            return -1;
        }

        for (j = 0; j < lengths[i]; ++j) {
            Dictionary *next;

            if (lists[i][j] == NULL) {
                free_dictionary(root);
                return -1;
            }

            next = find_child(current, lists[i][j]);
            if (next == NULL) {
                next = add_child(current, lists[i][j]);
                if (next == NULL) {
                    free_dictionary(root);
                    return -1;
                }
            }

            current = next;
        }
    }

    return 0;
}

static int print_escaped_string(const char *text)
{
    const unsigned char *position;

    if (text == NULL || putchar('"') == EOF) {
        return -1;
    }

    position = (const unsigned char *)text;
    while (*position != '\0') {
        switch (*position) {
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
                if (putchar(*position) == EOF) {
                    return -1;
                }
                break;
        }
        ++position;
    }

    return putchar('"') == EOF ? -1 : 0;
}

static int print_dictionary(const Dictionary *dictionary)
{
    size_t i;

    if (dictionary == NULL || putchar('{') == EOF) {
        return -1;
    }

    for (i = 0; i < dictionary->child_count; ++i) {
        if (i != 0 && fputs(", ", stdout) == EOF) {
            return -1;
        }

        if (print_escaped_string(dictionary->children[i].key) != 0 ||
            fputs(": ", stdout) == EOF ||
            print_dictionary(&dictionary->children[i]) != 0) {
            return -1;
        }
    }

    return putchar('}') == EOF ? -1 : 0;
}

int main(void)
{
    const char *list1[] = {"users", "alice", "email"};
    const char *list2[] = {"users", "alice", "address"};
    const char *list3[] = {"users", "bob", "email"};
    const char *list4[] = {"settings", "theme"};
    const char *const *lists[] = {list1, list2, list3, list4};
    const size_t lengths[] = {
        sizeof(list1) / sizeof(list1[0]),
        sizeof(list2) / sizeof(list2[0]),
        sizeof(list3) / sizeof(list3[0]),
        sizeof(list4) / sizeof(list4[0])
    };
    Dictionary root = {0};
    int result;

    result = lists_to_nested_dictionary(
        lists,
        lengths,
        sizeof(lists) / sizeof(lists[0]),
        &root);

    if (result != 0) {
        fputs("Failed to create dictionary.\n", stderr);
        return EXIT_FAILURE;
    }

    if (print_dictionary(&root) != 0 || putchar('\n') == EOF) {
        free_dictionary(&root);
        fputs("Failed to print dictionary.\n", stderr);
        return EXIT_FAILURE;
    }

    free_dictionary(&root);
    return EXIT_SUCCESS;
}