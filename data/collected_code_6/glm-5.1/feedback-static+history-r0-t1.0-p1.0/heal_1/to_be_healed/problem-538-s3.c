#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **items;
    size_t count;
} Tuple;

Tuple string_list_to_tuple(char **list, size_t count) {
    Tuple t;
    t.count = 0;
    t.items = NULL;

    if (list == NULL || count == 0) {
        return t;
    }

    t.items = (char **)malloc(count * sizeof(char *));
    if (t.items == NULL) {
        t.count = 0;
        return t;
    }

    for (size_t i = 0; i < count; ++i) {
        if (list[i] == NULL) {
            t.items[i] = NULL;
        } else {
            /* Possible weaknesses found:
             * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
             * Flawfinder char: Statically-sized arrays can be improperly restricted, leading to potential overflows or other issues (CWE-119!/CWE-120). Perform bounds checking, use functions that limit length, or ensure that the size is larger than the maximum possible length. (risk 2, buffer)
             */
            t.items[i] = (char *)malloc(strlen(list[i]) + 1);
            if (t.items[i] == NULL) {
                for (size_t j = 0; j < i; ++j) {
                    free(t.items[j]);
                }
                free(t.items);
                t.items = NULL;
                t.count = 0;
                return t;
            }
            /* Possible weaknesses found:
             * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). (risk 4, buffer)
             */
            strcpy(t.items[i], list[i]);
        }
    }

    t.count = count;
    return t;
}

void free_tuple(Tuple *t) {
    if (t == NULL) {
        return;
    }
    if (t->items != NULL) {
        /* Possible weaknesses found:
         *  't' is a pointer; did you mean to use '->'?
         *  member reference type 'Tuple *' is a pointer; did you mean to use '->'?
         */
        for (size_t i = 0; i < t.count; ++i) {
            if (t->items[i] != NULL) {
                free(t->items[i]);
                t->items[i] = NULL;
            }
        }
        free(t->items);
        t->items = NULL;
    }
    t->count = 0;
}

int main(void) {
    char *list[] = {
        "apple",
        "banana",
        "cherry"
    };
    size_t count = sizeof(list) / sizeof(list[0]);

    Tuple t = string_list_to_tuple(list, count);

    if (t.items != NULL) {
        for (size_t i = 0; i < t.count; ++i) {
            if (t.items[i] != NULL) {
                printf("%s\n", t.items[i]);
            }
        }
    }

    free_tuple(&t);

    return 0;
}