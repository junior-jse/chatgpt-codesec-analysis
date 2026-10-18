#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 128
#define INITIAL_CAPACITY 64

typedef struct {
    char *word;
    int count;
} WordCount;

typedef struct {
    WordCount *items;
    size_t size;
    size_t capacity;
} WordCountArray;

static int word_count_compare(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    if (wb->count != wa->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
}

static int find_word_index(WordCountArray *array, const char *word) {
    for (size_t i = 0; i < array->size; i++) {
        if (strcmp(array->items[i].word, word) == 0) {
            return (int)i;
        }
    }
    return -1;
}

static int add_or_increment_word(WordCountArray *array, const char *word) {
    int index = find_word_index(array, word);
    if (index >= 0) {
        array->items[index].count++;
        return 0;
    }
    
    if (array->size >= array->capacity) {
        size_t new_capacity = array->capacity * 2;
        WordCount *new_items = realloc(array->items, new_capacity * sizeof(WordCount));
        if (!new_items) {
            return -1;
        }
        array->items = new_items;
        array->capacity = new_capacity;
    }
    
    char *word_copy = malloc(strlen(word) + 1);
    if (!word_copy) {
        return -1;
    }
    strcpy(word_copy, word);
    
    array->items[array->size].word = word_copy;
    array->items[array->size].count = 1;
    array->size++;
    return 0;
}

static void free_word_count_array(WordCountArray *array) {
    for (size_t i = 0; i < array->size; i++) {
        free(array->items[i].word);
    }
    free(array->items);
    array->items = NULL;
    array->size = 0;
    array->capacity = 0;
}

int most_common_elements(const char *text, WordCount **result, size_t *result_size) {
    if (!text || !result || !result_size) {
        return -1;
    }
    
    WordCountArray array;
    array.items = malloc(INITIAL_CAPACITY * sizeof(WordCount));
    if (!array.items) {
        return -1;
    }
    array.size = 0;
    array.capacity = INITIAL_CAPACITY;
    
    char word[MAX_WORD_LEN];
    size_t word_len = 0;
    
    for (const char *p = text; ; p++) {
        if (isalnum((unsigned char)*p) || *p == '\'') {
            if (word_len < MAX_WORD_LEN - 1) {
                word[word_len++] = (char)tolower((unsigned char)*p);
            }
        } else {
            if (word_len > 0) {
                word[word_len] = '\0';
                if (add_or_increment_word(&array, word) != 0) {
                    free_word_count_array(&array);
                    return -1;
                }
                word_len = 0;
            }
            if (*p == '\0') {
                break;
            }
        }
    }
    
    if (array.size == 0) {
        free_word_count_array(&array);
        *result = NULL;
        *result_size = 0;
        return 0;
    }
    
    qsort(array.items, array.size, sizeof(WordCount), word_count_compare);
    
    *result = array.items;
    *result_size = array.size;
    return 0;
}

void free_most_common_result(WordCount *result, size_t result_size) {
    if (!result) {
        return;
    }
    for (size_t i = 0; i < result_size; i++) {
        free(result[i].word);
    }
    free(result);
}

int main(void) {
    const char *text = "The quick brown fox jumps over the lazy dog. "
                        "The dog barks, and the fox runs away. "
                        "Quick brown fox, lazy dog.";
    
    WordCount *result = NULL;
    size_t result_size = 0;
    
    if (most_common_elements(text, &result, &result_size) != 0) {
        fprintf(stderr, "Failed to process text\n");
        return 1;
    }
    
    printf("Word frequency (most common first):\n");
    for (size_t i = 0; i < result_size; i++) {
        printf("%s: %d\n", result[i].word, result[i].count);
    }
    
    free_most_common_result(result, result_size);
    return 0;
}