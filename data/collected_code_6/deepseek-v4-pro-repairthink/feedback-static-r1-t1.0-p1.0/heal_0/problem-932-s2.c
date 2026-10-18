#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int contains_word(char **list, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(list[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

int remove_duplicates(char ***list, int *count) {
    if (list == NULL || *list == NULL || count == NULL || *count <= 0) {
        return -1;
    }

    char **unique_list = malloc(*count * sizeof(char *));
    if (unique_list == NULL) {
        return -1;
    }

    int unique_count = 0;

    for (int i = 0; i < *count; i++) {
        if ((*list)[i] == NULL) {
            continue;
        }

        if (!contains_word(unique_list, unique_count, (*list)[i])) {
            unique_list[unique_count] = strdup((*list)[i]);
            if (unique_list[unique_count] == NULL) {
                for (int j = 0; j < unique_count; j++) {
                    free(unique_list[j]);
                }
                free(unique_list);
                return -1;
            }
            unique_count++;
        }
    }

    for (int i = 0; i < *count; i++) {
        if ((*list)[i] != NULL) {
            free((*list)[i]);
        }
    }
    free(*list);

    char **resized_list = realloc(unique_list, unique_count * sizeof(char *));
    if (resized_list == NULL && unique_count > 0) {
        for (int j = 0; j < unique_count; j++) {
            free(unique_list[j]);
        }
        free(unique_list);
        return -1;
    }
    if (unique_count == 0) {
        free(unique_list);
        *list = NULL;
    } else {
        *list = resized_list;
    }
    *count = unique_count;
    return 0;
}

int main(void) {
    const char *words[] = {"apple", "banana", "apple", "cherry", "banana", "date", "apple"};
    int count = sizeof(words) / sizeof(words[0]);

    char **list = malloc(count * sizeof(char *));
    if (list == NULL) {
        return 1;
    }

    for (int i = 0; i < count; i++) {
        list[i] = strdup(words[i]);
        if (list[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(list[j]);
            }
            free(list);
            return 1;
        }
    }

    if (remove_duplicates(&list, &count) != 0) {
        for (int i = 0; i < count; i++) {
            free(list[i]);
        }
        free(list);
        return 1;
    }

    for (int i = 0; i < count; i++) {
        printf("%s\n", list[i]);
        free(list[i]);
    }
    free(list);

    return 0;
}