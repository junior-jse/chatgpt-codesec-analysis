#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** remove_words(char** list, int* count, const char* word) {
    if (!list || !count || !word) {
        return list;
    }

    int new_count = 0;
    for (int i = 0; i < *count; i++) {
        if (strcmp(list[i], word) == 0) {
            free(list[i]);
            list[i] = NULL;
        } else {
            list[new_count++] = list[i];
        }
    }

    for (int i = new_count; i < *count; i++) {
        list[i] = NULL;
    }

    *count = new_count;

    if (new_count == 0) {
        free(list);
        return NULL;
    }

    char** temp = realloc(list, new_count * sizeof(char*));
    if (temp) {
        list = temp;
    }

    return list;
}

char** create_list(const char* items[], int size) {
    char** list = malloc(size * sizeof(char*));
    if (!list) {
        return NULL;
    }
    for (int i = 0; i < size; i++) {
        list[i] = malloc(strlen(items[i]) + 1);
        if (!list[i]) {
            for (int j = 0; j < i; j++) {
                free(list[j]);
            }
            free(list);
            return NULL;
        }
        strcpy(list[i], items[i]);
    }
    return list;
}

void free_list(char** list, int count) {
    if (!list) {
        return;
    }
    for (int i = 0; i < count; i++) {
        free(list[i]);
    }
    free(list);
}

int main(void) {
    const char* original[] = {"apple", "banana", "cherry", "banana", "date"};
    int count = 5;

    char** list = create_list(original, count);
    if (!list) {
        return 1;
    }

    list = remove_words(list, &count, "banana");
    
    for (int i = 0; i < count; i++) {
        printf("%s\n", list[i]);
    }

    free_list(list, count);

    return 0;
}