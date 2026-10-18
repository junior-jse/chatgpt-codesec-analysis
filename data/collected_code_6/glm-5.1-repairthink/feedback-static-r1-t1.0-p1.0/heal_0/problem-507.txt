#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** remove_words(char** list, int* count, const char** words_to_remove, int remove_count) {
    if (list == NULL || count == NULL || words_to_remove == NULL) {
        return NULL;
    }

    int new_count = 0;
    for (int i = 0; i < *count; i++) {
        int should_remove = 0;
        for (int j = 0; j < remove_count; j++) {
            if (strcmp(list[i], words_to_remove[j]) == 0) {
                should_remove = 1;
                break;
            }
        }
        if (should_remove) {
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

    char** new_list = realloc(list, (new_count + 1) * sizeof(char*));
    if (new_list == NULL && new_count > 0) {
        return list;
    }

    return new_list;
}

int main(void) {
    int count = 6;
    char** list = malloc((count + 1) * sizeof(char*));
    if (list == NULL) {
        return EXIT_FAILURE;
    }

    list[0] = strdup("apple");
    list[1] = strdup("banana");
    list[2] = strdup("cherry");
    list[3] = strdup("date");
    list[4] = strdup("elderberry");
    list[5] = strdup("fig");
    list[6] = NULL;

    const char* words_to_remove[] = {"banana", "date", "fig"};
    int remove_count = 3;

    list = remove_words(list, &count, words_to_remove, remove_count);
    if (list == NULL) {
        return EXIT_FAILURE;
    }

    for (int i = 0; i < count; i++) {
        printf("%s\n", list[i]);
    }

    for (int i = 0; i < count; i++) {
        free(list[i]);
    }
    free(list);

    return EXIT_SUCCESS;
}