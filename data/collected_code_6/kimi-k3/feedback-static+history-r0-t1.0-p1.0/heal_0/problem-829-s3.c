#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **strings;
    size_t *counts;
    size_t size;
    size_t capacity;
} StringCounter;

static int init_counter(StringCounter *counter, size_t capacity) {
    if (counter == NULL || capacity == 0) {
        return -1;
    }
    counter->strings = calloc(capacity, sizeof(char *));
    if (counter->strings == NULL) {
        return -1;
    }
    counter->counts = calloc(capacity, sizeof(size_t));
    if (counter->counts == NULL) {
        free(counter->strings);
        counter->strings = NULL;
        return -1;
    }
    counter->size = 0;
    counter->capacity = capacity;
    return 0;
}

static void free_counter(StringCounter *counter) {
    if (counter == NULL) {
        return;
    }
    if (counter->strings != NULL) {
        for (size_t i = 0; i < counter->size; i++) {
            free(counter->strings[i]);
        }
        free(counter->strings);
    }
    free(counter->counts);
    counter->strings = NULL;
    counter->counts = NULL;
    counter->size = 0;
    counter->capacity = 0;
}

static int add_string(StringCounter *counter, const char *str) {
    if (counter == NULL || str == NULL || counter->size >= counter->capacity) {
        return -1;
    }
    for (size_t i = 0; i < counter->size; i++) {
        if (strcmp(counter->strings[i], str) == 0) {
            counter->counts[i]++;
            return 0;
        }
    }
    counter->strings[counter->size] = malloc(strlen(str) + 1);
    if (counter->strings[counter->size] == NULL) {
        return -1;
    }
    strcpy(counter->strings[counter->size], str);
    counter->counts[counter->size] = 1;
    counter->size++;
    return 0;
}

char *find_second_most_frequent(const char **sequence, size_t length) {
    if (sequence == NULL || length == 0) {
        return NULL;
    }

    StringCounter counter;
    if (init_counter(&counter, length) != 0) {
        return NULL;
    }

    for (size_t i = 0; i < length; i++) {
        if (sequence[i] == NULL) {
            free_counter(&counter);
            return NULL;
        }
        if (add_string(&counter, sequence[i]) != 0) {
            free_counter(&counter);
            return NULL;
        }
    }

    if (counter.size < 2) {
        free_counter(&counter);
        return NULL;
    }

    size_t first_idx = 0;
    size_t second_idx = 0;
    size_t first_count = 0;
    size_t second_count = 0;
    int found_second = 0;

    for (size_t i = 0; i < counter.size; i++) {
        if (counter.counts[i] > first_count) {
            second_count = first_count;
            second_idx = first_idx;
            first_count = counter.counts[i];
            first_idx = i;
            found_second = (second_count > 0) ? 1 : found_second;
        } else if (counter.counts[i] < first_count && counter.counts[i] > second_count) {
            second_count = counter.counts[i];
            second_idx = i;
            found_second = 1;
        }
    }

    char *result = NULL;
    if (found_second && second_count > 0) {
        result = malloc(strlen(counter.strings[second_idx]) + 1);
        if (result != NULL) {
            strcpy(result, counter.strings[second_idx]);
        }
    }

    free_counter(&counter);
    return result;
}

int main(void) {
    const char *sequence[] = {
        "apple", "banana", "apple", "cherry",
        "banana", "apple", "date", "banana"
    };
    size_t length = sizeof(sequence) / sizeof(sequence[0]);

    char *result = find_second_most_frequent(sequence, length);
    if (result != NULL) {
        printf("Second most frequent string: %s\n", result);
        free(result);
    } else {
        printf("No second most frequent string found.\n");
    }

    return 0;
}