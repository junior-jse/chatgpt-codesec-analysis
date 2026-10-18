#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_LENGTH 4096

typedef struct {
    char **strings;
    size_t *counts;
    size_t size;
    size_t capacity;
} StringCounter;

static size_t safe_strnlen(const char *str, size_t max_len) {
    size_t len = 0;

    if (str == NULL) {
        return 0;
    }
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return len;
}

static char *safe_strdup(const char *str) {
    size_t len;
    size_t buf_size;
    size_t i;
    char *copy;

    if (str == NULL) {
        return NULL;
    }

    len = safe_strnlen(str, MAX_STRING_LENGTH + 1);
    if (len > MAX_STRING_LENGTH) {
        return NULL;
    }

    buf_size = len + 1;
    copy = malloc(buf_size);
    if (copy == NULL) {
        return NULL;
    }

    for (i = 0; i < len && i + 1 < buf_size; i++) {
        copy[i] = str[i];
    }
    copy[i] = '\0';

    return copy;
}

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
    /* Possible weaknesses found:
     *  The scope of the variable 'i' can be reduced. [variableScope]
     */
    size_t i;

    if (counter == NULL) {
        return;
    }

    if (counter->strings != NULL) {
        for (i = 0; i < counter->size; i++) {
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
    size_t i;

    if (counter == NULL || str == NULL || counter->size >= counter->capacity) {
        return -1;
    }

    for (i = 0; i < counter->size; i++) {
        if (strncmp(counter->strings[i], str, MAX_STRING_LENGTH + 1) == 0) {
            counter->counts[i]++;
            return 0;
        }
    }

    counter->strings[counter->size] = safe_strdup(str);
    if (counter->strings[counter->size] == NULL) {
        return -1;
    }

    counter->counts[counter->size] = 1;
    counter->size++;
    return 0;
}

char *find_second_most_frequent(const char **sequence, size_t length) {
    StringCounter counter;
    size_t first_idx = 0;
    size_t second_idx = 0;
    size_t first_count = 0;
    size_t second_count = 0;
    size_t i;
    int found_second = 0;
    char *result = NULL;

    if (sequence == NULL || length == 0) {
        return NULL;
    }

    if (init_counter(&counter, length) != 0) {
        return NULL;
    }

    for (i = 0; i < length; i++) {
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

    for (i = 0; i < counter.size; i++) {
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

    if (found_second && second_count > 0) {
        result = safe_strdup(counter.strings[second_idx]);
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