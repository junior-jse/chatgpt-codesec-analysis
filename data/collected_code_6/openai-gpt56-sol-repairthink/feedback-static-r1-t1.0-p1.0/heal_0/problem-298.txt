#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct NestedList NestedList;

typedef struct {
    bool is_list;
    union {
        int value;
        NestedList *list;
    } data;
} Element;

struct NestedList {
    Element *elements;
    size_t count;
};

static bool contains_value(const NestedList *list, int value)
{
    if (list == NULL) {
        return false;
    }

    for (size_t i = 0; i < list->count; ++i) {
        const Element *element = &list->elements[i];

        if (element->is_list) {
            if (contains_value(element->data.list, value)) {
                return true;
            }
        } else if (element->data.value == value) {
            return true;
        }
    }

    return false;
}

static bool print_common_elements(const NestedList *source,
                                  const NestedList *other)
{
    if (source == NULL || other == NULL) {
        return false;
    }

    for (size_t i = 0; i < source->count; ++i) {
        const Element *element = &source->elements[i];

        if (element->is_list) {
            if (!print_common_elements(element->data.list, other)) {
                return false;
            }
        } else if (contains_value(other, element->data.value)) {
            if (printf("%d\n", element->data.value) < 0) {
                return false;
            }
        }
    }

    return true;
}

static void free_nested_list(NestedList *list)
{
    if (list == NULL) {
        return;
    }

    for (size_t i = 0; i < list->count; ++i) {
        if (list->elements[i].is_list) {
            free_nested_list(list->elements[i].data.list);
        }
    }

    free(list->elements);
    free(list);
}

static NestedList *create_list(size_t count)
{
    NestedList *list = calloc(1, sizeof(*list));

    if (list == NULL) {
        return NULL;
    }

    if (count > 0) {
        if (count > SIZE_MAX / sizeof(*list->elements)) {
            free(list);
            return NULL;
        }

        list->elements = calloc(count, sizeof(*list->elements));
        if (list->elements == NULL) {
            free(list);
            return NULL;
        }
    }

    list->count = count;
    return list;
}

int main(void)
{
    NestedList *first = create_list(3);
    NestedList *first_nested = create_list(2);
    NestedList *second = create_list(3);
    NestedList *second_nested = create_list(2);

    if (first == NULL || first_nested == NULL ||
        second == NULL || second_nested == NULL) {
        free_nested_list(first);
        free_nested_list(first_nested);
        free_nested_list(second);
        free_nested_list(second_nested);
        return EXIT_FAILURE;
    }

    first->elements[0] = (Element){ .is_list = false, .data.value = 1 };
    first->elements[1] = (Element){ .is_list = true, .data.list = first_nested };
    first->elements[2] = (Element){ .is_list = false, .data.value = 5 };
    first_nested->elements[0] = (Element){ .is_list = false, .data.value = 2 };
    first_nested->elements[1] = (Element){ .is_list = false, .data.value = 3 };

    second->elements[0] = (Element){ .is_list = false, .data.value = 3 };
    second->elements[1] = (Element){ .is_list = true, .data.list = second_nested };
    second->elements[2] = (Element){ .is_list = false, .data.value = 6 };
    second_nested->elements[0] = (Element){ .is_list = false, .data.value = 1 };
    second_nested->elements[1] = (Element){ .is_list = false, .data.value = 4 };

    bool success = print_common_elements(first, second);

    free_nested_list(first);
    free_nested_list(second);

    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}